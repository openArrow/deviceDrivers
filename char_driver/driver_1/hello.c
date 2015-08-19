#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
MODULE_LICENSE("GPL");
int nod;
module_param(nod, int, S_IRUGO);
static int init_module1(void)
{
printk("<1>begins:%s\n",__func__);
printk(KERN_ALERT"Hello World %d.\n", nod);
return 0;
}
static void cleanup_module1(void)
{


}
module_init(init_module1);
module_exit(cleanup_module1);
