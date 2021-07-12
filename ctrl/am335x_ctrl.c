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
#include <linux/wait.h>
#include <linux/jiffies.h>
#include <ctrl/am335x_ctrl.h>
#include <ctrl/am335x_regs.h>

#define TIMING_DEVICE_NAME      "timings"
#define POLARITY_DEVICE_NAME    "polarities"
#define TIMEOUT_MSECS           1000

#define timing_dev_to_ctrl(tdev) container_of(tdev, struct am335x_ctrl, timing_dev)
#define pol_dev_to_ctrl(pdev) container_of(pdev, struct am335x_ctrl, pol_dev)

#undef READ_DATA_BURST

////////////////////////////////////////////////////////////////////////////////
// SysFS implementations

/* timings */

// clk_freq
static ssize_t clk_freq_show(struct device *dev, 
                             struct device_attribute *attr, char *buf)
{
        unsigned long clk_freq;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);
        clk_freq = clk_get_rate(ctrl->hw_clk);
        return sprintf(buf, "%lu\n", clk_freq);
}

static ssize_t clk_freq_store(struct device *dev, struct device_attribute *attr,
                              const char *buf, size_t count)
{
        unsigned long clk_freq, ret;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = kstrtoul(buf, 10, &clk_freq);
        if(ret)
                return ret;
        
        ret = clk_set_rate(ctrl->hw_clk, clk_freq);
        if(ret)
                return ret;

        return count;
}

static DEVICE_ATTR_RW(clk_freq);

// clk_div
static ssize_t clk_div_show(struct device *dev,
                            struct device_attribute *attr, char *buf)
{
        int clk_div;
        struct am335x_ctrl *ctrl;

        ctrl = timing_dev_to_ctrl(dev);

        clk_div = am335x_lcdc_get_clkdiv(ctrl->reg_base_addr);
        return sprintf(buf, "%d\n", clk_div);
}

static ssize_t clk_div_store(struct device *dev,
                             struct device_attribute *attr,
                             const char *buf, size_t count)
{
        int ret, clk_div;
        struct am335x_ctrl *ctrl;

        ctrl = timing_dev_to_ctrl(dev);
        ret = kstrtoint(buf, 10, &clk_div);
        if(ret)
                return ret;
        
        am335x_lcdc_set_clkdiv(ctrl->reg_base_addr, clk_div);
        return count;
}

static DEVICE_ATTR_RW(clk_div);

// w_su
static ssize_t w_su_show(struct device *dev, 
                         struct device_attribute *attr, char *buf)
{
        int w_su;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);
        w_su = am335x_get_lidd_w_su(ctrl->hw_res, LIDD_CS0);
        return sprintf(buf, "%d\n", w_su);//, w_su_cs1);
}

static ssize_t w_su_store(struct device *dev, struct device_attribute *attr,
                          const char *buf, size_t count)
{
        int ret, w_su;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = kstrtoint(buf, 10, &w_su);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_w_su(ctrl->reg_base_addr, LIDD_CS0, w_su);
        return count;
}

static DEVICE_ATTR_RW(w_su);

// w_strobe
static ssize_t w_strobe_show(struct device *dev, 
                             struct device_attribute *attr, char *buf)
{
        int w_strobe;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        w_strobe = am335x_get_lidd_w_strobe(ctrl->reg_base_addr, LIDD_CS0);
        return sprintf(buf, "%d\n", w_strobe);
}

static ssize_t w_strobe_store(struct device *dev, struct device_attribute *attr,
                              const char *buf, size_t count)
{
        int ret, w_strobe;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = kstrtoint(buf, 10, &w_strobe);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_w_strobe(ctrl->reg_base_addr, LIDD_CS0, w_strobe);
        return count;
}

static DEVICE_ATTR_RW(w_strobe);

// w_hold
static ssize_t w_hold_show(struct device *dev, 
                           struct device_attribute *attr, char *buf)
{
        int w_hold;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);
        
        w_hold = am335x_get_lidd_w_hold(ctrl->reg_base_addr, LIDD_CS0);
        return sprintf(buf, "%d\n", w_hold);
}

static ssize_t w_hold_store(struct device *dev, struct device_attribute *attr,
                            const char *buf, size_t count)
{
        int ret, w_hold;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = kstrtoint(buf, 10, &w_hold);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_w_hold(ctrl->reg_base_addr, LIDD_CS0, w_hold);
        return count;
}

static DEVICE_ATTR_RW(w_hold);

// r_su
static ssize_t r_su_show(struct device *dev, 
                         struct device_attribute *attr, char *buf)
{
        int r_su;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        r_su = am335x_get_lidd_r_su(ctrl->reg_base_addr, LIDD_CS0);
        return sprintf(buf, "%d\n", r_su);
}

static ssize_t r_su_store(struct device *dev, struct device_attribute *attr,
                          const char *buf, size_t count)
{
        int ret, r_su;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = kstrtoint(buf, 10, &r_su);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_r_su(ctrl->reg_base_addr, LIDD_CS0, r_su);
        return count;
}

