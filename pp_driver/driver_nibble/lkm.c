#include "header.h"
static int __init hello_init(void)
{
    int lv;
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
    res = alloc_chrdev_region(&dev, first_minor, nod, "ppdev");
    if(res == -1)
    {
	printk(KERN_ERR "Registration Failed");
	goto out;
    }
    port = check_region(BASEPORT,3); //to check if port is free or not
    if(port<0)
    {
	release_region(BASEPORT,3); //to release the port
    }
    if(!request_region(BASEPORT, 3, DRVNAME))
	goto out;
    printk(KERN_ALERT "Inserting parport module\n");
    pp_sculldev = (struct pp_ScullDev*)kmalloc(sizeof(struct pp_ScullDev)*nod, GFP_KERNEL);
    for(lv = 0; lv<nod; lv++)
    {

	pp_sculldev[lv].i_cdev.owner = THIS_MODULE;
	pp_sculldev[lv].i_cdev.ops = &ops;
	pp_sculldev[lv].i_cdev.dev = dev;
	pp_sculldev[lv].quantum = QUANTUM_SIZE;
	pp_sculldev[lv].qset = QSET_SIZE;
	pp_sculldev[lv].size = DEV_SIZE;
	ldev = MKDEV(MAJOR(dev), lv);
	printk(KERN_ALERT "Major no.:%d\n", MAJOR(ldev));
	printk(KERN_ALERT "Minor no.:%d\n", MINOR(ldev));
	cdev_init(&(pp_sculldev[lv].i_cdev), &ops);
	cdev_add(&(pp_sculldev[lv].i_cdev), dev, 1);
    }
#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
    return 0;
out:
    printk(KERN_ERR "Error\n");
    return -1;
}
static void __exit hello_exit(void)
{

    unregister_chrdev_region(dev, nod);
    kfree(pp_sculldev);
    cdev_del(&(pp_sculldev->i_cdev));
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
    if (!port) {
	release_region(BASEPORT,3);
    }
    printk("<1>Removing module parportleds\n");
#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif

}
module_init(hello_init);
module_exit(hello_exit);

int parport_open(struct inode *inode, struct file *filp) {
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif

#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
    return 0;

}
int parport_release(struct inode *inode, struct file *filp) {
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif

#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
    return 0;
}
ssize_t parport_read(struct file *filp, char *buf,size_t count, loff_t *f_pos) {
    char status_bufferR,status_bufferL,parport_buffer;
    int b,a=0;
    outb(0,BASEPORT);
    //    outb(0,STATUS);
    outb(0,CONTROL);
    while(1)
    {
	while(!a)
	{
	    b = inb(STATUS);
	    a=(b & 1<<7);
	}
	outb(1,CONTROL); 
	status_bufferR = inb(STATUS);
	status_bufferR = status_bufferR << 1;
        status_bufferR = status_bufferR >> 4;
        outb(0,CONTROL);
        while(!a)
        {
            b = inb(STATUS);
            a=(b & 1<<7);
        }
        status_bufferL = inb(STATUS);
	status_bufferL = status_bufferL << 1;
	parport_buffer = status_bufferL | status_bufferR ;
	copy_to_user(buf,&parport_buffer,1);
    }
    return 0;

}

ssize_t parport_write( struct file *filp, const char *buf, size_t count, loff_t     *f_pos) {
    int i,a=1;
    //  char *tmp;
    struct pp_ScullDev *pp_lsculldev1;
    struct pp_ScullQset *pp_lscullqset;
    int lquantum,lqset,lcount,b;
    char parport_buffer,cr,cl;
    lcount = (int)count;
    /*pp_lsculldev1 = filp->private_data;
      lquantum = pp_lsculldev1->quantum;
      lqset = pp_lsculldev1->qset;
      pp_lscullqset = (struct pp_ScullQset *)kmalloc(sizeof(struct pp_ScullQset), GFP_KERNEL);
      pp_lscullqset->data =(void **) kmalloc(sizeof(char *)*lqset, GFP_KERNEL);
      pp_lscullqset->data[0] = (void *)0x378;
      pp_lscullqset->data[1] = (void *)0x379;
      pp_lscullqset->data[2] = (void *)0x37A; */
    //flushing the registers
    outb(0,BASEPORT);
    //  outb(0,STATUS);
    outb(0,CONTROL);
    //sending control signal to reader

    i = 0;
    while(lcount)
    {

	//      outb(*(pp_lscullqset->data[2]),  pp_lscullqset->data[2]);
	outb(1<<0, CONTROL);
	while(!a)
	{
	    b=inb(STATUS);
	    a=(b & 1<<7);
	}

	copy_from_user(&parport_buffer,buf + i,1);
	i++;
	cl = parport_buffer >>4;
	cr=parport_buffer <<4;
	cr = cr >>4;
	outb(cr,BASEPORT);
	outb(0,CONTROL);
	while(!a)
	{
	    b=inb(STATUS);
	    a=(b & 1<<7);
	}
	outb(cl,BASEPORT);
	lcount = lcount - 1;
    }
    return 1;
}
