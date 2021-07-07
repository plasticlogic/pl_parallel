// SPDX-License-Identifier: GPL-3.0
/*
 * am335x_ctrl.c - controller implementations for am335x based systems
 *
 * Copyright (c) 2021 PL Germany
 * 
 * Authors 
 *      Lars Görner <lars.goerner@plasticlogic.com>
 *
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <ctrl/am335x_ctrl.h>
#include <ctrl/am335x_regs.h>

#define TIMING_DEVICE_NAME      "timings"
#define POLARITY_DEVICE_NAME    "polarities"

#define timing_dev_to_ctrl(tdev) container_of(tdev, struct am335x_ctrl, timing_dev)
#define pol_dev_to_ctrl(pdev) container_of(pdev, struct am335x_ctrl, pol_dev)

////////////////////////////////////////////////////////////////////////////////
// SysFS implementations

/* timings */

// clk_freq
static ssize_t clk_freq_show(struct device *dev, 
                             struct device_attribute *attr, char *buf)
{
        unsigned long clk_freq, clk_div, ret;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);
        clk_freq = clk_get_rate(ctrl->hw_clk);
        clk_div = am335x_lcdc_get_clkdiv(ctrl->reg_base_addr);
        ret = clk_freq / clk_div;
        return sprintf(buf, "%lu\n", ret);
}

static ssize_t clk_freq_store(struct device *dev, struct device_attribute *attr,
                              const char *buf, size_t count)
{
        unsigned long clk_freq, clk_div, new_freq, ret;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = sscanf(buf, "%lu", &new_freq);
        if(ret)
                return ret;
        
        clk_div = am335x_lcdc_get_clkdiv(ctrl->reg_base_addr);
        clk_freq = new_freq * clk_div;
        ret = clk_set_rate(ctrl->hw_clk, clk_freq);
        if(ret)
                return ret;

        return count;
}

static DEVICE_ATTR_RW(clk_freq);

// w_su
static ssize_t w_su_show(struct device *dev, 
                         struct device_attribute *attr, char *buf)
{
        int w_su_cs0, w_su_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);
        w_su_cs0 = am335x_get_lidd_w_su(ctrl->hw_res, LIDD_CS0);
        w_su_cs1 = am335x_get_lidd_w_su(ctrl->hw_res, LIDD_CS1);
        return sprintf(buf, "%d %d\n", w_su_cs0, w_su_cs1);
}

static ssize_t w_su_store(struct device *dev, struct device_attribute *attr,
                          const char *buf, size_t count)
{
        int ret, w_su_cs0, w_su_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d %d\n", &w_su_cs0, &w_su_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_w_su(ctrl->reg_base_addr, LIDD_CS0, w_su_cs0);
        am335x_set_lidd_w_su(ctrl->reg_base_addr, LIDD_CS1, w_su_cs1);
        return count;
}

static DEVICE_ATTR_RW(w_su);

// w_strobe
static ssize_t w_strobe_show(struct device *dev, 
                             struct device_attribute *attr, char *buf)
{
        int w_strobe_cs0, w_strobe_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        w_strobe_cs0 = am335x_get_lidd_w_strobe(ctrl->reg_base_addr, LIDD_CS0);
        w_strobe_cs1 = am335x_get_lidd_w_strobe(ctrl->reg_base_addr, LIDD_CS1);
        return sprintf(buf, "%d %d\n", w_strobe_cs0, w_strobe_cs1);
}

static ssize_t w_strobe_store(struct device *dev, struct device_attribute *attr,
                              const char *buf, size_t count)
{
        int ret, w_strobe_cs0, w_strobe_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d %d\n", &w_strobe_cs0, &w_strobe_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_w_strobe(ctrl->reg_base_addr, LIDD_CS0, w_strobe_cs0);
        am335x_set_lidd_w_strobe(ctrl->reg_base_addr, LIDD_CS1, w_strobe_cs1);
        return count;
}

static DEVICE_ATTR_RW(w_strobe);

