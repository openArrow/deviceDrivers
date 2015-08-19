#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <asm/tsc.h>
#include <linux/proc_fs.h>
#include <linux/timex.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MAVERIC");
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

#ifndef PROC_NAME
#define PROC_NAME "hello_proc_time"
#endif

#ifndef DATA_SIZE 
#define DATA_SIZE 0
#endif

#ifndef QSET_SIZE 
#define QSET_SIZE 8
#endif

#ifndef QUANTUM_SIZE 
#define QUANTUM_SIZE 8
#endif


dev_t dev;
int nod = NOD;
int first_minor = FIRST;
int res = 0;
//struct file_operations *ops;
int major;
unsigned long j1,j2;
dev_t ldev;
struct ScullQset
{
	struct ScullQset *next;
	void **data;
};
struct ScullDev
{
	struct ScullQset *scullqset;
	int quantum;
        int qset;
	unsigned long size;
	struct cdev i_cdev;
	struct proc_dir_entry *entry;
};
struct ScullDev *sculldev;
int scull_open(struct inode *inode, struct file *filp);
int scull_release(struct inode *inode, struct file *filp);	
ssize_t scull_write(struct file *, const char __user *, size_t , loff_t *);
ssize_t scull_read(struct file *, char __user *, size_t , loff_t *);
int read_proc(char *buf,char **start,off_t offset,int count,int *eof,void *data ); 
const struct file_operations ops={
	open:scull_open,
	write:scull_write,
	read:scull_read,
	release:scull_release
};
//struct file_opeartion ops;
//sculldev->quantum = QUANTUM_SIZE;
//sculldev->qset = QSET_SIZE;
//ZZsculldev->size = DATA_SIZE;
