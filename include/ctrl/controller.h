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

struct controller {
        struct kobject kobj;
        int (*init)(struct controller *ctrl);
        ssize_t (*write)(struct controller *ctrl, const char *buf, size_t len);
        void (*destroy)(struct controller *ctrl);
};
