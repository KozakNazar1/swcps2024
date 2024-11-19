#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

void main(void){
	int i;
	int errFlag;
	char arg1[5];
	int	status;

	printf("Parent: My PID=%d.\n", getpid());
	int shMemId = shmget(IPC_PRIVATE, 4096, 0xfff);
	if (shMemId == -1){
		fprintf(stderr, "Can't allocate the shared memory segment.\n");
		exit(1);
	}
	printf("Parent: shared memory segment has been allocated (4Kb=1Page).\n");
	char* shMemPtr = 0;
	shMemPtr = shmat(shMemId, shMemPtr, 0);
	if ((long)shMemPtr == -1){
		fprintf(stderr, "Can't attach the shared memory segment.\n");
		exit(1);
	}
	printf("Parent: shared memory segment has been attached at 0x%08lX.\n",
		(unsigned long)shMemPtr);
	printf("Parent: putting 100 chars to the attached segment:\n");
	for (i = 0; i < 100; i++){
		*(shMemPtr + i) = i;
		printf("%d", *(shMemPtr + i));
	}
	printf("\n");
	int cPid = fork();
	switch (cPid){
	case -1:
		fprintf(stderr, "Can't fork for the child.\n");
		exit(1);
	case 0:
		sprintf(arg1, "%d", shMemId);
		errFlag = execl("./shmat_example__child", "shmat_example__child", arg1, (char*)0);
		if (errFlag == -1){
			fprintf(stderr, "Can't execute the external child.\n");
			exit(1);
		}
	default:
		wait(&status);
		errFlag = shmdt(shMemPtr);
		if (errFlag == -1){
			fprintf(stderr, "Can't detach the shared memory segment.\n");
			exit(1);
		}
		printf("Parent: shared memory segment has been detached.\n");
		errFlag = shmctl(shMemId, IPC_RMID, (struct shmid_ds*)&shMemId);
		if (errFlag == -1){
			fprintf(stderr, "Can't delete the shared memory segment.\n");
			exit(1);
		}
		printf("Parent: shared memory segment has been destroyed.\n");
		exit(0);
	}
}