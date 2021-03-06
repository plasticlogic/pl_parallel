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

#define SET_BIT(val, n) ((val) |= 1U << (n))
#define CLEAR_BIT(val, n) ((val) &= ~(1U << (n)))

#define WRITE_REG_BIT(reg, b, n) \
        ((b) ? SET_BIT((reg), (n)) : CLEAR_BIT((reg), (n)))

#define READ_REG_BIT(reg, n) (((reg) >> n) & 1U)

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

static inline int am335x_lcdc_get_major_version(void __iomem *base_addr)
{
        union am335x_lcdc_pid_reg reg;
        reg.reg_val = readl(base_addr);
        return reg.major;
}

static inline int am335x_lcdc_get_minor_version(void __iomem *base_addr)
{
        union am335x_lcdc_pid_reg reg;
        reg.reg_val = readl(base_addr);
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

static inline void am335x_lcdc_set_ctrl_mode(void __iomem *base_addr, 
                                             enum lcd_mode mode)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CTRL_OFFS);
        reg.modesel = mode;
        writel(reg.reg_val, base_addr + AM335X_LCDC_CTRL_OFFS);
}

static inline enum lcd_mode am335x_lcdc_get_ctrl_mode(void __iomem *base_addr)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CTRL_OFFS);
        return reg.modesel;
}

static inline void am335x_lcdc_set_auto_uflow_restart(void __iomem *base_addr,
                                                      unsigned int enable)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CTRL_OFFS);
        reg.auto_uflow_restart = enable;
        writel(reg.reg_val, base_addr + AM335X_LCDC_CTRL_OFFS);
}

static inline int am335x_lcdc_get_auto_uflow_restart(void __iomem *base_addr)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CTRL_OFFS);
        return reg.auto_uflow_restart;
}

static inline void am335x_lcdc_set_clkdiv(void __iomem *base_addr, 
                                          unsigned int clk_div)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CTRL_OFFS);
        reg.clkdiv = clk_div;
        writel(reg.reg_val, base_addr + AM335X_LCDC_CTRL_OFFS);
}

static inline int am335x_lcdc_get_clkdiv(void __iomem *base_addr)
{
        union am335x_lcdc_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CTRL_OFFS);
        return reg.clkdiv;
}

enum standby_mode {
        FORCE_STANDBY = 0,
        NO_STANDBY = 1,
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

static inline void am335x_lcdc_set_standby_mode(void __iomem *base_addr,
                                                enum standby_mode mode)
{
        union am335x_lcdc_sysconfig_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_SYSCONFIG_OFFS);
        reg.standbymode = mode;
        writel(reg.reg_val, base_addr + AM335X_LCDC_SYSCONFIG_OFFS);
}

static inline enum standby_mode am335x_lcdc_get_standby_mode(
                                                void __iomem *base_addr)
{
        union am335x_lcdc_sysconfig_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_SYSCONFIG_OFFS);
        return reg.standbymode;
}

static inline void am335x_lcdc_set_idle_mode(void __iomem *base_addr,
                                             enum idle_mode mode)
{
        union am335x_lcdc_sysconfig_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_SYSCONFIG_OFFS);
        reg.idlemode = mode;
        writel(reg.reg_val, base_addr + AM335X_LCDC_SYSCONFIG_OFFS);
}

static inline enum idle_mode am335x_lcdc_get_idle_mode(void __iomem *base_addr)
{
        union am335x_lcdc_sysconfig_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_SYSCONFIG_OFFS);
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
                uint32_t eof1_en_clr : 1;
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
                uint32_t eof1_en_set : 1;
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
                uint32_t eof1_en_clr : 1;
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

static inline void am335x_lcdc_set_core_clk_en(void __iomem *base_addr,
                                               unsigned int enable)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_ENABLE_OFFS);
        reg.core_clk_en = enable;
        writel(reg.reg_val, base_addr + AM335X_LCDC_CLKC_ENABLE_OFFS);
}

static inline int am335x_lcdc_get_core_clk_en(void __iomem *base_addr)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_ENABLE_OFFS);
        return reg.core_clk_en;
}

