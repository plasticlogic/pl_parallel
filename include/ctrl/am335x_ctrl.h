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

#include <controller.h>

struct am335x_ctrl_timings {
        unsigned char w_setup;
        unsigned char w_strobe;
        unsigned char w_hold;
        unsigned char r_setup;
        unsigned char r_strobe;
        unsigned char r_hold;
        unsigned char ta;
}

struct am335x_ctrl_sig_pol {
        unsigned int ale_pol : 1;
        unsigned int rs_en_pol : 1;
        unsigned int ws_dir_pol : 1;
        unsigned int cs0_e0_pol : 1;
        unsigned int cs1_e1_pol : 1;
}

struct am335x_ctrl {
        struct controller ctrl;
        struct am335x_ctrl_timings timings;
        struct am335x_ctrl_sig_pol sig_pol;
};