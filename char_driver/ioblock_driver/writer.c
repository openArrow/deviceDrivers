#include <stdio.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
        int fd,c;
        char ch1[200], ch[]="I am writting into driver I am writting into driver I am writting into driver I am writting into driver";
	printf("writing %d bytes into the driver/n",(int) sizeof(ch));
        fd = open(argv[1], O_WRONLY);
        write(fd, ch, sizeof(ch) );
        c = sizeof(ch);
        printf("Application running\n");
        close(fd);
        return 0;
}