static inline void am335x_lcdc_set_lidd_clk_en(void __iomem *base_addr,
                                               unsigned int enable)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_ENABLE_OFFS);
        reg.lidd_clk_en = enable;
        writel(reg.reg_val, base_addr + AM335X_LCDC_CLKC_ENABLE_OFFS);
}

static inline int am335x_lcdc_get_lidd_clk_en(void __iomem *base_addr)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_ENABLE_OFFS);
        return reg.lidd_clk_en;
}

static inline void am335x_lcdc_set_dma_clk_en(void __iomem *base_addr,
                                              unsigned int enable)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_ENABLE_OFFS);
        reg.dma_clk_en = enable;
        writel(reg.reg_val, base_addr + AM335X_LCDC_CLKC_ENABLE_OFFS);
}

static inline int am335x_lcdc_get_dma_clk_en(void __iomem *base_addr)
{
        union am335x_lcdc_clkc_enable_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_ENABLE_OFFS);
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

static inline void am335x_lcdc_set_main_clk_rst(void __iomem *base_addr,
                                                unsigned int enable)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
        reg.main_rst = enable;
        writel(reg.reg_val, base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
}

static inline int am335x_lcdc_get_main_clk_rst(void __iomem *base_addr)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
        return reg.main_rst;
}

static inline void am335x_lcdc_set_core_clk_rst(void __iomem *base_addr,
                                                unsigned int enable)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
        reg.core_rst = enable;
        writel(reg.reg_val, base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
}

static inline int am335x_lcdc_get_core_clk_rst(void __iomem *base_addr)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
        return reg.core_rst;
}

static inline void am335x_lcdc_set_lidd_clk_rst(void __iomem *base_addr,
                                                unsigned int enable)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
        reg.lidd_rst = enable;
        writel(reg.reg_val, base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
}

static inline int am335x_lcdc_get_lidd_clk_rst(void __iomem *base_addr)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
        return reg.lidd_rst;
}

static inline void am335x_lcdc_set_dma_clk_rst(void __iomem *base_addr,
                                               unsigned int enable)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
        reg.dma_rst = enable;
        writel(reg.reg_val, base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
}

static inline int am335x_lcdc_get_dma_clk_rst(void __iomem *base_addr)
{
        union am335x_lcdc_clkc_reset_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_CLKC_RESET_OFFS);
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

struct am335x_lidd_sig_pol {
        enum polarity ale_pol;
        enum polarity rs_en_pol;
        enum polarity ws_dir_pol;
        enum polarity cs0_e0_pol;
        enum polarity cs1_e1_pol;
};

enum lidd_device {
        LIDD_CS0 = 0,
        LIDD_CS1 = 1,
};

#define AM335X_LIDD_MODE_SEL_OFFS       0
#define AM335X_ALEPOL_OFFS              3
#define AM335X_RS_EN_POL_OFFS           4
#define AM335X_WS_DIR_POL_OFFS          5
#define AM335X_CS0_E0_POL_OFFS          6
#define AM335X_CS1_E1_POL_OFFS          7
#define AM335X_LIDD_DMA_EN_OFFS         8
#define AM335X_DMA_CS0_CS1_OFFS         9

