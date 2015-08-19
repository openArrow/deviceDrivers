#include <stdio.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	int fd,c;
	char ch1[200], ch[]="I am writting into driver I am writting into driver I am writting into driver I am writting into driver";
	fd = open(argv[1], O_WRONLY);
	write(fd, ch, sizeof(ch) );
	c = sizeof(ch);
	printf("Application running\n");
	
	close(fd);
	 fd = open(argv[1], O_RDONLY);	
	read(fd, ch1, c);
	printf("output:%s\n", ch1);
	close(fd);
	return 0;
}
