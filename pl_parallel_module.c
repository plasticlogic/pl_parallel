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
#include <linux/of_device.h>
#include <linux/mod_devicetable.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/err.h>

#include <ctrl/controller.h>
#include <ctrl/am335x_ctrl.h>

#define DEVICE_NAME     "parallel"
#define CLASS_NAME      "pl_par"

static struct cdev *pl_parallel_cdev = NULL;
static struct controller *ctrl = NULL;
static dev_t cdev_dev_t = 0;

////////////////////////////////////////////////////////////////////////////////
// Cdev

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
        int ret = 0;
        unsigned long c, cnt = 0;
        unsigned char *read_buffer, *src, *dst;
        
        read_buffer = kmalloc(size, GFP_KERNEL);
        if(!read_buffer)
                return -ENOMEM;

        ret = ctrl->read(ctrl, (unsigned short *)read_buffer, size / 2);
        if(ret < 0)
                goto err;

        src = read_buffer;
        dst = data;

        while(size) {
                c = (size > PAGE_SIZE) ? PAGE_SIZE : size;

                if(copy_to_user(dst, src, c)) {
                        ret = -EFAULT;
                        break;
                }

                src += c;
                dst += c;
                cnt += c;       
                size -= c;
        }

err:
        kfree(read_buffer);
        return (ret) ? ret : cnt;
}