static inline void am335x_set_lidd_mode(void __iomem *base_addr, 
                                        enum lidd_mode mode)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        reg &= ~(3 << AM335X_LIDD_MODE_SEL_OFFS);
        reg |= (mode << AM335X_LIDD_MODE_SEL_OFFS);
        writel(reg, base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum lidd_mode am335x_get_lidd_mode(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        return (reg & 3U);
}

static inline void am335x_set_ale_pol(void __iomem *base_addr, 
                                      enum polarity pol)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        WRITE_REG_BIT(reg, pol, AM335X_ALEPOL_OFFS);
        writel(reg, base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum polarity am335x_get_ale_pol(void __iomem *base_addr)
{
        int reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        return READ_REG_BIT(reg, AM335X_ALEPOL_OFFS);
}

static inline void am335x_set_rs_en_pol(void __iomem *base_addr, 
                                        enum polarity pol)
{
        int reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        WRITE_REG_BIT(reg, pol, AM335X_RS_EN_POL_OFFS);
        writel(reg, base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum polarity am335x_get_rs_en_pol(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        return READ_REG_BIT(reg, AM335X_RS_EN_POL_OFFS);
}

static inline void am335x_set_ws_dir_pol(void __iomem *base_addr, 
                                         enum polarity pol)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        WRITE_REG_BIT(reg, pol, AM335X_WS_DIR_POL_OFFS);
        writel(reg, base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum polarity am335x_get_ws_dir_pol(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        return READ_REG_BIT(reg, AM335X_WS_DIR_POL_OFFS);
}

static inline void am335x_set_cs0_e0_pol(void __iomem *base_addr, 
                                         enum polarity pol)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        WRITE_REG_BIT(reg, pol, AM335X_CS0_E0_POL_OFFS);
        writel(reg, base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum polarity am335x_get_cs0_e0_pol(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        return READ_REG_BIT(reg, AM335X_CS0_E0_POL_OFFS);
}

static inline void am335x_set_cs1_e1_pol(void __iomem *base_addr, 
                                         enum polarity pol)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        WRITE_REG_BIT(reg, pol, AM335X_CS1_E1_POL_OFFS);
        writel(reg, base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum polarity am335x_get_cs1_e1_pol(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        return READ_REG_BIT(reg, AM335X_CS1_E1_POL_OFFS);
}

static inline void am335x_set_lidd_dma_en(void __iomem *base_addr, 
                                          unsigned enable)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        WRITE_REG_BIT(reg, enable, AM335X_LIDD_DMA_EN_OFFS);
        writel(reg, base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline int am335x_get_lidd_dma_en(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        return READ_REG_BIT(reg, AM335X_LIDD_DMA_EN_OFFS);
}

static inline void am335x_set_dma_cs0_cs1(void __iomem *base_addr, 
                                          enum lidd_device ld)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        WRITE_REG_BIT(reg, ld, AM335X_DMA_CS0_CS1_OFFS);
        writel(reg, base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline enum lidd_device am335x_get_dma_cs0_cs1(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        return READ_REG_BIT(reg, AM335X_DMA_CS0_CS1_OFFS);
}

static inline void am335x_set_lidd_pols(void __iomem *base_addr, 
                                        struct am335x_lidd_sig_pol *pols)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        WRITE_REG_BIT(reg, pols->ale_pol, AM335X_ALEPOL_OFFS);
        WRITE_REG_BIT(reg, pols->cs0_e0_pol, AM335X_CS0_E0_POL_OFFS);
        WRITE_REG_BIT(reg, pols->cs1_e1_pol, AM335X_CS1_E1_POL_OFFS);
        WRITE_REG_BIT(reg, pols->rs_en_pol, AM335X_RS_EN_POL_OFFS);
        WRITE_REG_BIT(reg, pols->ws_dir_pol, AM335X_WS_DIR_POL_OFFS);
        writel(reg, base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
}

static inline struct am335x_lidd_sig_pol am335x_get_lidd_pols(
                                                void __iomem *base_addr)
{
        struct am335x_lidd_sig_pol pols;
        unsigned reg = readl(base_addr + AM335X_LCDC_LIDD_CTRL_OFFS);
        pols.ale_pol = READ_REG_BIT(reg, AM335X_ALEPOL_OFFS);
        pols.cs0_e0_pol = READ_REG_BIT(reg, AM335X_CS0_E0_POL_OFFS);
        pols.cs1_e1_pol = READ_REG_BIT(reg, AM335X_CS1_E1_POL_OFFS);
        pols.rs_en_pol = READ_REG_BIT(reg, AM335X_RS_EN_POL_OFFS);
        pols.ws_dir_pol = READ_REG_BIT(reg, AM335X_WS_DIR_POL_OFFS);
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
        (ld == LIDD_CS0 ?                       \
        AM335X_LCDC_LIDD_CS0_CONF_OFFS :        \
        AM335X_LCDC_LIDD_CS1_CONF_OFFS)

#define get_lidd_csx_addr_offs(lidd_device)     \
        (ld == LIDD_CS0 ?                       \
        AM335X_LCDC_LIDD_CS0_ADDR_OFFS :        \
        AM335X_LCDC_LIDD_CS1_ADDR_OFFS)

#define get_lidd_csx_data_offs(lidd_device)     \
        (ld == LIDD_CS0 ?                       \
        AM335X_LCDC_LIDD_CS0_DATA_OFFS :        \
        AM335X_LCDC_LIDD_CS1_DATA_OFFS)

static inline void am335x_set_lidd_ta(void __iomem *base_addr, 
                                      enum lidd_device ld, int ta)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        reg.ta = ta;
        writel(reg.reg_val, base_addr + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_ta(void __iomem *base_addr, 
                                     enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        return reg.ta;
}

static inline void am335x_set_lidd_r_hold(void __iomem *base_addr, 
                                          enum lidd_device ld, int r_hold)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        reg.r_hold = r_hold;
        writel(reg.reg_val, base_addr + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_r_hold(void __iomem *base_addr, 
                                         enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        return reg.r_hold;
}

static inline void am335x_set_lidd_r_strobe(void __iomem *base_addr, 
                                            enum lidd_device ld, int r_strobe)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        reg.r_strobe = r_strobe;
        writel(reg.reg_val, base_addr + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_r_strobe(void __iomem *base_addr, 
                                            enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        return reg.r_strobe;
}

static inline void am335x_set_lidd_r_su(void __iomem *base_addr, 
                                        enum lidd_device ld, int r_su)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        reg.r_su = r_su;
        writel(reg.reg_val, base_addr + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_r_su(void __iomem *base_addr, 
                                       enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        return reg.r_su;
}

static inline void am335x_set_lidd_w_hold(void __iomem *base_addr, 
                                          enum lidd_device ld, int w_hold)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        reg.w_hold = w_hold;
        writel(reg.reg_val, base_addr + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_w_hold(void __iomem *base_addr, 
                                          enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        return reg.w_hold;
}

static inline void am335x_set_lidd_w_strobe(void __iomem *base_addr, 
                                            enum lidd_device ld, int w_strobe)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        reg.w_strobe = w_strobe;
        writel(reg.reg_val, base_addr + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_w_strobe(void __iomem *base_addr, 
                                           enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        return reg.w_strobe;
}

static inline void am335x_set_lidd_w_su(void __iomem *base_addr, 
                                        enum lidd_device ld, int w_su)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        reg.w_su = w_su;
        writel(reg.reg_val, base_addr + get_lidd_csx_conf_offs(ld));
}

static inline int am335x_get_lidd_w_su(void __iomem *base_addr, 
                                        enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        return reg.w_su;
}

static inline void am335x_set_lidd_timings(void __iomem *base_addr,
                                           enum lidd_device ld,
                                           struct am335x_lidd_timings *lt)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        reg.r_hold = lt->r_hold;
        reg.r_strobe = lt->r_strobe;
        reg.r_su = lt->r_setup;
        reg.w_hold = lt->w_hold;
        reg.w_strobe = lt->w_strobe;
        reg.w_su = lt->w_setup;
        reg.ta = lt->ta;
        writel(reg.reg_val, base_addr + get_lidd_csx_conf_offs(ld));
}

static inline struct am335x_lidd_timings am335x_get_lidd_timings(
                                                void __iomem *base_addr,
                                                enum lidd_device ld)
{
        union am335x_lcdc_lidd_csx_conf_reg reg;
        struct am335x_lidd_timings lt;
        reg.reg_val = readl(base_addr + get_lidd_csx_conf_offs(ld));
        lt.r_hold = reg.r_hold;
        lt.r_strobe = reg.r_strobe;
        lt.r_setup = reg.r_su;
        lt.w_hold = reg.w_hold;
        lt.w_strobe = reg.w_strobe;
        lt.w_setup = reg.w_su;
        lt.ta = reg.ta;
        return lt;
}

static inline void am335x_set_lidd_addr(void __iomem *base_addr,
                                        enum lidd_device ld, int addr)
{
        iowrite16(addr, base_addr + get_lidd_csx_addr_offs(ld));        
}

static inline short am335x_get_lidd_addr(void __iomem *base_addr, 
                                         enum lidd_device ld)
{
        return ioread16(base_addr + get_lidd_csx_addr_offs(ld));
}

static inline void am335x_set_lidd_data(void __iomem *base_addr,
                                        enum lidd_device ld, short data)
{
        iowrite16(data, base_addr + get_lidd_csx_data_offs(ld));
}

static inline short am335x_get_lidd_data(void __iomem *base_addr,
                                         enum lidd_device ld)
{
        return ioread16(base_addr + get_lidd_csx_data_offs(ld));
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

static inline void am335x_set_lcddma_frame_mode(void __iomem *base_addr,
                                                enum dma_frame_mode mode)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.frame_mode = mode;
        writel(reg.reg_val, base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

static inline void am335x_set_lcddma_big_endian_en(void __iomem *base_addr,
                                                   unsigned int enable)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.bigendian = enable;
        writel(reg.reg_val, base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

static inline void am335x_set_lcddma_byte_swap_en(void __iomem *base_addr,
                                                  unsigned int enable)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.byte_swap = enable;
        writel(reg.reg_val, base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

static inline void am335x_set_lcddma_burst_size(void __iomem *base_addr,
                                                enum dma_burst_size bs)
{
        uint32_t reg = readl(base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg &= ~(3 << 4);
        reg |= (bs << 4);
        writel(reg, base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

static inline void am335x_set_lcddma_fifo_threshold(void __iomem *base_addr,
                                                    enum dma_fifo_threshold ft)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.th_fifo_ready = ft;
        writel(reg.reg_val, base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}

static inline void am335x_set_lcddma_master_prio(void __iomem *base_addr,
                                                 enum dma_master_prio mp)
{
        union am335x_lcdc_lcddma_ctrl_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
        reg.dma_master_prio = mp;
        writel(reg.reg_val, base_addr + AM335X_LCDC_LCDDMA_CTRL_OFFS);
}



static inline void am335x_set_lcddma_fb0_base_addr(void __iomem *base_addr,
                                                   const void *base)
{
        writel((uintptr_t)base, base_addr +  AM335X_LCDC_LCDDMA_FB0_BASE_OFFS);
}

static inline void am335x_set_lcddma_fb1_base_addr(void __iomem *base_addr,
                                                   const void *base)
{
        writel((uintptr_t)base, base_addr +  AM335X_LCDC_LCDDMA_FB1_BASE_OFFS);
}

static inline void am335x_set_lcddma_fb0_ceil_addr(void __iomem *base_addr,
                                                   const void *ceil)
{
        writel((uintptr_t)ceil, base_addr + AM335X_LCDC_LCDDMA_FB0_CEIL_OFFS);
}

static inline void am335x_set_lcddma_fb1_ceil_addr(void __iomem *base_addr,
                                                   const void *ceil)
{
        writel((uintptr_t)ceil, base_addr + AM335X_LCDC_LCDDMA_FB1_CEIL_OFFS);
}

union am335x_lcddma_irqstatus_raw_reg {
        struct {
                uint32_t done_raw_set : 1;
                uint32_t recurrent_raster_done_raw_set : 1;
                uint32_t sync_raw_set : 1;
                uint32_t acb_raw_set : 1;
                uint32_t reserved1 : 1;
                uint32_t fuf_raw_set : 1;
                uint32_t pl_raw_set : 1;
                uint32_t reserved2 : 1;
                uint32_t eof0_raw_set : 1;
                uint32_t eof1_raw_set : 1;
                uint32_t reserved3 : 22;
        };
        uint32_t reg_val;
};

static inline int am335x_get_lcddma_done_raw_irq(void __iomem *base_addr)
{
        union am335x_lcddma_irqstatus_raw_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_IRQSTATUS_RAW_OFFS);
        return reg.done_raw_set;
}

static inline int am335x_get_lcddma_sync_raw_irq(void __iomem *base_addr)
{
        union am335x_lcddma_irqstatus_raw_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_IRQSTATUS_RAW_OFFS);
        return reg.sync_raw_set;
}

static inline int am335x_get_lcddma_acb_raw_irq(void __iomem *base_addr)
{
        union am335x_lcddma_irqstatus_raw_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_IRQSTATUS_RAW_OFFS);
        return reg.acb_raw_set;
}

static inline int am335x_get_lcddma_fuf_raw_irq(void __iomem *base_addr)
{
        union am335x_lcddma_irqstatus_raw_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_IRQSTATUS_RAW_OFFS);
        return reg.fuf_raw_set;
}

static inline int am335x_get_lcddma_pl_raw_irq(void __iomem *base_addr)
{
        union am335x_lcddma_irqstatus_raw_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_IRQSTATUS_RAW_OFFS);
        return reg.pl_raw_set;
}

static inline int am335x_get_lcddma_fbx_eof_raw_irq(void __iomem *base_addr,
                                                    enum dma_framebuffer fb)
{
        union am335x_lcddma_irqstatus_raw_reg reg;
        reg.reg_val = readl(base_addr + AM335X_LCDC_IRQSTATUS_RAW_OFFS);
        if(fb == FB0)
                return reg.eof0_raw_set;
        else
                return reg.eof1_raw_set;
}

#define AM335X_LCDDMA_IRQ_DONE_CLR_OFFS                         0
#define AM335X_LCDDMA_IRQ_RECURRENT_RASTER_DONE_CLR_OFFS        1
#define AM335X_LCDDMA_IRQ_SYNC_CLR_OFFS                         2
#define AM335X_LCDDMA_IRQ_ACB_CLR_OFFS                          3
#define AM335X_LCDDMA_IRQ_FUF_CLR_OFFS                          5
#define AM335X_LCDDMA_IRQ_PL_CLR_OFFS                           6
#define AM335X_LCDDMA_IRQ_EOF0_CLR_OFFS                         8
#define AM335X_LCDDMA_IRQ_EOF1_CLR_OFFS                         9

static inline int am335x_get_lcddma_done_clr(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_IRQSTATUS_OFFS);
        return READ_REG_BIT(reg, AM335X_LCDDMA_IRQ_DONE_CLR_OFFS);
}

static inline int am335x_get_lcddma_recurrent_raster_done_clr(
                                                        void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_IRQSTATUS_OFFS);
        return READ_REG_BIT(reg, AM335X_LCDDMA_IRQ_RECURRENT_RASTER_DONE_CLR_OFFS);
}

static inline int am335x_get_lcddma_sync_clr(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_IRQSTATUS_OFFS);
        return READ_REG_BIT(reg, AM335X_LCDDMA_IRQ_SYNC_CLR_OFFS);
}

static inline int am335x_get_lcddma_acb_clr(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_IRQSTATUS_OFFS);
        return READ_REG_BIT(reg, AM335X_LCDDMA_IRQ_ACB_CLR_OFFS);
}

static inline int am335x_get_lcddma_fuf_clr(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_IRQSTATUS_OFFS);
        return READ_REG_BIT(reg, AM335X_LCDDMA_IRQ_FUF_CLR_OFFS);
}

static inline int am335x_get_lcddma_pl_clr(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_IRQSTATUS_OFFS);
        return READ_REG_BIT(reg, AM335X_LCDDMA_IRQ_PL_CLR_OFFS);
}

static inline int am335x_get_lcddma_eof0_clr(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_IRQSTATUS_OFFS);
        return READ_REG_BIT(reg, AM335X_LCDDMA_IRQ_EOF0_CLR_OFFS);
}

static inline int am335x_get_lcddma_eof1_clr(void __iomem *base_addr)
{
        unsigned reg = readl(base_addr + AM335X_LCDC_IRQSTATUS_OFFS);
        return READ_REG_BIT(reg, AM335X_LCDDMA_IRQ_EOF1_CLR_OFFS);
}

union am335x_lcddma_irqenable_set_reg {
        struct {
                uint32_t done_en_set : 1;
                uint32_t recurrent_raster_done_en_set : 1;
                uint32_t sync_en_set : 1;
                uint32_t acb_en_set : 1;
                uint32_t reserved1 : 1;
                uint32_t fuf_en_set : 1;
                uint32_t pl_en_set : 1;
                uint32_t reserved2 : 1; 
                uint32_t eof0_en_set : 1;
                uint32_t eof1_en_set : 1;
                uint32_t reserved3 : 22;
        };
        uint32_t reg_val;
};

static inline void am335x_set_lcddma_done_en_set(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_set_reg reg;
        reg.done_en_set = 1; 
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_SET_OFFS);
}

static inline void am335x_set_lcddma_recurrent_raster_done_en_set(
                                                        void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_set_reg reg;
        reg.recurrent_raster_done_en_set = 1; 
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_SET_OFFS);
}

static inline void am335x_set_lcddma_sync_en_set(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_set_reg reg;
        reg.sync_en_set = 1; 
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_SET_OFFS);
}

static inline void am335x_set_lcddma_acb_en_set(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_set_reg reg;
        reg.acb_en_set = 1; 
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_SET_OFFS);
}

static inline void am335x_set_lcddma_fuf_en_set(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_set_reg reg;
        reg.fuf_en_set = 1; 
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_SET_OFFS);
}

static inline void am335x_set_lcddma_pl_en_set(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_set_reg reg;
        reg.pl_en_set = 1; 
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_SET_OFFS);
}

static inline void am335x_set_lcddma_eof0_en_set(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_set_reg reg;
        reg.eof0_en_set = 1; 
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_SET_OFFS);
}

static inline void am335x_set_lcddma_eof1_en_set(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_set_reg reg;
        reg.eof1_en_set = 1; 
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_SET_OFFS);
}

union am335x_lcddma_irqenable_clr_reg {
        struct {
                uint32_t done_en_clr : 1;
                uint32_t recurrent_raster_done_en_clr : 1;
                uint32_t sync_en_clr : 1;
                uint32_t acb_en_clr : 1;
                uint32_t reserved1 : 1;
                uint32_t fuf_en_clr : 1;
                uint32_t pl_en_clr : 1;
                uint32_t reserved2 : 1; 
                uint32_t eof0_en_clr : 1;
                uint32_t eof1_en_clr : 1;
                uint32_t reserved3 : 22;
        };
        uint32_t reg_val;
};

static inline void am335x_set_lcddma_done_en_clr(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_clr_reg reg;
        reg.done_en_clr = 1;
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_CLEAR_OFFS);
}

