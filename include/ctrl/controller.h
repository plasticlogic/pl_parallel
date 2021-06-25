// SPDX-License-Identifier: GPL-3.0
/*
 * ctrl.h - controller interface
 *
 * Copyright (c) 2001-2003 PL Germany
 * 
 * Authors
 *      Lars Görner <lars.goerner@plasticlogic.com>
 *
 */

#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/gpio/consumer.h>

#define HRDY_GPIO_ID    "hrdy"

struct controller {
        struct kobject kobj;
        struct device *dev;
        int (*init)(struct controller *ctrl);
        ssize_t (*write)(struct controller *ctrl, const char *buf, size_t len);
        void (*destroy)(struct controller *ctrl);
        struct gpio_desc *hrdy_gpio;
};
#define to_controller(x) container_of(x, struct controller, kobj)
