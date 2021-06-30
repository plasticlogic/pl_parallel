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
#include <linux/gpio/consumer.h>

#define PAR_CTRL_NAME "ctrl"
#define HRDY_GPIO_ID    "hrdy"

struct controller {
        struct kobject kobj;
        struct device *dev;
        int (*init)(struct controller *ctrl);
        ssize_t (*read)(struct controller *ctrl, short addr, 
                        short *buf, size_t len);
        ssize_t (*write)(struct controller *ctrl, short addr, 
                         const short *buf, size_t len);
        void (*destroy)(struct controller *ctrl);
        struct gpio_desc *hrdy_gpio;
};
#define to_controller(x) container_of(x, struct controller, kobj)

#endif /* CONTROLLER_H */