static inline void am335x_set_lcddma_recurrent_raster_done_en_clr(
                                                        void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_clr_reg reg;
        reg.recurrent_raster_done_en_clr = 1;
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_CLEAR_OFFS);
}

static inline void am335x_set_lcddma_sync_en_clr(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_clr_reg reg;
        reg.sync_en_clr = 1;
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_CLEAR_OFFS);
}

static inline void am335x_set_lcddma_acb_en_clr(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_clr_reg reg;
        reg.acb_en_clr = 1;
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_CLEAR_OFFS);
}

static inline void am335x_set_lcddma_fuf_en_clr(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_clr_reg reg;
        reg.fuf_en_clr = 1;
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_CLEAR_OFFS);
}

static inline void am335x_set_lcddma_pl_en_clr(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_clr_reg reg;
        reg.pl_en_clr = 1;
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_CLEAR_OFFS);
}

static inline void am335x_set_lcddma_eof0_en_clr(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_clr_reg reg;
        reg.eof0_en_clr = 1;
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_CLEAR_OFFS);
}

static inline void am335x_set_lcddma_eof1_en_clr(void __iomem *base_addr)
{
        union am335x_lcddma_irqenable_clr_reg reg;
        reg.eof1_en_clr = 1;
        writel(reg.reg_val, base_addr + AM335X_LCDC_IRQENABLE_CLEAR_OFFS);
}

#endif /* AM335X_REGS_H */