static DEVICE_ATTR_RW(r_su);

// r_strobe
static ssize_t r_strobe_show(struct device *dev, 
                             struct device_attribute *attr, char *buf)
{
        int r_strobe;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        r_strobe = am335x_get_lidd_r_strobe(ctrl->reg_base_addr, LIDD_CS0);
        return sprintf(buf, "%d\n", r_strobe);
}

static ssize_t r_strobe_store(struct device *dev, struct device_attribute *attr,
                              const char *buf, size_t count)
{
        int ret, r_strobe;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = kstrtoint(buf, 10, &r_strobe);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_r_strobe(ctrl->reg_base_addr, LIDD_CS0, r_strobe);
        return count;
}

static DEVICE_ATTR_RW(r_strobe);

// r_hold
static ssize_t r_hold_show(struct device *dev, 
                           struct device_attribute *attr, char *buf)
{
        int r_hold;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        r_hold = am335x_get_lidd_r_hold(ctrl->reg_base_addr, LIDD_CS0);
        return sprintf(buf, "%d\n", r_hold);
}

static ssize_t r_hold_store(struct device *dev, struct device_attribute *attr,
                            const char *buf, size_t count)
{
        int ret, r_hold;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = kstrtoint(buf, 10, &r_hold);
        if(ret < 0)
                return ret;

        am335x_set_lidd_r_hold(ctrl->reg_base_addr, LIDD_CS0, r_hold);
        return count;
}

static DEVICE_ATTR_RW(r_hold);

// cs_delay
static ssize_t cs_delay_show(struct device *dev, struct 
                             device_attribute *attr, char *buf)
{
        int cs_delay;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        cs_delay = am335x_get_lidd_ta(ctrl->reg_base_addr, LIDD_CS0);
        return sprintf(buf, "%d\n", cs_delay);
}

static ssize_t cs_delay_store(struct device *dev, struct device_attribute *attr,
                              const char *buf, size_t count)
{
        int ret, cs_delay;
        struct am335x_ctrl *ctrl = timing_dev_to_ctrl(dev);

        ret = kstrtoint(buf, 10, &cs_delay);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_ta(ctrl->reg_base_addr, LIDD_CS0, cs_delay);
        return count;
}

static DEVICE_ATTR_RW(cs_delay);

