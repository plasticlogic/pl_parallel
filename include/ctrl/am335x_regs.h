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

#ifndef AM335X_REGS_H
#define AM335X_REGS_H

#include <linux/kernel.h>
#include <asm-generic/io.h>
#include <linux/ioport.h>

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

union am335x_lcdc_pid_reg {
        struct  {
                uint32_t minor : 6;
                uint32_t custom : 2;
                uint32_t major : 3;
                uint32_t rtl : 5;
                uint32_t func : 12;
                uint32_t reserved : 2;
                uint32_t scheme : 2;
        };
        uint32_t reg_val;
};

static inline int am335x_lcdc_get_major_version(struct resource *hw_res)
{
        union am335x_lcdc_pid_reg reg;
        reg.reg_val = readl(hw_res->start);
        return reg.major;
}

static inline int am335x_lcdc_get_minor_version(struct resource *hw_res)
{
        union am335x_lcdc_pid_reg reg;
        reg.reg_val = readl(hw_res->start);
        return reg.minor;
}

enum lcd_mode {
        LIDD_MODE = 0,
        RASTER_MODE = 1,
};

union am335x_lcdc_ctrl_reg {
        struct {
                uint32_t modesel : 1;
                uint32_t auto_uflow_restart : 1;
                uint32_t reserved : 6;
                uint32_t clkdiv : 8;
                uint32_t reserved2 : 16;
        };
        uint32_t reg_val;
};

static inline void am335x_lcdc_set_ctrl_mode(struct resource *hw_res, 
                                             enum lcd_mode mode)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CTRL_OFFS);
        reg.modesel = mode;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_CTRL_OFFS);
}

static inline enum lcd_mode am335x_lcdc_get_ctrl_mode(struct resource *hw_res)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CTRL_OFFS);
        return reg.modesel;
}

static inline void am335x_lcdc_set_auto_uflow_restart(struct resource *hw_res,
                                                      unsigned int enable)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CTRL_OFFS);
        reg.auto_uflow_restart = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_CTRL_OFFS);
}

static inline int am335x_lcdc_get_auto_uflow_restart(struct resource *hw_res)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CTRL_OFFS);
        return reg.auto_uflow_restart;
}

static inline void am335x_lcdc_set_clkdiv(struct resource *hw_res, 
                                          unsigned int clk_div)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CTRL_OFFS);
        reg.clkdiv = clk_div;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_CTRL_OFFS);
}

static inline int am335x_lcdc_get_clkdiv(struct resource *hw_res)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CTRL_OFFS);
        return reg.clkdiv;
}

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

union am335x_lcdc_sysconfig_reg {
        struct {
                uint32_t reserved : 2;
                uint32_t idlemode : 2;
                uint32_t standbymode : 2;
                uint32_t reserved2 : 26;
        };
        uint32_t reg_val;
};

static inline void am335x_lcdc_set_standby_mode(struct resource *hw_res,
                                                enum standby_mode mode)
{
        union am335x_lcdc_sysconfig_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_SYSCONFIG_OFFS);
        reg.standbymode = mode;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_SYSCONFIG_OFFS);
}

static inline enum standby_mode am335x_lcdc_get_standby_mode(
                                                struct resource *hw_res)
{
        union am335x_lcdc_sysconfig_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_SYSCONFIG_OFFS);
        return reg.standbymode;
}

static inline void am335x_lcdc_set_idle_mode(struct resource *hw_res,
                                             enum idle_mode mode)
{
        union am335x_lcdc_sysconfig_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_SYSCONFIG_OFFS);
        reg.idlemode = mode;
        writel(hw_res->start + AM335X_LCDC_SYSCONFIG_OFFS);
}

static inline enum idle_mode am335x_lcdc_get_idle_mode(struct resource *hw_res)
{
        union am335x_lcdc_sysconfig_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_SYSCONFIG_OFFS);
        return reg.idlemode;
}

