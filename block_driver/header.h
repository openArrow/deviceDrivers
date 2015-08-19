#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/genhd.h>
#include<linux/blkdev.h>
#include<linux/hdreg.h>

int major,minor;
int nsectors=4;
int sect_size=512;

struct sbull      /*Device Structure*/
{
	u8 *data;			/*The data array*/
	int size;			/*Device size in sector*/
	struct gendisk *gd;		/*kernel representation of indivisual disk drive*/
	spinlock_t lock;		/*For mutual exclusion*/
	struct request_queue *queue;	/*The device request queue*/
}*dev;
void setup(struct sbull *);
static void sbull_request(struct request_queue *);
static void sbull_transfer(struct sbull *,sector_t ,unsigned long , char *, int );
int sbull_getgeo(struct block_device *, struct hd_geometry *);
int sbull_open(struct block_device *,fmode_t );
int sbull_release(struct gendisk *,fmode_t );
struct block_device_operations b_ops={   //Device operations
owner: THIS_MODULE,
open: sbull_open,
release: sbull_release,
getgeo:sbull_getgeo
};
static int start(void);
static void end(void);
