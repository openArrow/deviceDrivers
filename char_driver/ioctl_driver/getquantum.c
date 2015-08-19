#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include"ioctl_header.h"
int main(int argc,char *argv[])
{
    int fd1,count,count1,new_pos=0;
    char buff[250];
    int y;
    fd1 = open(argv[1],O_RDONLY);
    ioctl(fd1,SCULL_IOC_TQUANTUM,&y);
    printf("quantum_size y = %d\n",y);
    if(fd1>0)
    {
	printf("read mode opened successfully\n");
    }
    else
    {
	printf("opened not successfully\n");
	return -1;
    }


    return 0;
}

