// SPDX-License-Identifier: GPL-3.0
/*
 * am335x_ctrl.c - controller implementations for am335x based systems
 *
 * Copyright (c) 2001-2003 PL Germany
 * 
 * Authors 
 *      Lars Görner <lars.goerner@plasticlogic.com>
 *
 */

#include <ctrl/am335x_ctrl.h>
#include <ctrl/am335x_regs.h>

////////////////////////////////////////////////////////////////////////////////
// SysFS implementations

/* Programm glue */

static ssize_t am335x_attr_show(struct kobject *kobj,
                                struct attribute *attr, char *buf)
{
        struct am335x_attribute *attribute;
        struct am335x_ctrl *ctrl;

        attribute = to_am335x_attr(attr);
        ctrl = to_am335x_ctrl(kobj);

        if(!attribute->show)
                return -EIO;

        return attribute->show(ctrl, attribute, buf);
}

static ssize_t am335_attr_store(struct kobject *kobj,
                                struct attribute *attr,
                                const char *buf, size_t count)
{
        struct am335x_attribute *attribute;
        struct am335x_ctrl *ctrl;

        attribute = to_am335x_attr(attr);
        ctrl = to_am335x_ctrl(kobj);

        if(!attribute->store)
                return -EIO;

        return attribute->store(ctrl, attribute, buf, count);
}

static const struct sysfs_ops am335x_sysfs_ops = {
        .show = am335x_attr_show,
        .store = am335x_attr_store,
};

static void am335x_release(struct kobject *kobj)
{
        struct am335x_ctrl *ctrl;

        ctrl = to_am335x_ctrl(kobj);
        gpiod_put(ctrl->ctrl.hrdy_gpio);
        kfree(ctrl->ctrl.dev, ctrl);
}

/* timings */

// clk_freq

// w_su
static ssize_t w_su_show(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                         char *buf)
{
        int w_su_cs0, w_su_cs1;
        w_su_cs0 = am335x_get_lidd_w_su(ctrl->hw_res, LIDD_CS0);
        w_su_cs1 = am335x_get_lidd_w_su(ctrl->hw_res, LIDD_CS1);
        return sprintf(buf, "%d %d\n", w_su_cs0, w_su_cs1);
}