union am335x_lcdc_irqstatus_raw_reg {
        struct {
                uint32_t done_raw_set : 1;
                uint32_t recurrent_raster_done_raw_set : 1;
                uint32_t sync_raw_set : 1;
                uint32_t acb_raw_set : 1;
                uint32_t reserved : 1;
                uint32_t fuf_raw_set : 1;
                uint32_t pl_raw_set : 1;
                uint32_t reserved2 : 1;
                uint32_t eof0_raw_set : 1;
                uint32_t eof1_raw_set : 1;
                uint32_t reserved3 : 22;
        };
        uint32_t reg_val;
};

union am335x_lcdc_irqstatus_reg {
        struct {
                uint32_t done_en_clr : 1;
                uint32_t recurrent_raster_done_en_clr : 1;
                uint32_t sync_en_clr : 1;
                uint32_t acb_en_clr : 1;
                uint32_t reserved : 1;
                uint32_t fuf_en_clr : 1;
                uint32_t pl_en_clr : 1;
                uint32_t reserved2 : 1;
                uint32_t eof0_en_clr : 1;
                uint32_t eof0_en_clr : 1;
                uint32_t reserved3 : 22;
        };
        uint32_t reg_val;
};

union am335x_lcdc_irqenable_set_reg {
        struct {
                uint32_t done_en_set : 1;
                uint32_t recurrent_raster_done_en_set : 1;
                uint32_t sync_en_set : 1;
                uint32_t acb_en_set : 1;
                uint32_t reserved : 1;
                uint32_t fuf_en_set : 1;
                uint32_t pl_en_set : 1;
                uint32_t reserved2 : 1;
                uint32_t eof0_en_set : 1;
                uint32_t eof0_en_set : 1;
                uint32_t reserved3 : 22;
        };
        uint32_t reg_val;
};

union am335x_lcdc_irqenable_clear_reg {
        struct {
                uint32_t done_en_clr : 1;
                uint32_t recurrent_raster_done_en_clr : 1;
                uint32_t sync_en_clr : 1;
                uint32_t acb_en_clr : 1;
                uint32_t reserved : 1;
                uint32_t fuf_en_clr : 1;
                uint32_t pl_en_clr : 1;
                uint32_t reserved2 : 1;
                uint32_t eof0_en_clr : 1;
                uint32_t eof0_en_clr : 1;
                uint32_t reserved3 : 22;
        };
        uint32_t reg_val;
};

union am335x_lcdc_clkc_enable_reg {
        struct {
                uint32_t core_clk_en : 1;
                uint32_t lidd_clk_en : 1;
                uint32_t dma_clk_en : 1;
                uint32_t reserved : 29;
        };
        uint32_t reg_val;
};

static inline void am335x_lcdc_set_core_clk_en(struct resource *hw_res,
                                               unsigned int enable)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_ENABLE_OFFS);
        reg.core_clk_en = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_CLKC_ENABLE_OFFS);
}

static inline int am335x_lcdc_get_core_clk_en(struct resource *hw_res)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_ENABLE_OFFS);
        return reg.core_clk_en;
}

static inline void am335x_lcdc_set_lidd_clk_en(struct resource *hw_res,
                                               unsigned int enable)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_ENABLE_OFFS);
        reg.lidd_clk_en = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_CLKC_ENABLE_OFFS);
}

static inline int am335x_lcdc_get_lidd_clk_en(struct resource *hw_res)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_ENABLE_OFFS);
        return reg.lidd_clk_en;
}

static inline void am335x_lcdc_set_dma_clk_en(struct resource *hw_res,
                                              unsigned int enable)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_ENABLE_OFFS);
        reg.dma_clk_en = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_CLKC_ENABLE_OFFS);
}

static inline int am335x_lcdc_get_dma_clk_en(struct resource *hw_res)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_ENABLE_OFFS);
        return reg.dma_clk_en;
}

union am335x_lcdc_clkc_reset_reg {
        struct {
                uint32_t core_rst : 1;
                uint32_t lidd_rst : 1;
                uint32_t dma_rst : 1;
                uint32_t main_rst : 1;
                uint32_t reserved : 28;
        };
        uint32_t reg_val;
};

