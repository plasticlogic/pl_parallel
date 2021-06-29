// SPDX-License-Identifier: GPL-3.0
/*
 * pl_parallel_module.c - module entry point
 *
 * Copyright (c) 2001-2003 PL Germany
 * 
 * Authors 
 *      Lars Görner <lars.goerner@plasticlogic.com>
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/kobject.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/err.h>

#include <ctrl/controller.h>
#include <ctrl/am335x_ctrl.h>

#define DEVICE_NAME     "parallel"

static dev_t pl_parallel_dev_t = 0;
static struct class *pl_parallel_class;
static struct cdev *pl_parallel_cdev;
static struct controller *ctrl;

////////////////////////////////////////////////////////////////////////////////
// Cdev

static short *buffer = NULL;

static int pl_parallel_open(struct inode *inode, struct file *file)
{
        return 0;
}

static int pl_parallel_release(struct inode *inode, struct file *file)
{
        return 0;
}

static ssize_t pl_parallel_read(struct file *file, char __user *data,
                                size_t size, loff_t *offset)
{
        return -EPERM;
}

static ssize_t pl_parallel_write(struct file *file, const char __user *data,
                                 size_t size, loff_t *offset)
{
        int copy_size, not_copied;
        short* buffer;
        int ret;

        buffer = kzalloc(size, GFP_KERNEL);
        if(!buffer) {
                dev_err(ctrl->dev, "Allocate buffer failed.");
                ret = -ENOMEM;
                goto alloc_buf_mem_fail;
        }

        ret = copy_from_user(buffer, data, size);
        if(ret) {
                dev_err(ctrl->dev, "Copy data from user space failed.");
                goto copy_user_data_fail;
        }

        ret = ctrl->write(ctrl, buffer[0], buffer[1], size - 1);
        kfree(buffer);
        return ret;

copy_user_data_fail:
        kfree(buffer);
alloc_buf_mem_fail:
        return ret;
}

static struct file_operations pl_parallel_fops = {
        .owner = THIS_MODULE,
        .open = pl_parallel_open,
        .release = pl_parallel_release,
        .read = pl_parallel_read,
        .write = pl_parallel_write,
};

////////////////////////////////////////////////////////////////////////////////
// Devices

static enum pl_parallel_type {
        AM335X,
};

static const struct platform_device_id pl_parallel_devtype[] = {
        { .name = "lcdc", .driver_data = AM335X },
        { /* sentinal */}
};
MODULE_DEVICE_TABLE(platform, pl_parallel_devtype);

static const struct of_device_id pl_parallel_dt_ids[] = {
        {
                .compatible = "ti,am33xx-tilcdc",
                .data = &pl_parallel_devtype[AM335X]
        },
        { /* sentinal */}
};
MODULE_DEVICE_TABLE(of, pl_parallel_dt_ids);

static struct controller *get_controller_by_dev_id(
                                        struct platform_device_id *id, 
                                        struct class *c)
{
        struct controller *ctrl;

        switch(id->driver_data) {
        case AM335X:
                ctrl = am335x_ctrl_create(c);
                break;
        default:
                ctrl = ERR_PTR(-ENODEV);
                break;
        }
        return ctrl;
}

////////////////////////////////////////////////////////////////////////////////
// Driver

static int pl_parallel_probe(struct platform_device *pdev)
{
        int ret;
        struct platform_device_id *dev_id;

        // find and create device
        const struct of_device_id *of_id = 
                of_match_device(pl_parallel_dt_ids, &pdev->dev);

        if(!of_id) {
                pr_err("%s: Cannot find any compatible hardware.", 
                        THIS_MODULE->name);
                ret = -ENODEV;
                goto of_match_fail;
        }

        // create cdev
        ret = alloc_chrdev_region(&pl_parallel_dev_t, 0, 1, DEVICE_NAME); // ???
        if(result) {
                pr_err("%s: Alloc cdev region failed.\n", THIS_MODULE->name);
                goto alloc_cdev_region_fail;
        }

        pl_parallel_cdev = cdev_alloc();
        if(!pl_parallel_cdev) {
                ret = -ENOMEM;
                pr_err("%s: Alloc cdev failed.\n", THIS_MODULE->name);
                goto alloc_cdev_fail;
        }

        cdev_init(pl_parallel_cdev, &pl_parallel_fops);
        ret = cdev_add(pl_parallel_cdev, pl_parallel_dev_t, 1); // ???
        if(ret) {
                pr_err("%s: Add cdev failed.\n", THIS_MODULE->name);
                goto add_cdev_fail;
        }

        // create class
        pl_parallel_class = class_create(THIS_MODULE, DEVICE_NAME);
        if(!pl_parallel_class) {
                ret = -EEXIST;
                pr_err("%s: Create class failed.\n", THIS_MODULE->name);
                goto create_class_fail;
        }

        // create device
        ctrl = get_controller_by_dev_id(of_id->data, pl_parallel_class);
        if(IS_ERR(ctrl)) {
                dev_err(&pdev->dev, "Create parallel device failed.");
                ret = PTR_ERR(ctrl);
                goto create_dev_fail;
        }
        ctrl->dev = &pdev->dev;

        ret = ctrl->init(ctrl);
        if(ret) {
                dev_err(&pdev->dev, "Init parallel device failed.");
        }



        return 0;

create_dev_fail:
        class_destroy(pl_parallel_class);
create_class_fail:
        cdev_del(pl_parallel_cdev);
add_cdev_fail:
alloc_cdev_fail:
        unregister_chrdev_region(pl_parallel_dev_t, 1);
alloc_cdev_region_fail:
of_match_fail:
        return ret;
}

static int pl_parallel_remove(struct platform_device *pdev)
{
        ctrl->destroy(ctrl);
        class_destroy(pl_parallel_class);
        cdev_del(pl_parallel_cdev);
        unregister_chrdev_region(pl_parallel_dev_t, 1);
}

static const platform_driver pl_parallel_driver = {
        .driver = {
                .name = DEVICE_NAME,
                .owner = THIS_MODULE,
                .of_match_table = pl_parallel_dt_ids,
        },
        .probe = pl_parallel_probe,
        .remove = pl_parallel_remove,
};

////////////////////////////////////////////////////////////////////////////////
// Module 

static int __init pl_parallel_init(void)
{
        pr_info("%s: Starting module...\n", THIS_MODULE->name);
        return platform_driver_probe(&pl_parallel_driver, pl_parallel_probe);
}

static void ___exit pl_parallel_exit(void)
{
        pr_info("%s: Exiting module...\n", THIS_MODULE->name);
        platform_driver_unregister(&pl_parallel_driver);
}

module_init(pl_parallel_init);
module_exit(pl_parallel_exit);

MODULE_LICENCE("GPL v3");
MODULE_VERSION("v1.0.0");
MODULE_DESCRIPTION("Parallel driver for PL Germany devices.");
MODULE_AUTHOR("Lars Görner <lars.goerner@plasticlogic.com>");