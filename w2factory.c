#include <linux/init.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>

int w2factory_init(void)
{
    struct mtd_info *mtd;
    
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