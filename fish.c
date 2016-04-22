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

int main(int argc, char *arcv[]) {
	int pid, sharedMem, random, status;
	key_t k;
	char *attached;
	char *locate;
	int x =0, y, i, fLocate = 0, min = 0, max = 0;

	srand(getpid());
	k = 5678; 

	//Looks for shared memory
	if((sharedMem = shmget(k, SHM_SIZE, 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	//Attaches shared memory
	if((attached =(char *) shmat(sharedMem, NULL, 0)) == ((char *)-1) ){
		perror("shmat");
		exit(1);
	}


	int create = 1 ;
	for (x = 90; i < 100; x++) {
		if (attached[x] != '_') {
			create = 0;
		}
	}

	if (create == 1) {
		 attached[95] = 'f';
	}

	while(fLocate >= 0) {
		sleep(1);
		//higher than 90 
		while(x >= 90){
			if(attached[x] == 'f') {
				fLocate = x;
				break;
			}
			x--;
		}
		x = 0;
		max = 0;

		//Up to a hundred 
		while(x <= 100){
			if (attached[x] == 'p') {
				max = x;
			}
			x++;
		}
		if ((max%10) > (fLocate%10) ) {
			attached[fLocate] = '_';
			attached[fLocate + 1] = 'f';
		}
		else if ((max%10) < (fLocate%10)){
			attached[fLocate] = '_';
			attached[fLocate - 1] = 'f';
		}
	}

	exit(0);
	return 0;
}