// w_hold
static ssize_t w_hold_show(struct device *dev, 
                           struct device_attribute *attr, char *buf)
{
        int w_hold_cs0, w_hold_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);
        
        w_hold_cs0 = am335x_get_lidd_w_hold(ctrl->reg_base_addr, LIDD_CS0);
        w_hold_cs1 = am335x_get_lidd_w_hold(ctrl->reg_base_addr, LIDD_CS1);
        return sprintf(buf, "%d %d\n", w_hold_cs0, w_hold_cs1);
}

static ssize_t w_hold_store(struct device *dev, struct device_attribute *attr,
                            const char *buf, size_t count)
{
        int ret, w_hold_cs0, w_hold_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d %d\n", &w_hold_cs0, &w_hold_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_w_hold(ctrl->reg_base_addr, LIDD_CS0, w_hold_cs0);
        am335x_set_lidd_w_hold(ctrl->reg_base_addr, LIDD_CS1, w_hold_cs1);
        return count;
}

static DEVICE_ATTR_RW(w_hold);

// r_su
static ssize_t r_su_show(struct device *dev, 
                         struct device_attribute *attr, char *buf)
{
        int r_su_cs0, r_su_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        r_su_cs0 = am335x_get_lidd_r_su(ctrl->reg_base_addr, LIDD_CS0);
        r_su_cs1 = am335x_get_lidd_r_su(ctrl->reg_base_addr, LIDD_CS1);
        return sprintf(buf, "%d %d\n", r_su_cs0, r_su_cs1);
}

static ssize_t r_su_store(struct device *dev, struct device_attribute *attr,
                          const char *buf, size_t count)
{
        int ret, r_su_cs0, r_su_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d %d\n", &r_su_cs0, &r_su_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_r_su(ctrl->reg_base_addr, LIDD_CS0, r_su_cs0);
        am335x_set_lidd_r_su(ctrl->reg_base_addr, LIDD_CS1, r_su_cs1);
        return count;
}

static DEVICE_ATTR_RW(r_su);

// r_strobe
static ssize_t r_strobe_show(struct device *dev, 
                             struct device_attribute *attr, char *buf)
{
        int r_strobe_cs0, r_strobe_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        r_strobe_cs0 = am335x_get_lidd_r_strobe(ctrl->reg_base_addr, LIDD_CS0);
        r_strobe_cs1 = am335x_get_lidd_r_strobe(ctrl->reg_base_addr, LIDD_CS1);
        return sprintf(buf, "%d %d\n", r_strobe_cs0, r_strobe_cs1);
}

static ssize_t r_strobe_store(struct device *dev, struct device_attribute *attr,
                              const char *buf, size_t count)
{
        int ret, r_strobe_cs0, r_strobe_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d %d\n", &r_strobe_cs0, &r_strobe_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_r_strobe(ctrl->reg_base_addr, LIDD_CS0, r_strobe_cs0);
        am335x_set_lidd_r_strobe(ctrl->reg_base_addr, LIDD_CS1, r_strobe_cs1);
        return count;
}

static DEVICE_ATTR_RW(r_strobe);

// r_hold
static ssize_t r_hold_show(struct device *dev, 
                           struct device_attribute *attr, char *buf)
{
        int r_hold_cs0, r_hold_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        r_hold_cs0 = am335x_get_lidd_r_hold(ctrl->reg_base_addr, LIDD_CS0);
        r_hold_cs1 = am335x_get_lidd_r_hold(ctrl->reg_base_addr, LIDD_CS1);
        return sprintf(buf, "%d %d\n", r_hold_cs0, r_hold_cs1);
}

static ssize_t r_hold_store(struct device *dev, struct device_attribute *attr,
                            const char *buf, size_t count)
{
        int ret, r_hold_cs0, r_hold_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d %d\n", &r_hold_cs0, &r_hold_cs1);
        if(ret < 0)
                return ret;

        am335x_set_lidd_r_hold(ctrl->reg_base_addr, LIDD_CS0, r_hold_cs0);
        am335x_set_lidd_r_hold(ctrl->reg_base_addr, LIDD_CS1, r_hold_cs1);
        return count;
}

static DEVICE_ATTR_RW(r_hold);