static inline void am335x_lcdc_set_main_clk_rst(struct resource *hw_res,
                                                unsigned int enable)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
        reg.main_rst = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
}

static inline int am335x_lcdc_get_main_clk_rst(struct resource *hw_res)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
        return reg.main_rst;
}

static inline void am335x_lcdc_set_core_clk_rst(struct resource *hw_res,
                                                unsigned int enable)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
        reg.core_rst = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
}

static inline int am335x_lcdc_get_core_clk_rst(struct resource *hw_res)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
        reg.core_rst;
}

static inline void am335x_lcdc_set_lidd_clk_rst(struct resource *hw_res,
                                                unsigned int enable)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
        reg.lidd_rst = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
}

static inline int am335x_lcdc_get_lidd_clk_rst(struct resource *hw_res)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
        return reg.lidd_rst;
}

static inline void am335x_lcdc_set_dma_clk_rst(struct resource *hw_res,
                                               unsigned int enable)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
        reg.dma_rst = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
}

static inline int am335x_lcdc_get_dma_clk_rst(struct resource *hw_res)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_CLKC_RESET_OFFS);
        return reg.dma_rst;
}

////////////////////////////////////////////////////////////////////////////////
// LIDD controller registers

struct am335x_lidd_timings {
        unsigned char w_setup;
        unsigned char w_strobe;
        unsigned char w_hold;
        unsigned char r_setup;
        unsigned char r_strobe;
        unsigned char r_hold;
        unsigned char ta;
};

struct am335x_lidd_sig_pol {
        sig_pol_t ale_pol : 1;
        sig_pol_t rs_en_pol : 1;
        sig_pol_t ws_dir_pol : 1;
        sig_pol_t cs0_e0_pol : 1;
        sig_pol_t cs1_e1_pol : 1;
};

enum lidd_mode {
        SYNC_MPU68 = 0,
        ASYNC_MPU68 = 1,
        SYNC_MPU80 = 2,
        ASYNC_MPU80 = 3,
        HITACHI = 4,
};

enum polarity {
        NO_INVERT = 0,
        INVERT = 1,
};

enum lidd_device {
        LIDD_CS0 = 0,
        LIDD_CS1 = 1,
};

union am335x_lcdc_lidd_ctrl_reg {
        struct {
                uint32_t lidd_mode_sel : 3;
                uint32_t ale_pol : 1;
                uint32_t rs_en_pol : 1;
                uint32_t ws_dir_pol : 1;
                uint32_t cs0_e0_pol : 1;
                uint32_t cs1_e1_pol : 1;
                uint32_t lidd_dma_en : 1;
                uint32_t dma_cs0_cs1 : 1;
                uint32_t reserved : 22;
        };
        uint32_t reg_val;
};

static inline void am335x_set_lidd_mode(struct resource *hw_res, 
                                        enum lidd_mode mode)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        reg.lidd_mode_sel = mode;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum lidd_mode am335x_get_lidd_mode(struct resource *hw_res)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        return reg.lidd_mode_sel;
}

static inline void am335x_set_ale_pol(struct resource *hw_res, 
                                      enum polarity pol)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        reg.ale_pol = pol;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum polarity am335x_get_ale_pol(struct resource *hw_res)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        return reg.ale_pol;
}

static inline void am335x_set_rs_en_pol(struct resource *hw_res, 
                                        enum polarity pol)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        reg.rs_en_pol = pol;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum polarity am335x_get_rs_en_pol(struct resource *hw_res)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        return reg.rs_en_pol;
}

static inline void am335x_set_ws_dir_pol(struct resource *hw_res, 
                                         enum polarity pol)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        reg.ws_dir_pol = pol;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum polarity am335x_get_ws_dir_pol(struct resource *hw_res)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        return reg.ws_dir_pol;
}

