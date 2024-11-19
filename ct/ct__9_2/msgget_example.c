#include <unistd.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/wait.h> 
#include <stdio.h> 
#include <stdlib.h> 

#define SLEEP_TIME 5 
#define ITERATION_NUM 7 
#define BUF_SIZE 50 

void main(void){
	long i;
	int errFlag;
	struct{
		long mtype;
		char mtext[BUF_SIZE];
	} message;
	int mqueue = msgget(IPC_PRIVATE, 0666 | IPC_CREAT); // !!!!!!!!! WSL is not supported !!!!!!!!!
	if (mqueue == -1){
		fprintf(stderr, "Can't associate the messages queue.\n");
		exit(1);
	}
	printf("Parent: messages queue has been created.\n");
	int mPid;
	int cPid = fork();
	switch (cPid){
	case -1:
		fprintf(stderr, "Can't fork for the child.\n");
		exit(1);
	case 0:
		mPid = getpid();
		printf("Child: My PID = %d.\n", mPid);
		for (i = 1; i < ITERATION_NUM; i++){
			errFlag = msgrcv(mqueue, (struct msgbuf*)&message, BUF_SIZE, i, 0);
			if (errFlag == -1)
			{
				fprintf(stderr, "Can't receive a message.\n");
				exit(1);
			}
			printf("Child from parent: Msg(%ld).'%s'\n",
				message.mtype, message.mtext);
			sleep(SLEEP_TIME);
			//for(j=0;j<1000000;j++); 
		}
		exit(0);
	default:
		mPid = getpid();
		printf("Parent: My PID=%d.\n", mPid);
		for (i = 1; i < ITERATION_NUM; i++){
			message.mtype = i;
			sprintf(message.mtext, "Transaction message number %05ld.", i);
			printf("Parent to child: Msg(%ld).'%s'\n",
				message.mtype, message.mtext);
			errFlag = msgsnd(mqueue, (struct msgbuf*)&message, BUF_SIZE, 0);
			if (errFlag == -1){
				fprintf(stderr, "Can't send a message.\n");
				exit(1);
			}
			sleep(SLEEP_TIME);
			//for(j=0;j<1000000;j++); 
		}
		int status;
		wait(&status);
		errFlag = msgctl(mqueue, IPC_RMID, (struct msqid_ds*)&mqueue);
		if (errFlag == -1){
			fprintf(stderr, "Can't delete the messages queue.\n");
			exit(1);
		}
		printf("Parent: messages queue has been deleted.\n");
		exit(0);
	}
}