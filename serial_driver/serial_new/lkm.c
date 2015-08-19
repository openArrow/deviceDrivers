#include "header.h"
static int __init hello_init(void)
{
    int lv;
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
    res = alloc_chrdev_region(&dev, first_minor, nod, "serial_dev");
    if(res == -1)
    {
	printk(KERN_ERR "Registration Failed");
	goto out;
    }
    port = check_region(BASEPORT,8); //to check if port is free or not
    if(port<0)
    {
	release_region(BASEPORT,8); //to release the port
    }
    if(!request_region(BASEPORT, 8, "my_serial_driver"))
	goto out;

    printk(KERN_ALERT "Inserting parport module\n");
    serial_sculldev = (struct serial_ScullDev*)kmalloc(sizeof(struct serial_ScullDev)*nod, GFP_KERNEL);
    for(lv = 0; lv<nod; lv++)
    {
	serial_sculldev[lv].i_cdev.owner = THIS_MODULE;
	serial_sculldev[lv].i_cdev.ops = &ops;
	serial_sculldev[lv].i_cdev.dev = dev;
	serial_sculldev[lv].quantum = QUANTUM_SIZE;
	serial_sculldev[lv].qset = QSET_SIZE;
	serial_sculldev[lv].size = DEV_SIZE;
	dev = MKDEV(MAJOR(dev), lv);
	printk(KERN_ALERT "Major no.:%d\n", MAJOR(dev));
	printk(KERN_ALERT "Minor no.:%d\n", MINOR(dev));
	cdev_init(&(serial_sculldev[lv].i_cdev), &ops);
	cdev_add(&(serial_sculldev[lv].i_cdev), dev, 1);
    }
    ///////////wait queue initialization/////////////////////////////////////////////////////////////////////////////
    wq_str = create_workqueue("my_workqueue");
    if(wq_str)
    {
	wq = kmalloc(sizeof(struct work_struct),GFP_KERNEL);
	INIT_WORK(wq,my_wq_func);




	rwq = kmalloc(sizeof(struct work_struct),GFP_KERNEL);
	INIT_WORK(rwq,my_rwq_func);



    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    init_waitqueue_head(&bio);
    outb(0,RBR_REG);
    outb(0,THR_REG);
    outb(0,IER_REG);
    outb(0,FCR_REG);
    outb(0,LCR_REG);
    outb(0,MCR_REG);


    //printk(KERN_INFO "status in init=%d\n",(int)inb(STATUS));
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
    kfree(serial_sculldev);
    cdev_del(&(serial_sculldev->i_cdev));
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
    ///////////////////destroying work queue/////////////////////////////////////////////
    flush_workqueue(wq_str);
    destroy_workqueue(wq_str);
    if (!port) {
	release_region(BASEPORT,8);
    }
    printk("<1>Removing module serialport\n");
#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif

}
module_init(hello_init);
module_exit(hello_exit);

int serialport_open(struct inode *inode, struct file *filep) {

    struct serial_ScullDev *lserial_sculldev;
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
    lserial_sculldev=container_of(inode->i_cdev,struct serial_ScullDev,i_cdev);
    filep->private_data=lserial_sculldev;

    if((filep->f_flags & O_ACCMODE)==O_WRONLY)
    {
	printk(KERN_INFO "file opened in WRONLY MODE");
    }
    if((filep->f_flags & O_ACCMODE)==O_RDONLY)
    {
	printk(KERN_INFO "file opened in RDONLY MODE");
    }
    if((filep->f_flags & O_ACCMODE)==O_RDWR)
    {
	printk(KERN_INFO "file opened in RDWR MODE");
    }

#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
    return 0;

}
int serialport_release(struct inode *inode, struct file *filep) {
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif

    filep->private_data=NULL;
#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
    return 0;
}

ssize_t serialport_read(struct file *filp, char *buf,size_t count, loff_t *f_pos) {


    unsigned char LCR_REG_T,DLL_REG_T,DLM_REG_T,MCR_REG_T,ch; 
    unsigned char dsr_bit5,rts_bit1,status;
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif

    dsr_bit5       = 1<<5;
    rts_bit1       = 1<<1; 
    /////////SET BAUD RATE TO 9600//////////////////////////////       
    LCR_REG_T = LCR_REG_T ^ LCR_REG_T;
    LCR_REG_T = DLAB & 1<<7;
    outb(LCR_REG_T,LCR_REG); //setting DLAB bit 0

    DLL_REG_T = 12;    
    DLM_REG_T = 0X00;
    outb(DLL_REG_T,DLL_REG);
    outb(DLM_REG_T,DLM_REG);

    outb(0x00,LCR_REG); //setting the DLAB bit as 0

    outb(0x03,LCR_REG); //setting the word length and stop bit
    status= inb(MSR_REG); 
    printk(KERN_INFO "status before---=%d\n",status);
    ///////////////////////////////////////////////////////////////// 
    while(1)
    {
	queue_work(wq_str,rwq);
	wait_event_interruptible(bio,flag==0);

	/////////////SEND REQUEST TO SEND//////////////////////////////        
	MCR_REG_T = MCR_REG_T ^ MCR_REG_T;
	MCR_REG_T = MCR_REG_T | rts_bit1; 
	outb(MCR_REG_T,MCR_REG);
	ssleep(1); 
	ch=inb(RBR_REG);
	printk(KERN_INFO "data recieved---=%c\n",ch);
	copy_to_user(buf,&ch,1);
	if(ch == '.') //termination condition for read
	    break;
	outb(0x00,MCR_REG);
    }
#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif

    return 0;
}

ssize_t serialport_write( struct file *filep, const char *buf, size_t count, loff_t *f_pos) {

    unsigned char ch,dtr_bit0,cts_bit4;
    unsigned char LCR_REG_T,DLL_REG_T,DLM_REG_T,MCR_REG_T;
    int lcount,ret,i=0;
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
    lcount=(int)count;
    dtr_bit0       = 1<<0; 
    cts_bit4       = 1<<4;
    LCR_REG_T = LCR_REG_T ^ LCR_REG_T;  //flush out lcdr_register
    outb(LCR_REG_T,LCR_REG);

    //////SETTING BAUD RATE TO 9600//////////////////////////////////////////
    LCR_REG_T=DLAB & 1<<7; 
    outb(LCR_REG_T,LCR_REG); //making DLAB bit 1

    DLL_REG_T=12;
    outb(DLL_REG_T,DLL_REG);

    DLM_REG_T=0X00;
    outb(DLM_REG_T,DLM_REG);
    outb(0x00,LCR_REG); //making DLAB bit 0 to access THR and RBR register 
    outb(0x03,LCR_REG); //setting the word length and stop bit
    while(lcount)
    {	      
	MCR_REG_T = MCR_REG_T ^ MCR_REG_T; //flush out MCR_T reg
	MCR_REG_T = MCR_REG_T | dtr_bit0;              // set DTR requst
	outb(MCR_REG_T,MCR_REG);
	queue_work(wq_str,wq);
	wait_event_interruptible(bio,flag==1); 
	ret=copy_from_user(&ch,buf+i,1);
	printk(KERN_INFO "data to send is =%c\n",ch);
	outb(ch,THR_REG);
	outb(0,MCR_REG); 
	ssleep(2); 
	printk(KERN_INFO "status of LSR =%d\n",(int)inb(LSR_REG));
	--lcount;
	i++;
    }
#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
    return lcount;

}


////////////////////////////////////function for work queue in write ///////////////////////////////////////////////
void my_wq_func(struct work_struct *work)
{
    unsigned char status,cts_bit4;
    cts_bit4       = 1<<4;
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif

    while(1)
    {
	status= inb(MSR_REG);
	schedule();
	status= status & cts_bit4;

	if(status)
	    break;
    }

    printk(KERN_INFO "status =%d\n",(int)status);
    flag=1;
    wake_up_interruptible(&bio);
#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif

}

////////////////////////////////////function for work queue in read ////////////////////////////////////////
void my_rwq_func(struct work_struct *work)
{
    unsigned char status,dsr_bit5;
    dsr_bit5       = 1<<5;
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif

    while(1)
    {
	status= inb(MSR_REG);
	schedule();
	status= status & dsr_bit5;
	if(status)
	    break;
    }
    printk(KERN_INFO "status after---=%d\n",status);

    flag=0;
    wake_up_interruptible(&bio);
#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
}
