#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MAVERIC");
#ifndef DEBUG
#define DEBUG
#endif
extern int p;
extern int first_minor;
extern int nod ;
extern dev_t dev;

