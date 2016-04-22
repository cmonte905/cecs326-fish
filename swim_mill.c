#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>

#define SHM_SIZE 100

int killMem = 0;

void ctrlC(int s) {
	printf("Control c was pressed\n");

	if(s == SIGINT) {
		shmctl(killMem, IPC_RMID, 0);
		kill(0, SIGTERM);
	}
}

int main(int argc, char *arcv[]) {
	signal(SIGINT,ctrlC);

	int pid, sharedMem, random;
	key_t k;
	char *attached;
	char (*mill) [10][10];
	int x =0, y, i, pelletNum = 18;
	k = 5678;

	//Creating shared memory 
	if((sharedMem = shmget(k, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}
	if((attached = (char *) shmat(sharedMem, NULL, 0)) == (char *) - 1 ) {
		perror("shmat");
		exit(1);
	}
	//creates the array for shared memory
	while (x < 100){
		*attached = '_';
		x++;
		attached++;
	}

	pid_t fishID = fork();
	if(fishID) {
		execl("fish", "fish", NULL);
	}
	else if (fishID == 0) {
		wait(NULL);
	}
	else {
		printf("Forking error of fish process\n");
		exit(1);
	}
	
	int count = 0;
	while(count < 30) {
		sleep(1);
		pid_t pelletID = fork();
		
		if(pelletID < 0) {
			perror("Forking Error with pellet process\n");
			exit(1);
		}
		if (pelletID != 0) {
			
		}
		else {
			sleep(1);
			execl("pellet" , "pellet", NULL);
		}
		//
		
		count++;
	}

	shmdt(attached);
	shmctl(sharedMem, IPC_RMID, 0); // clear memory
    kill(0, SIGTERM);

	exit(0);
	return 0;
}
