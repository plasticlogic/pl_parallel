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
#include <linux/errno.h>
#include <linux/err.h>

#define DEVICE_NAME     "parallel"

static dev_t pl_parallel_dev_t = 0;
static struct class *pl_parallel_class;
static struct cdev *pl_parallel_cdev;

////////////////////////////////////////////////////////////////////////////////
// Cdev

static int pl_parallel_open(struct inode *inode, struct file *file)
{

}

static int pl_parallel_release(struct inode *inode, struct file *file)
{

}

static ssize_t pl_parallel_read(struct file *file, char __user *data,
                                size_t size, loff_t *offset)
{

}

static ssize_t pl_parallel_write(struct file *file, const char __user *data,
                                 size_t size, loff_t *offset)
{

}

static struct file_operations pl_parallel_fops = {
        .owner = THIS_MODULE,
        .open = pl_parallel_open,
        .release = pl_parallel_release,
        .read = pl_parallel_read,
        .write = pl_parallel_write,
};

////////////////////////////////////////////////////////////////////////////////
// Driver

static int pl_parallel_probe(struct platform_device *pdev)
{
        int ret;

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

        return 0;

        // class_destroy(pl_parallel_class);
create_class_fail:
        cdev_del(pl_parallel_cdev);
add_cdev_fail:
alloc_cdev_fail:
        unregister_chrdev_region(pl_parallel_dev_t, 1);
alloc_cdev_region_fail:
        return ret;
}

static int pl_parallel_remove(struct platform_device *pdev)
{
        class_destroy(pl_parallel_class);
        cdev_del(pl_parallel_cdev);
        unregister_chrdev_region(pl_parallel_dev_t, 1);
}

static const platform_driver pl_parallel_driver = {
        .driver = {
                .name = DEVICE_NAME,
                .owner = THIS_MODULE,
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