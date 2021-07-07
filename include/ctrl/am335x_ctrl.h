// SPDX-License-Identifier: GPL-3.0
/*
 * am335x_ctrl.h - controller implementations for am335x based systems
 *
 * Copyright (c) 2021 PL Germany
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
#include <linux/io.h>
#include <linux/string.h>
#include <linux/platform_device.h>

#include <ctrl/controller.h>

#define AM335X_TCON_CLK_IDENTIFIER      "l4_per_cm:clk:0004:0"

struct am335x_ctrl {
        struct controller ctrl;
        struct device timing_dev;
        struct device pol_dev;
        struct resource *hw_res;
        struct clk *hw_clk;
        void __iomem *reg_base_addr;
        short cmd;
};
#define to_am335x_ctrl(x) container_of(x, struct am335x_ctrl, ctrl)

struct controller *am335x_ctrl_create(void);

#endif /* AM335X_CTRL_H */