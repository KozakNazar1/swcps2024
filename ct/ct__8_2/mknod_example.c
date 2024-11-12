#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h> 
#include <stdlib.h> 
#define SLEEP_TIME 5
#define ITERATION_NUM 8
#define BUF_SIZE 50
#define FIFOPIPE "fifopipe"
// PERM = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH 
#define PERM 0666 
void main(void){
	int i;
	int fifoPipe;
	char buf[BUF_SIZE];
	int errFlag;
	errFlag = mknod(FIFOPIPE, S_IFIFO | PERM, 0);
	if (errFlag == -1){
		fprintf(stderr, "Can't create the named fifo pipe.\n");
		exit(1);
	}
	printf("Parent: named fifo pipe has been created '%s'.\n", FIFOPIPE);

	int mPid;
	int cPid = fork();
	switch (cPid)
	{
	case -1:
		fprintf(stderr, "Can't fork for the child.\n");
		exit(1);
	case 0:
		mPid = getpid();
		printf("Child: My PID = %d.\n", mPid);
		fifoPipe = open(FIFOPIPE, O_RDONLY);
		if (fifoPipe == -1){
			fprintf(stderr, "Can't open the named fifo pipe to read.\n");
			exit(1);
		}
		printf("Child: named fifo pipe read ID=%d.\n", fifoPipe);
		for (i = 0; i < ITERATION_NUM; i++){
			read(fifoPipe, buf, BUF_SIZE);
			printf("Child from parent: %s\n", buf);
			//for(j = 0; j < 1000000; j++);
			sleep(SLEEP_TIME);
		}
		close(fifoPipe);
		exit(0);
	default:
		mPid = getpid();
		printf("Parent: My PID=%d.\n", mPid);
		fifoPipe = open(FIFOPIPE, O_WRONLY);
		if (fifoPipe == -1){
			fprintf(stderr, "Can't open the named fifo pipe to write.\n");
			exit(1);
		}
		printf("Parent: named fifo pipe write ID=%d.\n", fifoPipe);
		for (i = 0; i < ITERATION_NUM; i++){
			sprintf(buf, "Transaction message number %05d.", i);
			printf("Parent to child: %s\n", buf);
			write(fifoPipe, buf, BUF_SIZE);
			//for(j=0;j<1000000;j++);
			sleep(SLEEP_TIME);
		}
		int status;
		wait(&status);
		close(fifoPipe);
		errFlag = unlink(FIFOPIPE);
		if (errFlag == -1){
			fprintf(stderr, "Can't delete the named fifo pipe.\n");
			exit(1); 
		} 
		printf("Parent: named fifo pipe has been deleted '%s'.\n", FIFOPIPE); 
		exit(0); 
	}
}