// cs_delay
static ssize_t cs_delay_show(struct device *dev, struct 
                             device_attribute *attr, char *buf)
{
        int cs_delay_cs0, cs_delay_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        cs_delay_cs0 = am335x_get_lidd_ta(ctrl->reg_base_addr, LIDD_CS0);
        cs_delay_cs1 = am335x_get_lidd_ta(ctrl->reg_base_addr, LIDD_CS1);
        return sprintf(buf, "%d %d\n", cs_delay_cs0, cs_delay_cs1);
}

static ssize_t cs_delay_store(struct device *dev, struct device_attribute *attr,
                              const char *buf, size_t count)
{
        int ret, cs_delay_cs0, cs_delay_cs1;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d %d\n", &cs_delay_cs0, &cs_delay_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_ta(ctrl->reg_base_addr, LIDD_CS0, cs_delay_cs0);
        am335x_set_lidd_ta(ctrl->reg_base_addr, LIDD_CS1, cs_delay_cs1);
        return count;
}

static DEVICE_ATTR_RW(cs_delay);

static struct attribute *am335x_timings_attrs[] = {
        &dev_attr_clk_freq.attr,
        &dev_attr_w_su.attr,
        &dev_attr_w_strobe.attr,
        &dev_attr_w_hold.attr,
        &dev_attr_r_su.attr,
        &dev_attr_r_strobe.attr,
        &dev_attr_r_hold.attr,
        &dev_attr_cs_delay.attr,
        NULL,
};

ATTRIBUTE_GROUPS(am335x_timings);

static int am335x_timings_sysfs_register(struct am335x_ctrl *ctrl, struct class *c)
{
        int ret;
        ctrl->timing_dev.class = c;
        ctrl->timing_dev.groups = am335x_timings_groups;

        ret = dev_set_name(&ctrl->timing_dev, TIMING_DEVICE_NAME);
        if(ret)
                return ret;

        ret = device_register(&ctrl->timing_dev);
        if(ret) 
                return ret;
        else
                return 0;
}

static void am335x_timings_sysfs_unregister(struct am335x_ctrl *ctrl)
{
        device_unregister(&ctrl->timing_dev);
}

/* polarites */

// cs0_e0 pol
static ssize_t cs0_e0_pol_show(struct device *dev, 
                               struct device_attribute *attr, char *buf)
{
        int cs0_e0_pol;
        struct am335x_ctrl *ctrl = pol_dev_to_ctrl(dev);

        cs0_e0_pol = am335x_get_cs0_e0_pol(ctrl->reg_base_addr);
        return sprintf(buf, "%d\n", cs0_e0_pol);
}

static ssize_t cs0_e0_pol_store(struct device *dev, 
                                struct device_attribute *attr,
                                const char *buf, size_t count)
{
        int ret, cs0_e0_pol;
        struct am335x_ctrl *ctrl = pol_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d\n", &cs0_e0_pol);
        if(ret < 0) 
                return ret;

        am335x_set_cs0_e0_pol(ctrl->reg_base_addr, cs0_e0_pol);
        return count;
}

static DEVICE_ATTR_RW(cs0_e0_pol);

// cs1_e1 pol
static ssize_t cs1_e1_pol_show(struct device *dev, 
                               struct device_attribute *attr, char *buf)
{
        int cs1_e1_pol;
        struct am335x_ctrl *ctrl = pol_dev_to_ctrl(dev);

        cs1_e1_pol = am335x_get_cs1_e1_pol(ctrl->reg_base_addr);
        return sprintf(buf, "%d\n", cs1_e1_pol);
}

static ssize_t cs1_e1_pol_store(struct device *dev, 
                                struct device_attribute *attr,
                                const char *buf, size_t count)
{
        int ret, cs1_e1_pol;
        struct am335x_ctrl *ctrl = pol_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d\n", &cs1_e1_pol);
        if(ret < 0) 
                return ret;

        am335x_set_cs1_e1_pol(ctrl->reg_base_addr, cs1_e1_pol);
        return count;
}

static DEVICE_ATTR_RW(cs1_e1_pol);

