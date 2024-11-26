#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#define SLEEP_TIME 5
#define ITERATION_NUM 7
void main(int argc, char* argv[])
{
	int i;
	struct sembuf sem_command;
	if (argc != 3)
	{
		fprintf(stderr, "Missing arguments were detected.\n");
		exit(1);
	}
	sem_command.sem_num = 0;//selects first semaphore in array 
	sem_command.sem_flg = SEM_UNDO;//undoes when the process exits 
	int sem1Id = atoi(argv[1]);
	int sem2Id = atoi(argv[2]);
	int mPid = getpid();
	int pPid = getppid();
	printf("EChild: My PID = %d, My parent's PID = %d.\n", mPid,
		pPid);
	sem_command.sem_op = -1; // decrements semval by 1 
	semop(sem1Id, &sem_command, 1);
	sem_command.sem_op = 0; // waits for semval == 0 
	semop(sem2Id, &sem_command, 1);
	for (i = 0; i < ITERATION_NUM; i++)
	{
		printf("EXTERNAL CHILD now is working.\n");
		sleep(SLEEP_TIME);
		//for(j=0;j<1000000;j++);
	}
	exit(0);
}