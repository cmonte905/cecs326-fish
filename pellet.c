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

#define SHM_SIZE 100

int main(int argc, char const *argv[]) {
	int pid, sharedMem,random, status;
	key_t k;
	char *attached;
	char *locate;
	int x = 0, y , i;
	srand(getpid());
	k = 5678;

	if((sharedMem = shmget(k, SHM_SIZE,0666)) < 0) {
		perror("shmget");
		exit(1);
	}
	if( (attached = (char *) shmat(sharedMem, NULL, 0)) == (char *) - 1 ) {
		perror("shmat");
		exit(1);
	}

	x = 0;
	int randNum = rand()%90;
	
	attached[randNum] = 'p';
	int fLocate = randNum;

	while(fLocate <= 90){
		sleep(1);
		attached[fLocate] = '_';
		if((fLocate + 10) >= 90) { //Checks if it is in the last row
			if(attached[fLocate + 10] == 'f') {
				printf("Pellet process %d has been eaten at location (9,%d)\n",getpid(), (fLocate+10) %10);
			}
			else {
				printf("Pellet process %d has left the stream at (9,%d)\n",getpid(), (fLocate+10) %10);
			}
		}
		else {
			attached[fLocate] = 'p';
		}
		fLocate += 10;
	}

	exit(0);
	return 0;
}