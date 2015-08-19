#include <stdio.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
        int fd,c;
        char ch1[200]; 
         fd = open(argv[1], O_RDONLY);
	printf("Waiting for writer\n");
        read(fd, ch1, 200);
	printf("output:%s\n", ch1);
        close(fd);
        return 0;
}

