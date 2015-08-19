#include "header.h"
//extern int first_minor;
//extern int nod ;
//extern dev_t dev;
static void __exit hello_exit(void)
{
        #ifdef DEBUG
        printk(KERN_ALERT "Begins:%s\n", __func__);

        #endif
	unregister_chrdev_region(dev, nod);
        #ifdef DEBUG
        printk(KERN_ALERT "End:%s\n", __func__);
        #endif
}
module_exit(hello_exit);

