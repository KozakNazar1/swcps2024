#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h> 
#include <stdlib.h>
#define SLEEP_TIME 5
#define ITERATION_NUM 7
#define BUF_SIZE 50
void main(void){
	int i;
	int mPid;
	char buf[BUF_SIZE];
	int locpip[2];
	int errFlag = pipe(locpip);
	if (errFlag == -1){
		fprintf(stderr, "Can't create the pipe.\n");
		exit(1);
	}
	printf("Parent: pipe read ID = %d, "
		"pipe write ID = %d.\n", locpip[0], locpip[1]);
	int cPid = fork();
	switch (cPid){
	case -1:
		fprintf(stderr, "Can't fork for the child.\n");
		exit(1);
	case 0:
		close(locpip[1]);
		mPid = getpid();
		printf("Child: My PID = %d.\n", mPid);
		for (i = 0; i < ITERATION_NUM; i++){
			read(locpip[0], buf, BUF_SIZE);
			printf("Child from parent: %s\n", buf);
			sleep(SLEEP_TIME);
			//for(j=0;j<1000000;j++);
		}
		close(locpip[0]);
		exit(0);
	default:
		close(locpip[0]);
		mPid = getpid();
		printf("Parent: My PID = %d.\n", mPid);
		for (i = 0; i < ITERATION_NUM; i++){
			sprintf(buf, "Transaction message number %05d.", i);
			printf("Parent to child: %s\n", buf);
			write(locpip[1], buf, BUF_SIZE);
			sleep(SLEEP_TIME);
			//for(j=0;j<1000000;j++);
		}
		int status;
		wait(&status);
		close(locpip[1]);
		exit(0); 
	}
}
