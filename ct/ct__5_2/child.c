#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 

#define SLEEP_TIME 5 
#define ITERATION_NUM 7 

void main(void)
{
	int i = 0;
	int mPid = getpid();
	int pPid = getppid();
	printf("Child: My PID = %d, My parent's PID = %d\n\n", mPid, pPid);
	for (i; i < ITERATION_NUM; i++)
	{
		printf("CHILD now is working.\n");
		sleep(SLEEP_TIME);
		//for(j=0;j<1000000;j++); 
	}
	exit(0);
}