// ws_dir pol
static ssize_t ws_dir_pol_show(struct device *dev, 
                               struct device_attribute *attr, char *buf)
{
        int ws_dir_pol;
        struct am335x_ctrl *ctrl = pol_dev_to_ctrl(dev);

        ws_dir_pol = am335x_get_ws_dir_pol(ctrl->reg_base_addr);
        return sprintf(buf, "%d\n", ws_dir_pol);
}

static ssize_t ws_dir_pol_store(struct device *dev, 
                                struct device_attribute *attr,
                                const char *buf, size_t count)
{
        int ret, ws_dir_pol;
        struct am335x_ctrl *ctrl = pol_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d\n", &ws_dir_pol);
        if(ret < 0) 
                return ret;

        am335x_set_ws_dir_pol(ctrl->reg_base_addr, ws_dir_pol);
        return count;
}

static DEVICE_ATTR_RW(ws_dir_pol);

// rs_en pol
static ssize_t rs_en_pol_show(struct device *dev, 
                              struct device_attribute *attr, char *buf)
{
        int rs_en_pol;
        struct am335x_ctrl *ctrl = pol_dev_to_ctrl(dev);

        rs_en_pol = am335x_get_ws_dir_pol(ctrl->reg_base_addr);
        return sprintf(buf, "%d\n", rs_en_pol);
}

static ssize_t rs_en_pol_store(struct device *dev, 
                               struct device_attribute *attr,
                               const char *buf, size_t count)
{
        int ret, rs_en_pol;
        struct am335x_ctrl *ctrl = pol_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d\n", &rs_en_pol);
        if(ret < 0) 
                return ret;

        am335x_set_rs_en_pol(ctrl->reg_base_addr, rs_en_pol);
        return count;
}

static DEVICE_ATTR_RW(rs_en_pol);

// ale pol
static ssize_t ale_pol_show(struct device *dev, 
                            struct device_attribute *attr, char *buf)
{
        int ale_pol;
        struct am335x_ctrl *ctrl = pol_dev_to_ctrl(dev);

        ale_pol = am335x_get_ale_pol(ctrl->reg_base_addr);
        return sprintf(buf, "%d\n", ale_pol);
}

static ssize_t ale_pol_store(struct device *dev, 
                             struct device_attribute *attr,
                             const char *buf, size_t count)
{
        int ret, ale_pol;
        struct am335x_ctrl *ctrl = pol_dev_to_ctrl(dev);

        ret = sscanf(buf, "%d\n", &ale_pol);
        if(ret < 0) 
                return ret;

        am335x_set_ale_pol(ctrl->reg_base_addr, ale_pol);
        return count;
}

static DEVICE_ATTR_RW(ale_pol);

static struct attribute *am335x_polarities_attrs[] = {
        &dev_attr_cs0_e0_pol.attr,
        &dev_attr_cs1_e1_pol.attr,
        &dev_attr_ws_dir_pol.attr,
        &dev_attr_rs_en_pol.attr,
        &dev_attr_ale_pol.attr,
        NULL,
};

ATTRIBUTE_GROUPS(am335x_polarities);

static int am335x_polarities_sysfs_register(struct am335x_ctrl *ctrl, struct class *c)
{
        int ret;

        ctrl->pol_dev.class = c;
        ctrl->pol_dev.groups = am335x_polarities_groups;
        ret = dev_set_name(&ctrl->pol_dev, POLARITY_DEVICE_NAME);
        if(ret)
                return ret;

        ret = device_register(&ctrl->pol_dev);
        if(ret)
                return ret;
        else
                return 0;
}

static void am335x_polarities_sysfs_unregister(struct am335x_ctrl *ctrl)
{
        device_unregister(&ctrl->pol_dev);
}

////////////////////////////////////////////////////////////////////////////////
// Controller functions

static const unsigned int init_hw_clk_freq = 2000000;

static struct am335x_lidd_timings init_timings = {
        .w_setup = 1,
        .w_strobe = 1,
        .w_hold = 1,
        .r_setup = 1,
        .r_strobe = 1,
        .r_hold = 1,
        .ta = 1
};

