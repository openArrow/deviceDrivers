#include<asm-generic/ioctl.h>
#include<linux/ioctl.h>

#define SCULL_IOC_MAGIC 'K' 
#define SCULL_IOC_RESET _IO(SCULL_IOC_MAGIC,0)
#define SCULL_IOC_SQUANTUM _IOW(SCULL_IOC_MAGIC,1,int)
#define SCULL_IOC_SQSET _IO(SCULL_IOC_MAGIC,2,int)
#define SCULL_IOC_TQUANTUM _IOR(SCULL_IOC_MAGIC,3,int)
#define SCULL_IOC_GQUANTUM _IOR(SCULL_IOC_MAGIC,4,int)
#define SCULL_IOC_TQSET _IOR(SCULL_IOC_MAGIC,5,int)
#define SCULL_IOC_QQUANTUM _IO(SCULL_IOC_MAGIC,6)
#define SCULL_IOC_QQSET _IO(SCULL_IOC_MAGIC,7)
#define SCULL_IOC_XQUANTUM _IOWR(SCULL_IOC_MAGIC,8,int)
#define SCULL_IOC_XQSET _IOWR(SCULL_IOC_MAGIC,9,int)
#define SCULL_IOC_HQUANTUM _IO(SCULL_IOC_MAGIC,10)
#define SCULL_IOC_HQSET _IO(SCULL_IOC_MAGIC,11)
#define SCULL_IOC_MAXNR 14