static inline void am335x_set_cs0_e0_pol(struct resource *hw_res, 
                                         enum polarity pol)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        reg.cs0_e0_pol = pol;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum polarity am335x_get_cs0_e0_pol(struct resource *hw_res)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        return reg.cs0_e0_pol;
}

static inline void am335x_set_cs1_e1_pol(struct resource *hw_res, 
                                         enum polarity pol)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        reg.cs1_e1_pol = pol;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum polarity am335x_get_cs1_e1_pol(struct resource *hw_res)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        return reg.cs1_e1_pol;
}

static inline void am335x_set_lidd_dma_en(struct resource *hw_res, 
                                          unsigned int enable)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        reg.ale_pol = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline int am335x_get_lidd_dma_en(struct resource *hw_res)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        return reg.ale_pol;
}

static inline void am335x_set_dma_cs0_cs1(struct resource *hw_res, 
                                          enum lidd_device ld)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        reg.dma_cs0_cs1 = ld;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum lidd_device am335x_get_dma_cs0_cs1(struct resource *hw_res)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        return reg.dma_cs0_cs1;
}

static inline void am335x_set_lidd_pols(struct resource *hw_res, 
                                        struct am335x_lidd_sig_pol *pols)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        reg.ale_pol = pols->ale_pol;
        reg.cs0_e0_pol = pols->cs0_e0_pol;
        reg.cs1_e1_pol = pols->cs1_e1_pol;
        reg.rs_en_pol = pols->rs_en_pol;
        reg.ws_dir_pol = pols->ws_dir_pol;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline struct am335x_lidd_sig_pol am335x_get_lidd_pols(
                                                struct resource *hw_res)
{
        union am335x_lcdc_lidd_ctrl_reg reg;
        struct am335x_lidd_sig_pol pols;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LIDD_CTRL_OFFS);
        pols.ale_pol = reg.ale_pol;
        pols.cs0_e0_pol = reg.cs0_e0_pol;
        pols.cs1_e1_pol = reg.cs1_e1_pol;
        pols.rs_en_pol = reg.rs_en_pol;
        pols.ws_dir_pol = reg.ws_dir_pol;
        return pols;
}

union am335x_lcdc_lidd_csx_conf_reg {
        struct {
                uint32_t ta : 2;
                uint32_t r_hold : 4;
                uint32_t r_strobe : 6;
                uint32_t r_su : 5;
                uint32_t w_hold : 4;
                uint32_t w_strobe : 6;
                uint32_t w_su : 5;
        };
        uint32_t reg_val;
};

#define get_lidd_csx_conf_offs(lidd_device)     \
        ld == LIDD_CS0 ?                        \
                AM335X_LCDC_LIDD_CS0_CONF_OFFS :\
                AM335X_LCDC_LIDD_CS1_CONF_OFFS

#define get_lidd_csx_addr_offs(lidd_device)     \
        ld == LIDD_CS0 ?                        \
                AM335X_LCDC_LIDD_CS0_ADDR_OFFS :\
                AM335X_LCDC_LIDD_CS1_ADDR_OFFS

#define get_lidd_csx_data_offs(lidd_device)     \
        ld == LIDD_CS0 ?                        \
                AM335X_LCDC_LIDD_CS0_DATA_OFFS :\
                AM335X_LCDC_LIDD_CS1_DATA_OFFS

