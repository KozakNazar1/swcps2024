#include <unistd.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#define SLEEP_TIME 5
#define ITERATION_NUM 8

int sig = 0;

void interrupt(){
	sig = 1;
	return;
}

void main(void){
	int i;
	void (*prev_handler)(int);
#define errFlag prev_handler
	char answer;
	void(*f_ptr)(int) = &interrupt;
	errFlag = signal(SIGINT, f_ptr);
	if (errFlag == SIG_ERR)
	{
		fprintf(stderr, "Error creating signal handler.\n");
		exit(1);
	}
	int j = 0;
	
	while (1){
		printf("Running loop job... %d.\n", j);	
		j++;	
		sleep(SLEEP_TIME);	
		//for(i=0;i<1000000;i++)
	
		if (sig){		
			printf("Interrupt from console was detected.\n");		
			printf("Terminate the program?(Y)");		
			errFlag = signal(SIGINT, SIG_IGN);
		
			if (errFlag == SIG_ERR){
				fprintf(stderr, "Error deactivating signal handler.\n");
				exit(1);				
			}
		
			errFlag = signal(SIGINT, SIG_DFL);		
			if (errFlag == SIG_ERR){			
				fprintf(stderr, "Error restoring default signal handler.\n");			
				exit(1);		
			}
		
			answer = getchar();		
			if ((answer == 'y') || (answer == 'Y')){
				break;		
			}		
			else{ 
				errFlag = signal(SIGINT, f_ptr); 
				if (errFlag == SIG_ERR){
					fprintf(stderr, "Error creating signal handler.\n");
					exit(1);
				}
				sig = 0; 
			}
 
			(void)getchar();
		}
 
		if (sig){
			break;
		}
 
		if (j > ITERATION_NUM){
			printf("Automatic simulation of the console interrupt signal.\n");
			int mPid = getpid();
			printf("SIGINT(2) has been sent to PID %d.\n", mPid);	 
			kill(mPid, SIGINT);
		}
	}

#undef errFlag 
 
	exit(0);
}
