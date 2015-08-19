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
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/interrupt.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MAVERIC");
MODULE_DESCRIPTION("Serial port driver");

#ifndef DLAB 
#define DLAB 1<<7
#endif


#ifndef BASEPORT 
#define BASEPORT 0x3f8
#endif

#ifndef RBR_REG  
#define RBR_REG BASEPORT+0
#endif

#ifndef THR_REG  
#define THR_REG BASEPORT+0
#endif

#ifndef DLL_REG 
#define DLL_REG BASEPORT+0
#endif

#ifndef IER_REG 
#define IER_REG BASEPORT+1
#endif


#ifndef DLM_REG
#define DLM_REG BASEPORT+1
#endif

#ifndef IIR_REG 
#define IIR_REG BASEPORT+2
#endif

#ifndef FCR_REG 
#define FCR_REG BASEPORT+2
#endif

#ifndef LCR_REG 
#define LCR_REG BASEPORT+3
#endif

#ifndef MCR_REG 
#define MCR_REG BASEPORT+4
#endif

#ifndef LSR_REG 
#define LSR_REG BASEPORT+5
#endif

#ifndef MSRR_REG 
#define MSR_REG BASEPORT+6
#endif

#ifndef SCR_REG 
#define SCRR_REG BASEPORT+7
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
#define QSET_SIZE 8
#endif

#ifndef QUANTUM_SIZE 
#define QUANTUM_SIZE 8
#endif




dev_t dev;
int nod = NOD;
int first_minor = FIRST;
int res = 0;
int major;
int port;
int flag;
dev_t dev;

void my_wq_func(struct work_struct *);
void my_wq2_func(struct work_struct *);
void my_rwq_func(struct work_struct *);
void my_rwq2_func(struct work_struct *);

struct workqueue_struct *wq_str;
struct work_struct *wq,*wq2,*rwq,rwq2;
wait_queue_head_t bio;
struct serial_ScullQset
{
	struct ScullQset *next;
	void **data;
};

struct serial_ScullDev
{
	struct serial_ScullQset *serial_scullqset;
	int quantum;
        int qset;
	unsigned long size;
	struct cdev i_cdev;
	
};
struct serial_ScullDev *serial_sculldev;
int serialport_open(struct inode *inode, struct file *filp);
int serialport_release(struct inode *inode, struct file *filp);	
ssize_t serialport_write(struct file *, const char __user *, size_t , loff_t *);
ssize_t serialport_read(struct file *, char __user *, size_t , loff_t *);
const struct file_operations ops={
	open:serialport_open,
	write:serialport_write,
	read:serialport_read,
	release:serialport_release
};