static struct attribute *am335x_timings_attrs[] = {
        &dev_attr_clk_freq.attr,
        &dev_attr_clk_div.attr,
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

static void timings_dev_release(struct device *dev)
{
        memset(dev, 0, sizeof(*dev));
}

static int am335x_timings_sysfs_register(struct am335x_ctrl *ctrl, struct class *c)
{
        int ret;
        ctrl->timing_dev.class = c;
        ctrl->timing_dev.groups = am335x_timings_groups;
        ctrl->timing_dev.release = timings_dev_release;

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

        ret = kstrtoint(buf, 10, &cs0_e0_pol);
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

        ret = kstrtoint(buf, 10, &cs1_e1_pol);
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

        ret = kstrtoint(buf, 10, &ws_dir_pol);
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

        ret = kstrtoint(buf, 10, &rs_en_pol);
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

        ret = kstrtoint(buf, 10, &ale_pol);
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

static void polarities_dev_release(struct device *dev)
{
        memset(dev, 0, sizeof(*dev));
}

static int am335x_polarities_sysfs_register(struct am335x_ctrl *ctrl, struct class *c)
{
        int ret;

        ctrl->pol_dev.class = c;
        ctrl->pol_dev.groups = am335x_polarities_groups;
        ctrl->pol_dev.release = polarities_dev_release;
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

static const unsigned int init_hw_clk_freq = 200000000;
static const int init_clk_div = 2;

static struct am335x_lidd_timings init_timings = {
        .w_setup = 7,
        .w_strobe = 15,
        .w_hold = 15,
        .r_setup = 7,
        .r_strobe = 15,
        .r_hold = 3,
        .ta = 1
};

static struct am335x_lidd_sig_pol init_sig_pols = {
        .ale_pol = NO_INVERT,
        .rs_en_pol = NO_INVERT,
        .ws_dir_pol = NO_INVERT,
        .cs0_e0_pol = NO_INVERT,
        .cs1_e1_pol = NO_INVERT,
};

static inline int wait_hrdy_timeout(struct am335x_ctrl *ctrl) {
        unsigned long end_jiffies = jiffies + msecs_to_jiffies(TIMEOUT_MSECS);
        do {
                if(gpiod_get_value(ctrl->hrdy_gpio))
                        return 0;
        } while(jiffies < end_jiffies);
        return -ETIME;
}

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

        am_ctrl->reg_base_addr = devm_ioremap_nocache(&pdev->dev, 
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

        // request HRDY GPIO
        am_ctrl->hrdy_gpio = devm_gpiod_get(&pdev->dev, HRDY_GPIO_ID, GPIOD_IN);
        if(IS_ERR(am_ctrl->hrdy_gpio)) {
                ret = PTR_ERR(am_ctrl->hrdy_gpio);
                goto hrdy_gpio_req_fail;
        }

        // add object to sysfs
        ret = am335x_timings_sysfs_register(am_ctrl, c);
        if(ret)
                goto timings_add_fail;

        ret = am335x_polarities_sysfs_register(am_ctrl, c);
        if(ret)
                goto polarities_add_fail;

        // enable clocks        
        am335x_lcdc_set_core_clk_en(am_ctrl->reg_base_addr, 1);
        am335x_lcdc_set_lidd_clk_en(am_ctrl->reg_base_addr, 1);
        am335x_lcdc_set_dma_clk_en(am_ctrl->reg_base_addr, 1);

        // set clock divisor
        am335x_lcdc_set_clkdiv(am_ctrl->reg_base_addr, init_clk_div);

        // set signal polarities
        am335x_set_lidd_pols(am_ctrl->reg_base_addr, &init_sig_pols);

        am335x_lcdc_set_ctrl_mode(am_ctrl->reg_base_addr, LIDD_MODE);
        
        // set lidd mode
        am335x_set_lidd_mode(am_ctrl->reg_base_addr, SYNC_MPU80);
        
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
        devm_gpiod_put(&pdev->dev, am_ctrl->hrdy_gpio);
hrdy_gpio_req_fail:
clk_en_fail:
clk_set_rate_fail:
clk_prep_fail:
        devm_clk_put(&pdev->dev, am_ctrl->hw_clk);
clk_get_fail:
        devm_iounmap(&pdev->dev, am_ctrl->reg_base_addr);
remap_res_fail:
        devm_release_mem_region(&pdev->dev, am_ctrl->hw_res->start, 
                                resource_size(am_ctrl->hw_res));
req_hw_mem_fail:
get_pdev_res_fail:
        return ret;
}

static void destroy(struct controller *ctrl, struct platform_device *pdev,
                    struct class *c)
{
        struct am335x_ctrl *am_ctrl = to_am335x_ctrl(ctrl);
        devm_gpiod_put(&pdev->dev, am_ctrl->hrdy_gpio);
        devm_clk_put(&pdev->dev, am_ctrl->hw_clk);
        devm_iounmap(&pdev->dev, am_ctrl->reg_base_addr);
        devm_release_mem_region(&pdev->dev, am_ctrl->hw_res->start,
                                resource_size(am_ctrl->hw_res));
        am335x_timings_sysfs_unregister(am_ctrl);
        am335x_polarities_sysfs_unregister(am_ctrl);
        kfree(am_ctrl);
}

static void write_addr(struct am335x_ctrl *ctrl, short addr)
{
        am335x_set_lidd_addr(ctrl->reg_base_addr, LIDD_CS0, addr);
}

static int write_data(struct am335x_ctrl *ctrl, const short *data, size_t len)
{
        int ret;
        const short *tmp = data;
        ret = 0;
        do {
#               ifndef WRITE_DATA_BURST
                ret = wait_hrdy_timeout(ctrl);
                if(ret) {
                        pr_warn("%s: Write I8080 timeout!\n", THIS_MODULE->name);
                        return -EIO;
                }
#               endif
                am335x_set_lidd_data(ctrl->reg_base_addr, LIDD_CS0, *tmp++);
        } while(--len > 0);
        return 0;
}

static ssize_t read(struct controller *ctrl, short *buf, size_t len)
{
        int i, ret;
        struct am335x_ctrl *c = to_am335x_ctrl(ctrl);
        ret = 0;
        wait_hrdy_timeout(c);
        for(i = 0; i < len; i++) {
#               ifndef READ_DATA_BURST
                ret = wait_hrdy_timeout(c);
                if(ret) {
                        pr_warn("%s: Read I8080 timeout!\n", THIS_MODULE->name);
                        return -EIO;
                }
#               endif
                buf[i] = am335x_get_lidd_data(c->reg_base_addr, LIDD_CS0);
        }
        return len;
}

static ssize_t write(struct controller *ctrl, const short *buf, size_t len)
{
        struct am335x_ctrl *c = to_am335x_ctrl(ctrl);
        wait_hrdy_timeout(c);
        write_addr(c, buf[0]);
        if(len > 1) {
                wait_hrdy_timeout(c);
                write_data(c, &buf[1], len - 1);
                return len;
        }
        return 1;
}

struct controller *am335x_ctrl_create(void)
{
        struct am335x_ctrl *ctrl;

        // create controller object
        ctrl = kzalloc(sizeof(*ctrl), GFP_KERNEL);
        if(!ctrl)
                return ERR_PTR(-ENOMEM);

        ctrl->ctrl.init = init;
        ctrl->ctrl.read = read;
        ctrl->ctrl.write = write;
        ctrl->ctrl.destroy = destroy;

        return &ctrl->ctrl;
}