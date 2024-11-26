#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdio.h> 
#include <stdlib.h>
#define SLEEP_TIME 5
#define ITERATION_NUM 7
void main(void)
{
	int i;
	int errFlag;
	int status;
	struct sembuf sem_command;
	// two rwxrwxrwx semaphore arrays, each of one semaphore 
	int sem1Id = semget(IPC_PRIVATE, 1, 0xfff);
	if (sem1Id == -1)
	{
		fprintf(stderr, "Can't create first semaphore.\n");
		exit(1);
	}
	int sem2Id = semget(IPC_PRIVATE, 1, 0xfff);
	if (sem2Id == -1)
	{
		fprintf(stderr, "Can't create second semaphore.\n");
		exit(1);
	}
	sem_command.sem_num = 0;//selects first semaphore in array 
	sem_command.sem_op = 2; //increments semval by 2 
	sem_command.sem_flg = SEM_UNDO;//undoes when the process exits 
	semop(sem1Id, &sem_command, 1);//activates one semaphore 
	sem_command.sem_op = 1;//increments semval by 1 
	semop(sem2Id, &sem_command, 1);//activates one semaphore 
	char arg1[5];
	char arg2[5];
	int mPid;
	int pPid;
	int ecPid;
	int icPid = fork();
	switch (icPid)
	{
	case -1:
		fprintf(stderr, "Can't fork for internal child.\n");
		exit(1);
	case 0:
		ecPid = fork();
		switch (ecPid)
		{
		case -1:
			fprintf(stderr, "Can't fork for external child.\n");
			exit(1);
		case 0:
			sprintf(arg1, "%d", sem1Id);
			sprintf(arg2, "%d", sem2Id);
			errFlag = execl("./sembuf_example__child", "sembuf_example__child", arg1, arg2, (char*)0);
			if (errFlag == -1)
			{
				fprintf(stderr, "Can't execute the external child.\n");
				exit(1);
			}
		default:
			mPid = getpid();
			pPid = getppid();
			printf("IChild: My PID = %d, "
				"My parent's PID = %d, "
				"My child's PID = %d.\n",
				mPid, pPid, ecPid);
			sem_command.sem_op = -1;//decrements semval by 1 
			semop(sem1Id, &sem_command, 1);
			sem_command.sem_op = 0;//waits for semval == 0 
			semop(sem2Id, &sem_command, 1);
			for (i = 0; i < ITERATION_NUM; i++)
			{
				printf("INTERNAL CHILD now is working.\n");
				sleep(SLEEP_TIME);
				//for(j=0;j<1000000;j++);
			}
			wait(&status);
			printf("\nIChild: EChild exit code is %d.\n",
				(status & 0xff00) >> 8);
			exit(1);
		}
	default:
		mPid = getpid();
		pPid = getppid();
		printf("Parent: My PID = %d, "
			"My parent's PID = %d, "
			"My child's PID = %d.\n",
			mPid, pPid, icPid);
		sem_command.sem_op = 0; // waits for semval==0 
		semop(sem1Id, &sem_command, 1);
		printf("Parent:Press the <Enter>key to continue...\n");
		getchar();
		sem_command.sem_op = -1; // decrements semval by 1 
		semop(sem2Id, &sem_command, 1);
		for (i = 0; i < ITERATION_NUM; i++)
		{
			printf("PARENT now is working.\n");
			sleep(SLEEP_TIME);
			//for(j=0;j<1000000;j++);
		}
		wait(&status);
		printf("Parent: IChild exit code is %d.\n", (status &
			0xff00) >> 8);
		exit(0);
	}
}