static ssize_t pl_parallel_write(struct file *file, const char __user *data,
                                 size_t size, loff_t *offset)
{
        unsigned char *data_buf, *dst;
        const unsigned char *src;
        unsigned long c, cnt = 0;
        int ret = 0;

        if(size < 2)
                return -EINVAL;

        data_buf = kmalloc(size, GFP_KERNEL);
        if(!data_buf)
                return -ENOMEM;

        src = data;
        dst = data_buf;

        while(size) {
                c = (size > PAGE_SIZE) ? PAGE_SIZE : size;
                if(copy_from_user(dst, src, c)) {
                        ret = -EFAULT;
                        goto err;
                }

                dst += c;
                src += c;
                cnt += c;
                size -= c;
        }

        ret = ctrl->write(ctrl, (unsigned short *)data_buf, cnt / 2);
        
err:
        kfree(data_buf);
        return (ret) ? ret : cnt;
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

enum pl_parallel_type {
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

static struct controller *get_controller_by_dev_id(struct platform_device *pdev,
                                        struct platform_device_id *id, 
                                        struct class *c)
{
        struct controller *ctrl;

        switch(id->driver_data) {
        case AM335X:
                pr_info("%s: Create am335x ctrl device.\n", THIS_MODULE->name);
                ctrl = am335x_ctrl_create();
                break;
        default:
                ctrl = ERR_PTR(-ENODEV);
                break;
        }
        return ctrl;
}

////////////////////////////////////////////////////////////////////////////////
// Class

static ssize_t burst_en_show(struct class *c, struct class_attribute *attr, 
                             char *buffer)
{
        if(!ctrl)
                return -ENODEV;
        else
                return sprintf(buffer, "%d\n", ctrl->burst_en);
}

static ssize_t burst_en_store(struct class *c, struct class_attribute *attr, 
                              const char *buffer, size_t len)
{
        int ret;
        if(!ctrl)
                return -ENODEV;
        
        ret = kstrtoint(buffer, 10, &ctrl->burst_en);

        if(ret) {
                ctrl->burst_en = 0;
                return ret;
        }
        return len;
}

CLASS_ATTR_RW(burst_en);

static struct attribute *pl_par_attrs[] = {
        &class_attr_burst_en.attr,
        NULL,
};

ATTRIBUTE_GROUPS(pl_par);

static struct class pl_parallel_class = {
        .name = CLASS_NAME,
        .owner = THIS_MODULE,
        .class_groups = pl_par_groups,
};

////////////////////////////////////////////////////////////////////////////////
// Driver

static int pl_parallel_probe(struct platform_device *pdev)
{
        int ret;
        struct device *cdev_dev_tmp;
        struct platform_device_id *dev_id;
        // find and create device
        const struct of_device_id *of_id = 
                of_match_device(pl_parallel_dt_ids, &pdev->dev);

        if(!of_id) {
                pr_err("%s: Cannot find any compatible hardware.\n", 
                        THIS_MODULE->name);
                ret = -ENODEV;
                goto of_match_fail;
        }

        // register class
        ret = class_register(&pl_parallel_class);
        if(ret) {
                dev_err(&pdev->dev, "Register class failed.\n");
                goto class_register_fail;
        }
        
        dev_info(&pdev->dev, "Create cdev\n");
        // create cdev
        ret = alloc_chrdev_region(&cdev_dev_t, 0, 1, DEVICE_NAME);
        if(ret) {
                dev_err(&pdev->dev, "Alloc cdev region failed.\n");
                goto cdev_region_alloc_fail;
        }

        pl_parallel_cdev = cdev_alloc();
        if(!pl_parallel_cdev) {
                dev_err(&pdev->dev, "Alloc cdev_failed.\n");
                ret = -ENOMEM;
                goto cdev_alloc_fail;
        }

        cdev_init(pl_parallel_cdev, &pl_parallel_fops);
        ret = cdev_add(pl_parallel_cdev, cdev_dev_t, 1);
        if(ret) {
                dev_err(&pdev->dev, "Adding cdev failed.\n");
                goto add_cdev_fail;
        }

        cdev_dev_tmp = device_create(&pl_parallel_class, NULL, cdev_dev_t, 
                                     NULL, DEVICE_NAME);
        if(IS_ERR(cdev_dev_tmp)) {
                dev_err(&pdev->dev, "Create cdev device failed.");
                ret = PTR_ERR(cdev_dev_tmp);
                goto cdev_dev_create_fail;
        }

        // create device
        dev_id = (struct platform_device_id *)of_id->data;

        ctrl = get_controller_by_dev_id(pdev, dev_id, &pl_parallel_class);
        if(IS_ERR(ctrl)) {
                dev_err(&pdev->dev, "Create parallel device failed.\n");
                ret = PTR_ERR(ctrl);
                goto create_dev_fail;
        }

        ret = ctrl->init(ctrl, pdev, &pl_parallel_class);
        if(ret) {
                dev_err(&pdev->dev, "Init parallel device failed.\n");
                goto init_dev_fail;
        }

        return 0;

init_dev_fail:
        ctrl->destroy(ctrl, pdev, &pl_parallel_class);
cdev_dev_create_fail:
        cdev_del(pl_parallel_cdev);
create_dev_fail:
add_cdev_fail:
        devm_kfree(&pdev->dev, pl_parallel_cdev);
cdev_alloc_fail:
        unregister_chrdev_region(cdev_dev_t, 1);
cdev_region_alloc_fail:
        class_unregister(&pl_parallel_class);
class_register_fail:
of_match_fail:
        return ret;
}

static int pl_parallel_remove(struct platform_device *pdev)
{
        ctrl->destroy(ctrl, pdev, &pl_parallel_class);
        device_destroy(&pl_parallel_class, cdev_dev_t);
        class_unregister(&pl_parallel_class);
        cdev_del(pl_parallel_cdev);
        unregister_chrdev_region(cdev_dev_t, 1);
        return 0;
}

static struct platform_driver pl_parallel_driver = {
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

static void __exit pl_parallel_exit(void)
{
        pr_info("%s: Exiting module...\n", THIS_MODULE->name);
        platform_driver_unregister(&pl_parallel_driver);
}

module_init(pl_parallel_init);
module_exit(pl_parallel_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("v1.2.4");
MODULE_DESCRIPTION("Parallel driver for PL Germany devices.");
MODULE_AUTHOR("Lars Görner <lars.goerner@plasticlogic.com>");