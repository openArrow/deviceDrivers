#include "header.h"
static int __init hello_init(void)
{
	int lv,result;
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
	
///////////////////// WORK QUEUE///////////////////////////////////////////////////////////////	
        wq_str=create_workqueue("serial_workque");
        if(wq_str)
         {
             wq=kmalloc(sizeof(struct work_struct),GFP_KERNEL);
             INIT_WORK(wq,my_wq_func);
        
             //wq2=kmalloc(sizeof(struct work_struct));
             //INIT_WORK(wq2,my_wq2_func2);
            
             rwq=kmalloc(sizeof(struct work_struct),GFP_KERNEL);
             INIT_WORK(rwq,my_rwq_func);
             
             ///rwq2=kmalloc(sizeof(struct work_struct));
            // INIT_WORK(rwq2,my_rwq2_func2);   
         } 
        init_waitqueue_head(&bio);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	result= request_irq(irq,handler,IRQF_DISABLED,"my_interrupt_source",NULL);
	
	printk(KERN_INFO "result_irq------=%d\n",result);
	
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
//////////////////////////////////////////////////////////////////////////////////////
        flush_work(rwq);
	flush_work(wq);
      	flush_workqueue(wq_str);
       destroy_workqueue(wq_str);
////////////////////////////////////////////////////////////////////////////////////

     free_irq(irq,NULL);

#ifdef DEBUG
	printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
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
         
 //////////////////INTERRUPT//////////////////////////////////////////////////////////////////       	
	
	//result= request_irq(irq,handler,SA_INTERRUPT,"my_interrupt_source");

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
	unsigned char dsr_bit5,rts_bit1,status,out2,erbi0,elsi2,emsi3;
        
	int ret;
#ifdef DEBUG
	printk(KERN_ALERT "Begins:%s\n", __func__);
#endif

	dsr_bit5       = 1<<5;
	rts_bit1       = 1<<1; 
	out2           = 1<<3;
	erbi0          = 1<<0;
        elsi2          = 1<<2;
        emsi3          = 1<<3;
	rflag=0;
////////////////SET BAUD RATE TO 9600////////////////////////////////////////////////       
	LCR_REG_T = LCR_REG_T ^ LCR_REG_T;
	LCR_REG_T = DLAB & 1<<7;
	outb(LCR_REG_T,LCR_REG); //setting DLAB bit 0

	DLL_REG_T = 12;    
	DLM_REG_T = 0X00;
	outb(DLL_REG_T,DLL_REG);
	outb(DLM_REG_T,DLM_REG);

	outb(0x00,LCR_REG); //setting the DLAB bit as 0

	outb(0x03,LCR_REG); //setting the word lenth and stop bit
/////////////////EHNABLE INTERRUPT////////////////////////////////////////////////////	
        outb(out2,MCR_REG);
        outb(erbi0,IER_REG);  
        outb(elsi2,IER_REG);  
        outb(emsi3,IER_REG);  

//////////////////////////////////////////////////////////////////////////////////////	
	status= inb(MSR_REG); 
        printk(KERN_INFO "status in read start MSR_REG =%x\n",(int)status);
	
    while(1)
    {    
         
        wait_event_interruptible(bio,rflag==1);
	rflag=0;
	outb(rts_bit1,MCR_REG);
	wait_event_interruptible(bio,rflag==1);
	rflag=0;
	ch=inb(RBR_REG);
	printk(KERN_INFO "data recieved---=%c\n",ch);
        if(ch=='.')
	break;	
    }	
	///////////////////////////////////////////////////////////////// 
	/*while(1)
	{
                
                     // flush_work(rwq);
                     // flush_workqueue(wq_str);
                     // queue_work(wq_str,rwq);
	             	ch=inb(RBR_REG);       
                        if(ch=='.')
		        break;		
	                printk(KERN_INFO "data recieved----=%c\n",ch);
                        wait_event_interruptible(bio,rflag==0);
                        rflag=1;
	}    	
		while(1)
		{
			status= inb(MSR_REG);
			status= status & dsr_bit5;
			if(status)
				break;
		}

	//	printk(KERN_INFO "status after---=%d\n",status);
		/////////////SEND REQUEST TO SEND//////////////////////////////        
		MCR_REG_T = MCR_REG_T ^ MCR_REG_T;
		MCR_REG_T = MCR_REG_T | rts_bit1; 
		outb(MCR_REG_T,MCR_REG);
//		ssleep(1); 
		ch=inb(RBR_REG);
		printk(KERN_INFO "data recieved---=%c\n",ch);
		ret=copy_to_user(buf,&ch,1);
		if(ch == '.') //termination condition for read
		break;
		outb(0x00,MCR_REG);
	}*/
#ifdef DEBUG
	printk(KERN_ALERT "Ends:%s\n", __func__);
#endif

	return 0;
}

