#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int main(int argc,char *argv[])
{
	int fd;
	fd=open("/dev/Block_device",O_WRONLY);
	if(fd<0)
	{
		printf("Open unsuccessful");
	}
	else
	{
		printf("File open");
	}
	close(fd);
	return 0;
}
