#include <unistd.h> 
#include <sys/wait.h> 
#include <stdio.h> 
#include <stdlib.h> 

#define SLEEP_TIME 5 
#define ITERATION_NUM 7 

void main(void)
{
	int i = 0;
	int cPid = fork();
	int errFlag;
	int mPid;
	int pPid;
	switch (cPid)
	{
	case -1:
		fprintf(stderr, "Can't fork for a child.\n");
		exit(1);
	case 0:
		errFlag = execl("./child", "child", (char*)0);
		if (errFlag == -1)
		{
			fprintf(stderr, "Can't execute the external child.\n");
			exit(1);
		}
		break;
	default:
		mPid = getpid();
		pPid = getppid();
		printf("Parent: My Pid = %d, "
			"Parent Pid = %d, "
			"Child Pid = %d\n", mPid, pPid, cPid);
		for (i; i < ITERATION_NUM; i++)
		{
			printf("PARENT now is working.\n");
			sleep(SLEEP_TIME);
			//for(j=0;j<1000000;j++); 
		}
		int status;
		wait(&status);
		printf("Parent: Child exit code is %d.\n", (status & 0xff00) >> 8);
		exit(0);
	}
}
