#include "header.h"
module_param(nod, int,  S_IRUGO);
int scull_open(struct inode *inode, struct file *filp)
{
    struct ScullDev *lsculldev;
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
    lsculldev = container_of(inode->i_cdev, struct ScullDev,i_cdev);
    filp->private_data = lsculldev;
#ifdef DEBUG
    printk(KERN_ALERT "End:%s\n", __func__);
#endif
    return 0;
}

int scull_release(struct inode *inode, struct file *filp)
{
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
    filp->private_data = NULL;
#ifdef DEBUG
    printk(KERN_ALERT "End:%s\n", __func__);
#endif
    return 0;
}

ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *off)
{
    int lcount,rcount, lquantum, lqset,i,j,size,ret,k,pos;
    struct ScullDev *lsculldev;
    struct ScullQset *temp1, *temp2;
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif

    lsculldev = filp->private_data;
    lquantum = lsculldev->quantum;
    lqset = lsculldev->qset;
    lcount = (int)count;
    rcount = (int)count;
    k = lcount/lquantum + ((lcount%lquantum) ? 1 : 0);
    size = k / lqset + ((k%lqset) ? 1:0); 
    pos = 0;
    if(lcount>0)
    {
	for(j = 0; j<size; j++)
	{
	    temp1 = (struct ScullQset *)kmalloc(sizeof(struct ScullQset), GFP_KERNEL);
	    temp1->data =(void **) kmalloc(sizeof(char *)*lqset, GFP_KERNEL);
	    for(i= 0; i<lqset && rcount >=0; i++)
	    {
		temp1->data[i] = (char *)kmalloc(sizeof(char)*lquantum, GFP_KERNEL);
		ret = copy_from_user(temp1->data[i], buf + lquantum*pos, lquantum);
		rcount = rcount - lquantum;
		pos++;
	    }
	    if(j == 0)
	    {
		lsculldev->scullqset = temp1;
		temp2=temp1;
	    }
	    else
	    {
		temp2->next = temp1;
		temp2 = temp1;
	    }
	}
    }
    temp2->next = NULL;
    *off=count;
    printk(KERN_ALERT "offset%d\n", (int)*off);
    //if(lsculldev->size<*off)
    //	lsulldev->size = *off;

#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
    wait_flag = 1;
    wake_up(&(lsculldev[0].my_queue));
    return count;
}

ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *off)
{
    struct ScullDev *lsculldev = filp->private_data;
    struct ScullQset *temp1;
    int lquantum, lqset,lcount,i,k;
    wait_event(sculldev[0].my_queue, wait_flag!=0);
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
    lcount = (int)count;
    lquantum = lsculldev->quantum;
    lqset = lsculldev->qset;
    temp1 = lsculldev->scullqset;
    if(lsculldev == NULL)
	goto out;
    if(lsculldev->scullqset == NULL)
	goto out;
    k=0;
    while(lcount>0 )
    {
	for(i=0; i<lqset && lcount >0; i++)
	{
	    copy_to_user(buf + lquantum*k, temp1->data[i], lquantum);
	    k++;
	    lcount = lcount - lquantum;
	}
	temp1 = temp1->next;
    }
    wait_flag=0;  //wait_flag set to zero to block next read to wait for next write operation
#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
out:
    return 0;
}	
static int __init hello_init(void)
{
    int lv;
    //	ScullDev *lsculldev;
    lv = nod;
    res = alloc_chrdev_region(&dev, first_minor, nod, "chrdev");
    if(res == -1)
    {
	printk(KERN_ERR "Registration Failed");
	goto out;
    }
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
    sculldev = (struct ScullDev*)kmalloc(sizeof(struct ScullDev)*nod, GFP_KERNEL);
    for(lv = 0; lv<nod; lv++)
    {

	sculldev[lv].i_cdev.owner = THIS_MODULE;
	sculldev[lv].i_cdev.ops = &ops;
	sculldev[lv].i_cdev.dev = dev;
	sculldev[lv].quantum = QUANTUM_SIZE;
	sculldev[lv].qset = QSET_SIZE;
	sculldev[lv].size = DEV_SIZE;
	ldev = MKDEV(MAJOR(dev), lv);
	printk(KERN_ALERT "Major no.:%d\n", MAJOR(ldev));
	printk(KERN_ALERT "Minor no.:%d\n", MINOR(ldev));
	cdev_init(&(sculldev[lv].i_cdev), &ops);
	init_waitqueue_head(&(sculldev[lv].my_queue));
	cdev_add(&(sculldev[lv].i_cdev), dev, 1);
    }

#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif
    return 0;
out:
    return -1;
}

static void __exit hello_exit(void)
{
    //	int lv;
    unregister_chrdev_region(dev, nod);
    // for(lv = 0; lv<=nod; lv++)
    //	{
    kfree(sculldev);
    cdev_del(&(sculldev->i_cdev));
    //}
#ifdef DEBUG
    printk(KERN_ALERT "Begins:%s\n", __func__);
#endif
#ifdef DEBUG
    printk(KERN_ALERT "Ends:%s\n", __func__);
#endif

}
module_init(hello_init);
module_exit(hello_exit);



