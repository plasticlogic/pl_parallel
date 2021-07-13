// SPDX-License-Identifier: GPL-3.0
/*
 * ctrl.h - controller interface
 *
 * Copyright (c) 2021 PL Germany
 * 
 * Authors
 *      Lars Görner <lars.goerner@plasticlogic.com>
 *
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/clk.h>
#include <linux/ioport.h>

#define PAR_CTRL_NAME   "tcon"
#define HRDY_GPIO_ID    "hrdy"

struct controller {
        int (*init)(struct controller *ctrl, struct platform_device *pdev, 
                struct class *c);
        ssize_t (*read)(struct controller *ctrl, short *buf, size_t len);
        ssize_t (*write)(struct controller *ctrl, const short *buf, size_t len);
        void (*destroy)(struct controller *ctrl, struct platform_device *pdev,
                        struct class *c);
        int burst_en;
};

#endif /* CONTROLLER_H */