#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include "semaphore.c"
int c;
void *read_driver(void *arg)
{

    int fd;
    char ch1[200];
    fd = open("nod", O_RDONLY);
    read(fd, ch1, c);
    printf("Read successful\n");
    printf("output:%s\n", ch1);
    close(fd);
    pthread_exit("read exits\n");
}
void *write_driver(void *arg)
{
    int fd;
    char ch[]="I am writting into driver 1";
    fd = open("nod", O_WRONLY);
    write(fd, ch, sizeof(ch) );
    c = sizeof(ch);
    printf("Write of %d bytes successful\n",(int)sizeof(ch));
    close(fd);
    pthread_exit("write exits");
}
int main()
{
    pthread_t thread_id,thread_id1;
    sem_id = semget((key_t)1234,1,0777|IPC_CREAT);
    void *thread_result;
    semaphore_p();
    pthread_create(&thread_id ,NULL, write_driver, NULL);
    pthread_join(thread_id,&thread_result);
    semaphore_v();
    pthread_create(&thread_id1 ,NULL, read_driver, NULL);
    pthread_join(thread_id1,&thread_result);

    return 0;
}
