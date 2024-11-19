#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

void main(int argc, char* argv[]){
	int i;
	char* shMemPtr = 0;
	if (argc != 2){
		fprintf(stderr, "Missing argument was detected.\n");
		exit(1);
	}
	int shMemId = atoi(argv[1]);
	int mPid = getpid();
	printf("Child: My PID = %d.\n", mPid);
	shMemPtr = shmat(shMemId, shMemPtr, 0);
	if ((long)shMemPtr == -1){
		fprintf(stderr, "Can't attach the shared memory segment.\n");
		exit(1);
	}
	printf("Child: shared memory segment has been attached at 0x%08lX.\n",
		(unsigned long)shMemPtr);
	printf("Child: getting 100 chars from the attached segment:\n");
	for (i = 0; i < 100; i++){
		printf("%d", *(shMemPtr + i));
	}
	printf("\n");
	int errFlag = shmdt(shMemPtr);
	if (errFlag == -1){
		fprintf(stderr, "Can't detach the shared memory segment.\n");
		exit(1);
	}
	printf("Child: shared memory segment has been detached.\n");
	exit(0);
}