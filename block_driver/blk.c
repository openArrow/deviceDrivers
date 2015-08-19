#include"header.h"

module_init(start);
module_exit(end);

static int start(void)
{
	printk(KERN_INFO"This is %s func",__func__);
	major=register_blkdev(major,"My_Block");	/*to register blkdev*/
	if(major <= 0)
	printk(KERN_INFO"Unable to get major no\n");
	printk(KERN_INFO"Major N0 =%d\n",major);
	dev=kmalloc(sizeof(struct sbull),GFP_KERNEL);
	dev->size=nsectors*sect_size;
	dev->data=vmalloc(dev->size);
	if(dev->data==NULL)
	{
		printk(KERN_NOTICE"Vmalloc failure\n");
	}
	spin_lock_init(&dev->lock);	/*initializing the spin lock*/
	dev->queue=blk_init_queue(sbull_request,&dev->lock);
	setup(dev);
	return 0;
			
}
void setup(struct sbull *dev)  /*gendisk structure initialization*/
{
	printk(KERN_INFO"This is %s func",__func__);
	dev->gd=alloc_disk(1);
	if(!dev->gd)
	{
		printk(KERN_NOTICE"alloc_disk failure");
	}
	dev->gd->major=major;
	dev->gd->first_minor=minor;
	dev->gd->fops=&b_ops;
	dev->gd->queue=dev->queue;
	dev->gd->private_data=dev;
	snprintf(dev->gd->disk_name,15,"Block_device");
	set_capacity(dev->gd,nsectors*sect_size);
	add_disk(dev->gd);			/*making disk available to the system*/
	printk(KERN_INFO"END  %s func",__func__);
	return;
}
static void sbull_request(struct request_queue *q) //Request function
{
	struct request *req;
	printk(KERN_INFO"This is %s func",__func__);
	req=blk_fetch_request(q);
	while(req != NULL)
	{
		if(req==NULL || req->cmd_type != REQ_TYPE_FS )
		{
			printk(KERN_NOTICE"Non CMD req\n");
			__blk_end_request_all(req,-EIO);
			continue;
		}
		sbull_transfer(dev,blk_rq_pos(req),blk_rq_cur_sectors(req),req->buffer,rq_data_dir(req));
		if(!__blk_end_request_cur(req,0))
		{
			req=blk_fetch_request(q);
		}
	}
}
static void sbull_transfer(struct sbull *dev,sector_t sector,unsigned long nsect, char *buffer, int write)   //IO-Request
{
	unsigned long nbytes = nsect * 512;
	if (write)
	memcpy(dev->data, buffer, nbytes);
	else
	memcpy(buffer, dev->data, nbytes);

}
int sbull_getgeo(struct block_device * block_device, struct hd_geometry * geo)    //   Ramdisk geometry
{
	long size;
	size = dev->size * 2;
	geo->cylinders = (size & ~0x3f) >> 6;
	geo->heads = 4;
	geo->sectors = 16;
	geo->start = 0;
	return 0;
}
int sbull_open(struct block_device *blk_device,fmode_t fmod)
{
	#ifdef DEBUG
		printk(KERN_INFO"Start :%s \n",__func__);
	#endif
	#ifdef DEBUG
		printk(KERN_INFO"End :%s \n",__func__);
	#endif
return 0;
}

int sbull_release(struct gendisk *gendisk,fmode_t fmod)
{
	#ifdef DEBUG
		printk(KERN_INFO"Start :%s \n",__func__);
	#endif
	#ifdef DEBUG
		printk(KERN_INFO"End :%s \n",__func__);
	#endif
return 0;
}
static void end(void) 
{
	printk(KERN_INFO"This is %s func",__func__);
	del_gendisk(dev->gd);
	spin_unlock(&dev->lock);
	unregister_blkdev(major,"My_Block");
	
}
