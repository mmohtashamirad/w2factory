#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/err.h>

/*
 * This module is intended for embedded devices where the mtd partition
 * layout may be hard-coded in the firmware. If, for some reason, you
 * DO have to write to a read-only partition (which is often a bad idea),
 * this module is the way to go.
 *
 * The module is currently limited to the first 64 partitions, but this
 * should suffice for most purposes.
 *
 * Inspired by dougg3@electronics.stackexchange.com:
 * https://electronics.stackexchange.com/a/116133/97342
 */

#ifndef MODULE
#error "Must be compiled as a module."
#endif

#define MOD_WARNING KERN_WARNING "mtd-rw: "
#define MOD_INFO KERN_INFO "mtd-rw: "
#define MOD_ERR KERN_ERR "mtd-rw: "

static bool i_take_the_risk = false;
module_param(i_take_the_risk, bool, S_IRUGO);
MODULE_PARM_DESC(i_take_the_risk, "Make all partitions writeable");

int w2factory_init(void)
{
    struct mtd_info *mtd;
    if (!i_take_the_risk)
    {
        printk(MOD_ERR "must specify i_take_the_risk=1 to continue\n");
        return -EINVAL;
    }

    mtd = get_mtd_device_nm("factory");
    printk(KERN_INFO "value before, 0x%x\n",mtd->flags);
    mtd->flags |= MTD_WRITEABLE;
    printk(KERN_INFO "value after, 0x%x\n",mtd->flags);
    
    printk(KERN_INFO "unlock factory, now it is writeable\n");
    return 0;
}

void w2factory_exit(void)
{
    struct mtd_info *mtd;
    
    mtd = get_mtd_device_nm("factory");

    printk(KERN_INFO "value before, 0x%x\n",mtd->flags);
    mtd->flags &=  ~((uint32_t)MTD_WRITEABLE);
    printk(KERN_INFO "value after, 0x%x\n",mtd->flags);
    
    printk(KERN_INFO "lock factory, now it is read-only\n");


}

module_init(w2factory_init);
module_exit(w2factory_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MorseMicro");