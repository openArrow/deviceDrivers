#include <stdio.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	int fd,c;
	char ch1[200], ch[]="ABCDEFGHIJK";
	fd = open(argv[1], O_WRONLY);
	if(fd<0)
	perror("Error\n");
	write(fd, ch, sizeof(ch) );
	c = sizeof(ch);
	printf("Application running\n");
	close(fd);
	return 0;
}