static struct am335x_lidd_sig_pol init_sig_pols = {
        .ale_pol = NO_INVERT,
        .rs_en_pol = NO_INVERT,
        .ws_dir_pol = NO_INVERT,
        .cs0_e0_pol = NO_INVERT,
        .cs1_e1_pol = NO_INVERT,
};

static int init(struct controller *ctrl, struct platform_device *pdev, 
                struct class *c)
{
        int ret;
        struct am335x_ctrl *am_ctrl;

        am_ctrl = to_am335x_ctrl(ctrl);

        // get resource
        am_ctrl->hw_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        if(!am_ctrl->hw_res) {
                ret = -ENXIO;
                goto get_pdev_res_fail;
        }

        if(!devm_request_mem_region(&pdev->dev, am_ctrl->hw_res->start, 
                                    resource_size(am_ctrl->hw_res), 
                                    dev_name(&pdev->dev))) {
                ret = -EBUSY;
                goto req_hw_mem_fail;
        }

        am_ctrl->reg_base_addr = devm_ioremap(&pdev->dev, 
                                              am_ctrl->hw_res->start, 
                                              resource_size(am_ctrl->hw_res));
        if(!am_ctrl->reg_base_addr) {
                ret = -ENOMEM;
                goto remap_res_fail;
        }

        // get clk
        am_ctrl->hw_clk = devm_clk_get(&pdev->dev, AM335X_TCON_CLK_IDENTIFIER);
        if(IS_ERR(am_ctrl->hw_clk)) {
                ret = PTR_ERR(am_ctrl->hw_clk);
                goto clk_get_fail;
        }
        
        ret = clk_prepare(am_ctrl->hw_clk);
        if(ret)
                goto clk_prep_fail;

        // set clock frequency
        ret = clk_set_rate(am_ctrl->hw_clk, init_hw_clk_freq);
        if(ret)
                goto clk_set_rate_fail;

        // enable clock
        ret = clk_enable(am_ctrl->hw_clk);
        if(ret)
                goto clk_en_fail;

        pr_info("%s: Add am335x device if to class.\n", THIS_MODULE->name);
        // add object to sysfs
        ret = am335x_timings_sysfs_register(am_ctrl, c);
        if(ret)
                goto timings_add_fail;

        ret = am335x_polarities_sysfs_register(am_ctrl, c);
        if(ret)
                goto polarities_add_fail;

        //// get HRDY GPIO
        //ctrl->hrdy_gpio = gpiod_get(ctrl->dev, HRDY_GPIO_ID, GPIOD_IN);
        //if(IS_ERR(ctrl->hrdy_gpio)) {
        //        dev_err(ctrl->dev, "Request HRDY GPIO failed.");
        //        ret = PTR_ERR(ctrl->hrdy_gpio);
        //        goto hrdy_gpio_fail;
        //}

        // enable clocks        
        am335x_lcdc_set_core_clk_en(am_ctrl->reg_base_addr, 1);
        am335x_lcdc_set_lidd_clk_en(am_ctrl->reg_base_addr, 1);
        am335x_lcdc_set_dma_clk_en(am_ctrl->reg_base_addr, 1);

        // set clock divisor
        am335x_lcdc_set_clkdiv(am_ctrl->reg_base_addr, 2);
#pragma GCC warning "clock divisor must be set properly!"

        // set signal polarities
        am335x_set_lidd_pols(am_ctrl->reg_base_addr, &init_sig_pols);
        
        // set lidd mode
        am335x_set_lidd_mode(am_ctrl->reg_base_addr, ASYNC_MPU80);
        
        // set timings
        am335x_set_lidd_timings(am_ctrl->reg_base_addr, LIDD_CS0, &init_timings);
        am335x_set_lidd_timings(am_ctrl->reg_base_addr, LIDD_CS1, &init_timings);

        // set lcddma config
        am335x_set_lcddma_fifo_threshold(am_ctrl->reg_base_addr, FIFO_TH_16);
        am335x_set_lcddma_burst_size(am_ctrl->reg_base_addr, BURST_SIZE_16);
        am335x_set_lcddma_frame_mode(am_ctrl->reg_base_addr, ONE_FRAME);

