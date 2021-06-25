// SPDX-License-Identifier: GPL-3.0
/*
 * am335x_ctrl.h - controller implementations for am335x based systems
 *
 * Copyright (c) 2001-2003 PL Germany
 * 
 * Authors 
 *      Lars Görner <lars.goerner@plasticlogic.com>
 *
 */

#ifndef AM335X_CTRL_H
#define AM335X_CTRL_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/ioport.h>
#include <linux/clk.h>
#include <linux/string.h>

#include <controller.h>

struct am335x_ctrl {
        struct controller ctrl;
        struct clk *hw_clk;
        struct resource *hw_res;
};
#define to_am335x_ctrl(x) container_of(x, struct am335x_ctrl, ctrl.kobj)

struct am335x_attribute {
        struct attribute attr;
        ssize_t (*show)(struct am335x_ctrl *ctrl, 
                        struct am335_x_attribute *attr, char *buf);
        ssize_t (*store)(struct am335x_ctrl *ctrl,
                         struct am335x_attribute *attr,
                         const char *buf, size_t count);
};
#define to_am335x_attr(x) container_of(x, struct am335x_attribute, attr)

struct controller *am335x_ctrl_create(struct device *dev);

#endif /* AM335X_CTRL_H */