ssize_t serialport_write( struct file *filep, const char *buf, size_t count, loff_t *f_pos) {
       
      unsigned char ch,dtr_bit0,cts_bit4,ebti1,out2,emsi3,elsi2;
      unsigned char LCR_REG_T,DLL_REG_T,DLM_REG_T,MCR_REG_T;
      int lcount,ret,i=0;
#ifdef DEBUG
        printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
      lcount=(int)count;
      dtr_bit0       = 1<<0; 
      cts_bit4       = 1<<4;
      out2           = 1<<3;
      ebti1          = 1<<1;
      elsi2          = 0<<2;
      emsi3          = 1<<3;
      flag=0;
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
/////////////ENABLE INTERRUPT//////////////////////////////////////
      outb(out2,MCR_REG);
      outb(elsi2,IER_REG);
      outb(emsi3,IER_REG);
///////////////////////////////////////////////////////////
      outb(0x03,LCR_REG); //setting the word length and stop bit
//////////////////////////////////////////////////////////////////   
 
 while(lcount)
  {    
     ret=copy_from_user(&ch,buf+i,1);
      
      printk(KERN_INFO "data to send is =%c\n",ch);
      
      MCR_REG_T = MCR_REG_T ^ MCR_REG_T ;// FLUSH MCR_REG_T
      MCR_REG_T = MCR_REG_T | dtr_bit0;
      outb(MCR_REG_T,MCR_REG);// SET DTR BIT
      
      wait_event_interruptible(bio,flag==1); 
      printk(KERN_INFO "******wait_event_interrupt executed*****************\n");

      outb(ch,THR_REG);    
      msleep(100);
      outb(0,MCR_REG);
      flag=0;
      lcount--;
      i++;
  }

      /*ret=copy_from_user(&ch,buf+i,1);
      printk(KERN_INFO "data to send is =%c\n",ch);

      outb(ch,THR_REG);
      
	      
      wait_event_interruptible(bio,flag==1); 
      --lcount;
      i++;
      flag=0;
      msleep(500);

  }   
      while(lcount)
    {	      
      MCR_REG_T = MCR_REG_T ^ MCR_REG_T; //flush out MCR_T reg
      MCR_REG_T = MCR_REG_T | dtr_bit0;              // set DTR requst
      outb(MCR_REG_T,MCR_REG);
     :20

      // flush_work(wq);
      // flush_workqueue(wq_str);

      queue_work(wq_str,wq);
      wait_event_interruptible(bio,flag==1); 
      flag=0;
     / while(1)
      {
        status= inb(MSR_REG); 
        status= status & cts_bit4;
        
        if(status)
        break;
      }*
      //printk(KERN_INFO "status =%d\n",(int)status);
      ret=copy_from_user(&ch,buf+i,1);
      printk(KERN_INFO "data to send is =%c\n",ch);
      outb(ch,THR_REG);
      //outb(0,MCR_REG); 
      msleep(110); 
      outb(0,MCR_REG); 
      printk(KERN_INFO "status of LSR =%d\n",(int)inb(LSR_REG));
      --lcount;
      i++;
     }*/
#ifdef DEBUG
        printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
    return lcount;

}
 void my_rwq_func(struct work_struct *work)
   {
   
   unsigned char status,dsr_bit5;
         dsr_bit5= 1<<5;
         #ifdef DEBUG
            printk(KERN_ALERT "Begins----------------------------------------:%s\n", __func__);
         #endif

         
               status= inb(MSR_REG);
               printk(KERN_INFO "status before=%d\n",(int)status);  
                 while(1)
                {
                         schedule();
                       
                        status= inb(MSR_REG);
                        status= status & dsr_bit5;
                        if(status)
                        break;
                }
               rflag=1;
               printk(KERN_INFO "status after=%d\n",(int)status);  
               #ifdef DEBUG
                  printk(KERN_ALERT "end--------------------------------------------:%s\n", __func__);
            #endif
          wake_up_interruptible(&bio);
           
   }