static inline void am335x_set_lidd_ta(struct resource *hw_res, 
                                      enum lidd_device ld, int ta)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        reg.ta = ta;
        writel(reg.reg_val, hw_res->start + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_ta(struct resource *hw_res, 
                                     enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        return reg.ta;
}

static inline void am335x_set_lidd_r_hold(struct resource *hw_res, 
                                          enum lidd_device ld, int r_hold)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        reg.r_hold = r_hold;
        writel(reg.reg_val, hw_res->start + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_r_hold(struct resource *hw_res, 
                                         enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        return reg.r_hold;
}

static inline void am335x_set_lidd_r_strobe(struct resource *hw_res, 
                                            enum lidd_device ld, int r_strobe)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        reg.r_strobe = r_strobe;
        writel(reg.reg_val, hw_res->start + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_r_strobe(struct resource *hw_res, 
                                            enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        return reg.r_strobe;
}

static inline void am335x_set_lidd_r_su(struct resource *hw_res, 
                                        enum lidd_device ld, int r_su)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        reg.r_su = r_su;
        writel(reg.reg_val, hw_res->start + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_r_su(struct resource *hw_res, 
                                       enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        return reg.r_su;
}

static inline void am335x_set_lidd_w_hold(struct resource *hw_res, 
                                          enum lidd_device ld, int w_hold)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        reg.w_hold = w_hold;
        writel(reg.reg_val, hw_res->start + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_w_hold(struct resource *hw_res, 
                                          enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        return reg.w_hold;
}

static inline void am335x_set_lidd_w_strobe(struct resource *hw_res, 
                                            enum lidd_device ld, int w_strobe)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        reg.w_strobe = w_strobe;
        writel(reg.reg_val, hw_res->start + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_w_strobe(struct resource *hw_res, 
                                           enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        return reg.w_strobe;
}

static inline void am335x_set_lidd_w_su(struct resource *hw_res, 
                                        enum lidd_device ld, int w_su)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        reg.w_su = w_su;
        writel(reg.reg_val, hw_res->start + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_w_su(struct resource *hw_res, 
                                        enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        return reg.w_su;
}

static inline void am335x_set_lidd_timings(struct resource *hw_res,
                                           enum lidd_device ld,
                                           struct am335x_lidd_timings *lt)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        reg.r_hold = lt->r_hold;
        reg.r_strobe = lt->r_strobe;
        reg.r_su = lt->r_setup;
        reg.w_hold = lt->w_hold;
        reg.w_strobe = lt->w_strobe;
        reg.w_su = lt->w_setup;
        reg.ta = lt->ta;
        writel(reg.reg_val, hw_res->start + get_lidd_csx_conf_offs(ld));
}

static inline struct am335x_lidd_timings am335x_get_lidd_timings(
                                                struct resource *hw_res,
                                                enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        struct am335x_lidd_timings lt;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_conf_offs(ld));
        lt->r_hold = reg.r_hold;
        lt->r_strobe = reg.r_strobe;
        lt->r_setup = reg.r_su;
        lt->w_hold = reg.w_hold;
        lt->w_strobe = reg.w_strobe;
        lt->w_setup = reg.w_su;
        lt->ta 0 reg.ta;
        return lt;
}

union am335x_lcdc_lidd_csx_addr_reg {
        struct {
                uint32_t adr_indx : 16;
                uint32_t reserved : 16;
        };
        uint32_t reg_val;
};

static inline void am335x_set_lidd_addr(struct resource *hw_res,
                                        enum lidd_device ld, int addr)
{
        union am335x_lcdc_lidd_csx_addr_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_addr_offs(ld));
        reg.adr_indx = addr;
        writel(reg.reg_val, hw_res->start + get_lidd_csx_addr_offs(ld));        
}

union am335x_lcdc_lidd_csx_data_reg {
        struct {
                uint32_t data : 16;
                uint32_t reserved : 16;
        };
        uint32_t reg_val;
};

static inline void am335x_set_lidd_data(struct resource *hw_res,
                                        enum lidd_device ld,
                                        const void *data)
{
        union am335x_lcdc_lidd_csx_data_reg reg;
        reg.reg_val = readl(hw_res->start + get_lidd_csx_data_offs(ld));
        reg.data = data;
        writel(reg.reg_val, hw_res->start + get_lidd_csx_data_offs(ld));
}

////////////////////////////////////////////////////////////////////////////////
// LCDDMA registers

enum dma_framebuffer {
        FB0 = 0,
        FB1 = 1,
};

enum dma_master_prio {
        HIGH_PRIO = 0,
        LOW_PRIO = 7,
};

enum dma_fifo_threshold {
        FIFO_TH_8 = 0,
        FIFO_TH_16 = 1,
        FIFO_TH_32 = 2,
        FIFO_TH_64 = 3,
        FIFO_TH_128 = 4,
        FIFO_TH_256 = 5,
        FIFO_TH_512 = 6,
};

enum dma_burst_size {
        BURST_SIZE_1 = 0,
        BURST_SIZE_2 = 1,
        BURST_SIZE_4 = 2,
        BURST_SIZE_8 = 3,
        BURST_SIZE_16 = 4,
};

enum dma_frame_mode {
        ONE_FRAME = 0,
        DUAL_FRAME = 1,
};

union am335x_lcdc_lcddma_ctrl_reg {
        struct {
                uint32_t frame_mode : 1;
                uint32_t bigendian : 1;
                uint32_t reserved : 1;
                uint32_t byte_swap : 1;
                uint32_t burst_size : 3;
                uint32_t reserved2 : 1;
                uint32_t th_fifo_ready : 3;
                uint32_t reserved3 : 5;
                uint32_t dma_master_prio : 3;
                uint32_t reserved4 : 13;
        };
        uint32_t reg_val;
};

static inline void am335x_set_lcddma_frame_mode(struct resource *hw_res,
                                                enum dma_frame_mode mode)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.frame_mode = mode;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

static inline void am335x_set_lcddma_big_endian_en(struct resource *hw_res,
                                                   unsigned int enable)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.bigendian = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

static inline void am335x_set_lcddma_byte_swap_en(struct resource *hw_res,
                                                  unsigned int enable)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.byte_swap = enable;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

static inline void am335x_set_lcddma_burst_size(struct resource *hw_res,
                                                enum dma_burst_size bs)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.burst_size = bs;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

static inline void am335x_set_lcddma_fifo_threshold(struct resource *hw_res,
                                                    enum dma_fifo_threshold ft)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.th_fifo_ready = ft;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

static inline void am335x_set_lcddma_master_prio(struct resource *hw_res,
                                                 enum dma_master_prio mp)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.dma_master_prio = mp;
        writel(reg.reg_val, hw_res->start + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

#define get_lcddma_fbx_base_offs(fb)                    \
        fb == FB0 ? AM335X_LCDC_LCDDMA_FB0_BASE_OFFS :  \
        AM335X_LCDC_LCDDMA_FB1_BASE_OFFS

#define get_lcddma_fbx_ceil_offs(fb)                    \
        fb == FB0 ? AM335X_LCDC_LCDDMA_FB0_CEIL_OFFS :  \
        AM335X_LCDC_LCDDMA_FB1_CEIL_OFFS

union am335x_lcdc_lcddma_fbx_base_reg {
        struct {
                uint32_t reserved : 2;
                uint32_t fb_base : 30;
        };
        uint32_t reg_val;
};

static inline void am335x_set_lcddma_fbx_base_addr(struct resource *hw_res,
                                                   enum dma_framebuffer fb,
                                                   const void *base_addr)
{
        union am335x_lcdc_lcddma_fbx_base_reg reg;
        reg.reg_val = readl(hw_res->start + get_lcddma_fbx_base_offs(fb));
        reg.fb_base = base_addr >> 2;
        writel(reg.reg_val, hw_res->start + get_lcddma_fbx_base_offs(fb));
}

union am335x_lcdc_lcddma_fbx_ceiling_reg {
        struct {
                uint32_t reserved : 2;
                uint32_t fb_ceil : 30;
        };
        uint32_t reg_val;
};

static inline void am335x_set_lcddma_fbx_ceil_addr(struct resource *hw_res,
                                                   enum dma_framebuffer fb,
                                                   const void *ceil_addr)
{
        union am335x_lcdc_lcddma_fbx_ceiling_reg reg;
        reg.reg_val = readl(hw_res->start + get_lcddma_fbx_base_offs(fb));
        reg.fb_ceil = ceil_addr >> 2;
        writel(reg.reg_val, hw_res->start + get_lcddma_fbx_base_offs(fb));
}

#endif /* AM335X_REGS_H */