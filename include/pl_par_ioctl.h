// SPDX-License-Identifier: GPL-3.0
/*
 * pl_par_ioctl.h - controller implementations for am335x based systems
 *
 * Copyright (c) 2021 PL Germany
 * 
 * Authors 
 *      Lars Görner <lars.goerner@plasticlogic.com>
 *
 */

#ifndef PL_PAR_IOCTL_H
#define PL_PAR_IOCTL_H

#define V1_0

enum pl_par_rw {
        PL_PAR_READ = 0,
        PL_PAR_WRITE = 1,
};

struct pl_par_ioctl_message;

#ifdef V1_0

struct pl_par_ioctl_message {
        unsigned short adr;
        short *data;
        unsigned int data_size;
        enum pl_par_rw rd_wr;
        unsigned int disable_adr_send : 1;
        unsigned int disable_data_send : 1;
};

#endif /* V1_0 */

#endif /* PL_PAR_IOCTL_H */