void my_wq_func(struct work_struct *work)
 {
    unsigned char status,cts_bit4;
    #ifdef DEBUG
        printk(KERN_ALERT "Begins---:%s\n", __func__);
   #endif
  
    cts_bit4 = 1<<4;
     while(1)
      {
        schedule();
        status= inb(MSR_REG);
        status= status & cts_bit4;
        if(status)
        break;
      }
   flag=1;
   #ifdef DEBUG
        printk(KERN_ALERT "end---:%s\n", __func__);
   #endif

   wake_up_interruptible(&bio);
   
}

irqreturn_t handler (int val,void *ptr)
{

         int inb_status;
          unsigned char status,cts_bit4,dsr_bit5,data;
          cts_bit4 = 1<<4;
	  dsr_bit5 = 1<<5;
		  
  #ifdef DEBUG
         printk(KERN_ALERT "begin---:%s\n", __func__);
   #endif
        
      
	 inb_status=inb(IIR_REG);
	 printk(KERN_INFO "inb_status in interrupt handler= %x\n",inb_status);
	 switch(inb_status)
	 {
		 case 0x01:
                          printk(KERN_INFO "reciever line status interrupt enabled LSR_REG\n");
		          break;
		 case 0x04:	  
                          
                          printk(KERN_INFO "**********recieved data available ion RBR_REG*******\n");
                        
			  rflag=1;
			  wake_up_interruptible(&bio); 
			  break;
		 case 0x0c:	  
                          printk(KERN_INFO "character timeout indication\n");
                 case 0x02:

                          printk(KERN_INFO "*****TRANSMITTING HOLDING REG THR_REG empty*******\n");
                          flag=1;
			  wake_up_interruptible(&bio);
			  break;
		 case 0x00:	  
                          status=inb(MSR_REG);
                          
                           printk(KERN_INFO "########## status-----MSR_REG ########### =%d\n",(int)status);
			  if((status & dsr_bit5)==dsr_bit5)
			      {  
                                  printk(KERN_INFO "*****MODEM STATUS DSR_BIT5 INTERRUPT ENABLED******\n");
		                  queue_work(wq_str,rwq);
			      }		 
                          else if((status & cts_bit4)==cts_bit4)
			      {
                                  printk(KERN_INFO "*****MODEM STATUS CTS_BIT4 INTERRUPT ENABLED******\n");
		                  queue_work(wq_str,wq);

			      }
			  else
			      {
                                  printk(KERN_INFO "*****MODEM STATUS INTERRUPT ENABLED BUT CTS AND DSR ARE DISABLED******\n");
                               }

                           break;
		 default:
                            printk(KERN_INFO "default interrupt wakeup \n");
        }   

   #ifdef DEBUG
         printk(KERN_ALERT "end---:%s\n", __func__);
   #endif
 
   return 0;
}

