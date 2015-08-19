#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MAVERIC");
MODULE_DESCRIPTION("Parelle port driver");
#ifndef BASEPORT 
#define BASEPORT 0x378
#endif

#ifndef STATUS 
#define STATUS 0x379
#endif

#ifndef CONTROL 
#define CONTROL 0x37A
#endif

#ifndef DRVNAME 
#define DRVNAME "ppdev"
#endif



#ifndef DEBUG 
#define DEBUG
#endif

#ifndef NOD 
#define NOD 1
#endif

#ifndef FIRST 
#define FIRST 0
#endif

#ifndef DEV_SIZE
#define DEV_SIZE 1024
#endif

#ifndef DATA_SIZE 
#define DATA_SIZE 0
#endif

#ifndef QSET_SIZE 
#define QSET_SIZE 3
#endif

#ifndef QUANTUM_SIZE 
#define QUANTUM_SIZE 1
#endif


dev_t dev;
int nod = NOD;
int first_minor = FIRST;
int res = 0;
//struct file_operations *ops;
int major;
int port;
dev_t ldev;
struct pp_ScullQset
{
	struct ScullQset *next;
	void **data;
};
struct pp_ScullDev
{
	struct pp_ScullQset *pp_scullqset;
	int quantum;
        int qset;
	unsigned long size;
	struct cdev i_cdev;
};
struct pp_ScullDev *pp_sculldev;
int parport_open(struct inode *inode, struct file *filp);
int parport_release(struct inode *inode, struct file *filp);	
ssize_t parport_write(struct file *, const char __user *, size_t , loff_t *);
//ssize_t parport_read(struct file *, char __user *, size_t , loff_t *);
const struct file_operations ops={
	open:parport_open,
	write:parport_write,
	//read:parport_read,
	release:parport_release
};
//struct file_opeartion ops;
//sculldev->quantum = QUANTUM_SIZE;
//sculldev->qset = QSET_SIZE;
//ZZsculldev->size = DATA_SIZE;
