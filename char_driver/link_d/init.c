#include "header.h"
int first_minor=0;
int nod=1;
dev_t dev;
static int __init hello_init(void)
{
	int res;
        #ifdef DEBUG
        printk(KERN_ALERT "Begins:%s\n", __func__);
        #endif
	res = alloc_chrdev_region(&dev, first_minor, nod, "chrdev");
	if(res == -1)
        {
                printk(KERN_ERR "Registration Failed");
                goto out;
        }
	printk(KERN_ALERT "Major no.:%d\n", MAJOR(dev));
        printk(KERN_ALERT "Minor no.:%d\n", MINOR(dev));
        #ifdef DEBUG
        printk(KERN_ALERT "End:%s\n", __func__);
        #endif
	return 0;
	out:
	return -1;
}
module_init(hello_init);
