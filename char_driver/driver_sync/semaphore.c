
#include <unistd.h>
#include <sys/types.h>

#include <sys/sem.h>
#include <sys/ipc.h>

int sem_id;
/*struct sembuf
{
 short sem_num;
 short sem_op;
 short sem_flg;
};*/ 

union semun {
int val;
struct semid_ds *buf;
unsigned short int *array;
};

void set_sem_value ()
{
union semun sem_union;
//unsigned short values[1];
//values[0] = 1;
sem_union.val = 1;
semctl (sem_id, 0, SETVAL, sem_union);
}

static void semaphore_p()
{
struct sembuf sem_b;
sem_b.sem_num = 0;
sem_b.sem_op = -1;
sem_b.sem_flg = SEM_UNDO; 
semop(sem_id, &sem_b, 1);
}

static void semaphore_v()
{
struct sembuf sem_b;
sem_b.sem_num = 0;
sem_b.sem_op = 1;
sem_b.sem_flg = SEM_UNDO;
semop(sem_id, &sem_b, 1);
}