        return 0;

//hrdy_gpio_fail:
polarities_add_fail:
        am335x_timings_sysfs_unregister(am_ctrl);
timings_add_fail:
clk_en_fail:
clk_set_rate_fail:
clk_prep_fail:
        devm_clk_put(&pdev->dev, am_ctrl->hw_clk);
clk_get_fail:
        devm_iounmap(&pdev->dev, am_ctrl->reg_base_addr);
remap_res_fail:
        devm_release_mem_region(&pdev->dev, am_ctrl->hw_res, 
                                resource_size(am_ctrl->hw_res));
req_hw_mem_fail:
get_pdev_res_fail:
        return ret;
}

static void destroy(struct controller *ctrl, struct platform_device *pdev,
                    struct class *c)
{
        struct am335x_ctrl *am_ctrl = to_am335x_ctrl(ctrl);
        //gpiod_put(ctrl->ctrl.hrdy_gpio);
        devm_clk_put(&pdev->dev, am_ctrl->hw_clk);
        devm_iounmap(&pdev->dev, am_ctrl->reg_base_addr);
        devm_release_mem_region(&pdev->dev, am_ctrl->hw_res, 
                                resource_size(am_ctrl->hw_res));
        am335x_timings_sysfs_unregister(am_ctrl);
        am335x_polarities_sysfs_unregister(am_ctrl);
        kfree(am_ctrl);
}

static void write_addr(struct am335x_ctrl *ctrl, short addr)
{
        //am335x_set_lidd_dma_en(ctrl->reg_base_addr, 0);
        pr_info("ADDR: 0x%X\n", addr); //ctrl->cmd);
        am335x_set_lidd_addr(ctrl->reg_base_addr, LIDD_CS0, addr); //ctrl->cmd);
}

static void write_data(struct am335x_ctrl *ctrl, const short *data, size_t len)
{
        const short *tmp = data;
        do {
                pr_info("DATA: 0x%X\n", *tmp);
                am335x_set_lidd_data(ctrl->reg_base_addr, LIDD_CS0, *tmp++);
        } while(--len > 0);
        //am335x_set_lidd_dma_en(ctrl->reg_base_addr, 0);
        //am335x_set_lcddma_fbx_base_addr(ctrl->reg_base_addr, FB0, data);
        //am335x_set_lcddma_fbx_ceil_addr(ctrl->reg_base_addr, FB0, data + len - 1);
        //am335x_set_lidd_dma_en(ctrl->reg_base_addr, 1);
}

static ssize_t read(struct controller *ctrl, short *buf, size_t len)
{
        int i;
        struct am335x_ctrl *c = to_am335x_ctrl(ctrl);
        for(i = 0; i < len; i++) {
                *buf++ = am335x_get_lidd_data(c->reg_base_addr, LIDD_CS0);
        }
        return len;
}

static ssize_t write(struct controller *ctrl, const short *buf, size_t len)
{
        struct am335x_ctrl *c = to_am335x_ctrl(ctrl);
        pr_info("Start writing\n");
        pr_info("Write command...\n");
        write_addr(c, *buf);
        if(len > 0) {
                if(!buf)
                        return -ENODATA;
        
                pr_info("Write data...\n");
                write_data(c, buf + 1, len - 1);
                return len;
        }
        return 0;
}

struct controller *am335x_ctrl_create(void)
{
        struct am335x_ctrl *ctrl;

        pr_info("%s: Alloc am335x device memory.\n", THIS_MODULE->name);
        // create controller object
        ctrl = kzalloc(sizeof(*ctrl), GFP_KERNEL);
        if(!ctrl)
                return ERR_PTR(-ENOMEM);

        ctrl->ctrl.init = init;
        ctrl->ctrl.read = read;
        ctrl->ctrl.write = write;
        ctrl->ctrl.destroy = destroy;

        pr_info("%s: Creating am335x device done.\n", THIS_MODULE->name);

        return &ctrl->ctrl;
}