static ssize_t w_su_store(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                          const char *buf, size_t count)
{
        int ret;
        int w_su_cs0, w_su_cs1;
        ret = sscanf(buf, "%d %d\n", &w_su_cs0, &w_su_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_w_su(ctrl->hw_res, LIDD_CS0, w_su_cs0);
        am335x_set_lidd_w_su(ctrl->hw_res, LIDD_CS1, w_su_cs1);
        return count;
}

static struct am335x_attribute w_su_attribute = __ATTR_RW(w_su);

// w_strobe
static ssize_t w_strobe_show(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                         char *buf)
{
        int w_strobe_cs0, w_strobe_cs1;
        w_strobe_cs0 = am335x_get_lidd_w_strobe(ctrl->hw_res, LIDD_CS0);
        w_strobe_cs1 = am335x_get_lidd_w_strobe(ctrl->hw_res, LIDD_CS1);
        return sprintf(buf, "%d %d\n", w_strobe_cs0, w_strobe_cs1);
}

static ssize_t w_strobe_store(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                          const char *buf, size_t count)
{
        int ret;
        int w_strobe_cs0, w_strobe_cs1;
        ret = sscanf(buf, "%d %d\n", &w_strobe_cs0, &w_strobe_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_w_strobe(ctrl->hw_res, LIDD_CS0, w_strobe_cs0);
        am335x_set_lidd_w_strobe(ctrl->hw_res, LIDD_CS1, w_strobe_cs1);
        return count;
}

static struct am335x_attribute w_strobe_attribute = __ATTR_RW(w_strobe);

// w_hold
static ssize_t w_hold_show(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                         char *buf)
{
        int w_hold_cs0, w_hold_cs1;
        w_hold_cs0 = am335x_get_lidd_w_hold(ctrl->hw_res, LIDD_CS0);
        w_hold_cs1 = am335x_get_lidd_w_hold(ctrl->hw_res, LIDD_CS1);
        return sprintf(buf, "%d %d\n", w_hold_cs0, w_hold_cs1);
}

static ssize_t w_hold_store(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                          const char *buf, size_t count)
{
        int ret;
        int w_hold_cs0, w_hold_cs1;
        ret = sscanf(buf, "%d %d\n", &w_hold_cs0, &w_hold_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_w_hold(ctrl->hw_res, LIDD_CS0, w_hold_cs0);
        am335x_set_lidd_w_hold(ctrl->hw_res, LIDD_CS1, w_hold_cs1);
        return count;
}

static struct am335x_attribute w_hold_attribute = __ATTR_RW(w_hold);

// r_su
static ssize_t r_su_show(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                         char *buf)
{
        int r_su_cs0, r_su_cs1;
        r_su_cs0 = am335x_get_lidd_r_su(ctrl->hw_res, LIDD_CS0);
        r_su_cs1 = am335x_get_lidd_r_su(ctrl->hw_res, LIDD_CS1);
        return sprintf(buf, "%d %d\n", r_su_cs0, r_su_cs1);
}

static ssize_t w_su_store(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                          const char *buf, size_t count)
{
        int ret;
        int r_su_cs0, r_su_cs1;
        ret = sscanf(buf, "%d %d\n", &r_su_cs0, &r_su_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_r_su(ctrl->hw_res, LIDD_CS0, r_su_cs0);
        am335x_set_lidd_r_su(ctrl->hw_res, LIDD_CS1, r_su_cs1);
        return count;
}

static struct am335x_attribute r_su_attribute = __ATTR_RW(r_su);

// r_strobe
static ssize_t r_strobe_show(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                         char *buf)
{
        int r_strobe_cs0, r_strobe_cs1;
        r_strobe_cs0 = am335x_get_lidd_r_strobe(ctrl->hw_res, LIDD_CS0);
        r_strobe_cs1 = am335x_get_lidd_r_strobe(ctrl->hw_res, LIDD_CS1);
        return sprintf(buf, "%d %d\n", r_strobe_cs0, r_strobe_cs1);
}

static ssize_t r_strobe_store(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                          const char *buf, size_t count)
{
        int ret;
        int r_strobe_cs0, r_strobe_cs1;
        ret = sscanf(buf, "%d %d\n", &r_strobe_cs0, &r_strobe_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_r_strobe(ctrl->hw_res, LIDD_CS0, r_strobe_cs0);
        am335x_set_lidd_r_strobe(ctrl->hw_res, LIDD_CS1, r_strobe_cs1);
        return count;
}

static struct am335x_attribute r_strobe_attribute = __ATTR_RW(r_strobe);

// r_hold
static ssize_t r_hold_show(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                         char *buf)
{
        int r_hold_cs0, r_hold_cs1;
        r_hold_cs0 = am335x_get_lidd_r_hold(ctrl->hw_res, LIDD_CS0);
        r_hold_cs1 = am335x_get_lidd_r_hold(ctrl->hw_res, LIDD_CS1);
        return sprintf(buf, "%d %d\n", r_hold_cs0, r_hold_cs1);
}

static ssize_t r_hold_store(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                          const char *buf, size_t count)
{
        int ret;
        int r_hold_cs0, r_hold_cs1;
        ret = sscanf(buf, "%d %d\n", &r_hold_cs0, &r_hold_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_r_hold(ctrl->hw_res, LIDD_CS0, r_hold_cs0);
        am335x_set_lidd_r_hold(ctrl->hw_res, LIDD_CS1, r_hold_cs1);
        return count;
}

static struct am335x_attribute r_hold_attribute = __ATTR_RW(r_hold);

// cs_delay
static ssize_t cs_delay_show(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                         char *buf)
{
        int cs_delay_cs0, cs_delay_cs1;
        cs_delay_cs0 = am335x_get_lidd_ta(ctrl->hw_res, LIDD_CS0);
        cs_delay_cs1 = am335x_get_lidd_ta(ctrl->hw_res, LIDD_CS1);
        return sprintf(buf, "%d %d\n", cs_delay_cs0, cs_delay_cs1);
}

static ssize_t cs_delay_store(struct am335x_ctrl *ctrl, struct am335x_attribute *attr,
                          const char *buf, size_t count)
{
        int ret;
        int cs_delay_cs0, cs_delay_cs1;
        ret = sscanf(buf, "%d %d\n", &cs_delay_cs0, &cs_delay_cs1);
        if(ret < 0) 
                return ret;

        am335x_set_lidd_ta(ctrl->hw_res, LIDD_CS0, cs_delay_cs0);
        am335x_set_lidd_ta(ctrl->hw_res, LIDD_CS1, cs_delay_cs1);
        return count;
}

static struct am335x_attribute cs_delay_attribute = __ATTR_RW(cs_delay);

/* polarites */

// cs0_e0 pol
static ssize_t cs0_e0_pol_show(struct am335x_ctrl *ctrl, 
                               struct am335x_attribute *attr, char *buf)
{
        int cs0_e0_pol;
        cs0_e0_pol = am335x_get_cs0_e0_pol(ctrl->hw_res);
        return sprintf(buf, "%d\n", cs0_e0_pol);
}

static ssize_t cs0_e0_pol_store(struct am335x_ctrl *ctrl, 
                                struct am335x_attribute *attr,
                                const char *buf, size_t count)
{
        int ret;
        int cs0_e0_pol;
        ret = sscanf(buf, "%d\n", &cs0_e0_pol);
        if(ret < 0) 
                return ret;

        am335x_set_cs0_e0_pol(ctrl->hw_res, cs0_e0_pol);
        return count;
}

static struct am335x_attribute cs0_e0_pol_attribute = __ATTR_RW(cs0_e0_pol);

// cs1_e1 pol
static ssize_t cs1_e1_pol_show(struct am335x_ctrl *ctrl, 
                               struct am335x_attribute *attr, char *buf)
{
        int cs1_e1_pol;
        cs1_e1_pol = am335x_get_cs1_e1_pol(ctrl->hw_res);
        return sprintf(buf, "%d\n", cs1_e1_pol);
}

static ssize_t cs1_e1_pol_store(struct am335x_ctrl *ctrl, 
                                struct am335x_attribute *attr,
                                const char *buf, size_t count)
{
        int ret;
        int cs1_e1_pol;
        ret = sscanf(buf, "%d\n", &cs1_e1_pol);
        if(ret < 0) 
                return ret;

        am335x_set_cs1_e1_pol(ctrl->hw_res, cs1_e1_pol);
        return count;
}

static struct am335x_attribute cs1_e1_pol_attribute = __ATTR_RW(cs1_e1_pol);

// ws_dir pol
static ssize_t ws_dir_pol_show(struct am335x_ctrl *ctrl, 
                               struct am335x_attribute *attr, char *buf)
{
        int ws_dir_pol;
        ws_dir_pol = am335x_get_ws_dir_pol(ctrl->hw_res);
        return sprintf(buf, "%d\n", ws_dir_pol);
}

static ssize_t ws_dir_pol_store(struct am335x_ctrl *ctrl, 
                                struct am335x_attribute *attr,
                                const char *buf, size_t count)
{
        int ret;
        int ws_dir_pol;
        ret = sscanf(buf, "%d\n", &ws_dir_pol);
        if(ret < 0) 
                return ret;

        am335x_set_ws_dir_pol(ctrl->hw_res, ws_dir_pol);
        return count;
}

static struct am335x_attribute ws_dir_pol_attribute = __ATTR_RW(ws_dir_pol);

// rs_en pol
static ssize_t rs_en_pol_show(struct am335x_ctrl *ctrl, 
                               struct am335x_attribute *attr, char *buf)
{
        int rs_en_pol;
        rs_en_pol = am335x_get_ws_dir_pol(ctrl->hw_res);
        return sprintf(buf, "%d\n", rs_en_pol);
}

static ssize_t rs_en_pol_store(struct am335x_ctrl *ctrl, 
                                struct am335x_attribute *attr,
                                const char *buf, size_t count)
{
        int ret;
        int rs_en_pol;
        ret = sscanf(buf, "%d\n", &rs_en_pol);
        if(ret < 0) 
                return ret;

        am335x_set_rs_en_pol(ctrl->hw_res, rs_en_pol);
        return count;
}

static struct am335x_attribute rs_en_pol_attribute = __ATTR_RW(rs_en_pol);

// ale pol
static ssize_t ale_pol_show(struct am335x_ctrl *ctrl, 
                               struct am335x_attribute *attr, char *buf)
{
        int ale_pol;
        ale_pol = am335x_get_ale_pol(ctrl->hw_res);
        return sprintf(buf, "%d\n", ale_pol);
}

static ssize_t ale_pol_store(struct am335x_ctrl *ctrl, 
                                struct am335x_attribute *attr,
                                const char *buf, size_t count)
{
        int ret;
        int ale_pol;
        ret = sscanf(buf, "%d\n", &ale_pol);
        if(ret < 0) 
                return ret;

        am335x_set_ale_pol(ctrl->hw_res, ale_pol);
        return count;
}

static struct am335x_attribute ale_pol_attribute = __ATTR_RW(ale_pol);

/* general */


////////////////////////////////////////////////////////////////////////////////
// Controller functions

static const struct am335_ctrl_timings init_timings = {
        .w_setup = 1,
        .w_strobe = 1,
        .w_hold = 1,
        .r_setup = 1,
        .r_strobe = 1,
        .r_hold = 1,
        .ta = 1
};

static const struct am335x_ctrl_sig_pol init_sig_pols = {
        .ale_pol = NO_INVERT,
        .rs_en_pol = NO_INVERT,
        .ws_dir_pol = NO_INVERT,
        .cs0_e0_pol = NO_INVERT,
        .cs1_e1_pol = NO_INVERT,
};

static struct attribute *am335x_default_attrs[] = {
        &w_su_attribute.attr,
        &w_strobe_attribute.attr,
        &w_hold_attribute.attr,
        &r_su_attribute.attr,
        &r_strobe_attribute.attr,
        &r_hold_attribute.attr,
        &cs_delay_attribute.attr,
        &cs0_e0_pol_attribute.attr,
        &cs1_e1_pol_attribute.attr,
        &ws_dir_pol_attribute.attr,
        &rs_en_pol_attribute.attr,
        &ale_pol_attribute.attr,
        NULL, 
};

static struct kobj_type am335x_ktype = {
        .sysfs_ops = &am335x_sysfs_ops,
        .release = am335x_release,
        .default_attr = am335x_default_attrs,
};

static int init(struct controller *ctrl)
{

}

static ssize_t write(struct controller *ctrl, const char *buf, size_t len)
{

}

static void destroy(struct controller *ctrl)
{

}

struct controller *am335x_ctrl_create(struct class *c)
{
        int ret;
        struct am335x_ctrl *ctrl;

        // create controller object
        ctrl = kzalloc(sizeof(*ctrl), GFP_KERNEL);
        if(!ctrl) {
                pr_err("%s: Alloc controller structure memory failed.", 
                        THIS_MODULE->name);
                return ERR_PTR(-ENOMEM);
        }

        ctrl->ctrl.init = init;
        ctrl->ctrl.write = write;
        ctrl->ctrl.destroy = destroy;

        // get HRDY GPIO
        ctrl->ctrl.hrdy_gpio = gpiod_get(dev, HRDY_GPIO_ID, GPIOD_IN);
        if(IS_ERR(ctrl->ctrl.hrdy_gpio)) {
                pr_err("%s: Request HRDY GPIO failed.", THIS_MODULE->name);
                ret = PTR_ERR(ctrl->ctrl.hrdy_gpio);
                goto hrdy_gpio_fail;
        }

        // add object to sysfs


hrdy_gpio_fail:
        destroy(ctrl);
        return ERR_PTR(ret);
}