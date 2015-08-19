#include <stdio.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
        int fd,c;
        char ch[200];
        fd = open(argv[1], O_RDONLY);
        if(fd<0)
        perror("Error\n");
        read(fd, ch, 100);
        printf("Application running\n");
        close(fd);
        return 0;
}

