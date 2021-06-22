// SPDX-License-Identifier: GPL-3.0
/*
 * am335x_regs.h - register structures for am335x parallel HW (LCDC)
 *
 * Copyright (c) 2001-2003 PL Germany
 * 
 * Authors 
 *      Lars Görner <lars.goerner@plasticlogic.com>
 *
 * See the Am335x technical reference manual for mode info
 */

#define AM335X_LCDC_PID_OFFS                    0x00U
#define AM335X_LCDC_CTRL_OFFS                   0x04U
#define AM335X_LCDC_LIDD_CTRL_OFFS              0x0CU
#define AM335X_LCDC_LIDD_CS0_CONF_OFFS          0x10U
#define AM335X_LCDC_LIDD_CS0_ADDR_OFFS          0x14U
#define AM335X_LCDC_LIDD_CS0_DATA_OFFS          0x18U
#define AM335X_LCDC_LIDD_CS1_CONF_OFFS          0x1CU
#define AM335X_LCDC_LIDD_CS1_ADDR_OFFS          0x20U
#define AM335X_LCDC_LIDD_CS1_DATA_OFFS          0x24U
#define AM335X_LCDC_LCDDMA_CTRL_OFFS            0x40U
#define AM335X_LCDC_LCDDMA_FB0_BASE_OFFS        0x44U
#define AM335X_LCDC_LCDDMA_FB0_CEIL_OFFS        0x48U
#define AM335X_LCDC_LCDDMA_FB1_BASE_OFFS        0x4CU
#define AM335X_LCDC_LCDDMA_FB1_CEIL_OFFS        0x50U
#define AM335X_LCDC_SYSCONFIG_OFFS              0x54U
#define AM335X_LCDC_IRQSTATUS_RAW_OFFS          0x58U
#define AM335X_LCDC_IRQSTATUS_OFFS              0x5CU
#define AM335X_LCDC_IRQENABLE_SET_OFFS          0x60U
#define AM335X_LCDC_IRQENABLE_CLEAR_OFFS        0x64U
#define AM335X_LCDC_CLKC_ENABLE_OFFS            0x6CU
#define AM335X_LCDC_CLKC_RESET_OFFS             0x70U

////////////////////////////////////////////////////////////////////////////////
// General LCDC registers

struct am335x_lcdc_pid_reg {
        unsigned int minor : 6;
        unsigned int custom : 2;
        unsigned int major : 3;
        unsigned int rtl : 5;
        unsigned int func : 12;
        unsigned int reserved : 2;
        unsigned int scheme : 2;
};

enum lcd_mode {
        LIDD_MODE = 0,
        RASTER_MODE = 1,
};

struct am335x_lcdc_ctrl_reg {
        unsigned int modesel : 1;
        unsigned int auto_uflow_restart : 1;
        unsigned int reserved : 6;
        unsigned int clkdiv : 8;
        unsigned int reserved2 : 16;
};

enum standby_mode {
        FORCE_STANDBY = 0,
        MO_STANDBY = 1,
        SMART_STANDBY = 2,
};

enum idle_mode {
        FORCE_IDLE = 0,
        NO_IDLE = 1,
        SMART_IDLE = 2,
};

struct am335x_lcdc_sysconfig_reg {
        unsigned int reserved : 2;
        unsigned int idlemode : 2;
        unsigned int standbymode : 2;
        unsigned int reserved2 : 26;
};

struct am335x_lcdc_irqstatus_raw_reg {
        unsigned int donw_raw_set : 1;
        unsigned int recurrent_raster_done_raw_set : 1;
        unsigned int sync_raw_set : 1;
        unsigned int acb_raw_set : 1;
        unsigned int reserved : 1;
        unsigned int fuf_raw_set : 1;
        unsigned int pl_raw_set : 1;
        unsigned int reserved2 : 1;
        unsigned int eof0_raw_set : 1;
        unsigned int eof1_raw_set : 1;
        unsigned int reserved3 : 22;
};

struct am335x_lcdc_irqstatus_reg {
        unsigned int done_en_clr : 1;
        unsigned int recurrent_raster_done_en_clr : 1;
        unsigned int sync_en_clr : 1;
        unsigned int acb_en_clr : 1;
        unsigned int reserved : 1;
        unsigned int fuf_en_clr : 1;
        unsigned int pl_en_clr : 1;
        unsigned int reserved2 : 1;
        unsigned int eof0_en_clr : 1;
        unsigned int eof0_en_clr : 1;
        unsigned int reserved3 : 22;
};

struct am335x_lcdc_irqenable_set_reg {
        unsigned int done_en_set : 1;
        unsigned int recurrent_raster_done_en_set : 1;
        unsigned int sync_en_set : 1;
        unsigned int acb_en_set : 1;
        unsigned int reserved : 1;
        unsigned int fuf_en_set : 1;
        unsigned int pl_en_set : 1;
        unsigned int reserved2 : 1;
        unsigned int eof0_en_set : 1;
        unsigned int eof0_en_set : 1;
        unsigned int reserved3 : 22;
};

struct am335x_lcdc_irqenable_clear_reg {
        unsigned int done_en_clr : 1;
        unsigned int recurrent_raster_done_en_clr : 1;
        unsigned int sync_en_clr : 1;
        unsigned int acb_en_clr : 1;
        unsigned int reserved : 1;
        unsigned int fuf_en_clr : 1;
        unsigned int pl_en_clr : 1;
        unsigned int reserved2 : 1;
        unsigned int eof0_en_clr : 1;
        unsigned int eof0_en_clr : 1;
        unsigned int reserved3 : 22;
};

struct am335x_lcdc_clkc_enable {
        unsigned int core_clk_en : 1;
        unsigned int lidd_clk_en : 1;
        unsigned int dma_clk_en : 1;
        unsigned int reserved : 29;
};

struct am335x_lcdc_clkc_reset {
        unsigned int core_rst : 1;
        unsigned int lidd_rst : 1;
        unsigned int dma_rst : 1;
        unsigned int main_rst : 1;
        unsigned int reserved : 28;
};

////////////////////////////////////////////////////////////////////////////////
// LIDD controller registers

enum lidd_mode {
        SYNC_MPU68 = 0,
        ASYNC_MPU68 = 1,
        SYNC_MPU80 = 2,
        ASYNC_MPU80 = 3,
        HITACHI = 4,
};

struct am335x_lcdc_lidd_ctrl_reg {
        unsigned int lidd_mode_sel : 3;
        unsigned int alepol : 1;
        unsigned int rs_en_pol : 1;
        unsigned int ws_dir_pol : 1;
        unsigned int cs0_e0_pol : 1;
        unsigned int cs1_e1_pol : 1;
        unsigned int lidd_dma_en : 1;
        unsigned int dma_cs0_cs1 : 1;
        unsigned int reserved : 22;
};

struct am335x_lcdc_lidd_csx_conf_reg {
        unsigned int ta : 2;
        unsigned int r_hold : 4;
        unsigned int r_strobe : 6;
        unsigned int r_su : 5;
        unsigned int w_hold : 4;
        unsigned int w_strobe : 6;
        unsigned int w_su : 5;
};

struct am335x_lcdc_lidd_csx_addr_reg {
        unsigned int adr_indx : 16;
        unsigned int reserved : 16;
};

struct am335x_lcdc_lidd_csx_data_reg {
        unsigned int data : 16;
        unsigned int reserved : 16;
};

////////////////////////////////////////////////////////////////////////////////
// LCDDMA registers

struct am335x_lcdc_lcddma_ctrl_reg {
        unsigned int frame_mode : 1;
        unsigned int bigendian : 1;
        unsigned int reserved : 1;
        unsigned int byte_swap : 1;
        unsigned int burst_size : 3;
        unsigned int reserved2 : 1;
        unsigned int th_fifo_ready : 3;
        unsigned int reserved3 : 5;
        unsigned int dma_master_prio : 3;
        unsigned int reserved4 : 13;
};

struct am335x_lcdc_lcddma_fbx_base_reg {
        unsigned int reserved : 2;
        unsigned int fb_base : 30;
};

struct am335x_lcdc_lcddma_fbx_ceiling_reg {
        unsigned int reserved : 2;
        unsigned int fb_ceil : 30;
};