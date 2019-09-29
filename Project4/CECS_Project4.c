#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//union semun{int val;struct semid_ds *buf;ushort *array;};

int main(int argc, char *argv[]){
    
    //Checks to see if there is valid number of inputs
	if(argc < 4){
		printf("Invalid number of Inputs, ./filename r/n NS ? ? ? ");
		exit(1);
	}
    
    //Checks to see if there are any invalid characters in the inputs
    for(int i = 2; i < argc; i++) {
        //printf("%s/n",argv[i]);
        if(strcmp(argv[i],"0") != 0) {
           if(atoi(argv[i]) == 0) {
               printf("Invalid input for value %s \n",argv[i]);
               exit(8);
           }
        }
    }

    //Checks to see if the total number of elements match
    //the amount in the 3rd arguments
    if(argc != (atoi(argv[2]) + 3)){
        printf("Invalid number of semaphore Inputs\n");
        exit(7);
    }
    
    
	int sem_id,sem_value,i,j,NS,r_true,n_true;
	key_t ipc_key;
	struct	semid_ds sem_buf;
	union semun arg;
	ipc_key = ftok(".",'S');
	r_true = (strcmp(argv[1],"r") == 0)? 1: 0;
	n_true = (strcmp(argv[1],"n") == 0)? 1: 0;
	NS = atoi(argv[2]);
	ushort sem_array[NS];

    
    //Checks to see if r or n was entered
	if((r_true||n_true) == 0){
		printf("Input 1 is neither r, nor n.\n");
		exit(2);
	}


	if (n_true == 1){
                if ((sem_id = semget(ipc_key, NS, IPC_CREAT | 0666 | IPC_EXCL)) == -1){
                        perror ("semget: IPC | 0666\n");
                         exit(3);
                 }
         }
         else if (r_true ==  1 ){
                if ((sem_id = semget(ipc_key, NS, IPC_CREAT | 0666)) == -1){
                        perror ("semget: IPC | 0666\n");
                         exit(3);
                 }
         }

	for(j = 0; j < NS;j++){
		sem_array[j] = (unsigned short) strtoul(argv[j+3], NULL,0);
	}
	printf("Semaphore identifier %d\n", sem_id);

    /* Set arg (the union) to the address of the storage location for */
    /* returned semid_ds value */
    
	arg.buf = &sem_buf;
	if(semctl(sem_id,0,IPC_STAT,arg) == -1){
		perror("semctl: IPC_STAT\n");
	exit(4);
	}

	printf("Create %s", ctime(&sem_buf.sem_ctime));

    //
	arg.array = sem_array;
	if(semctl(sem_id,0,SETALL,arg) == -1){
		perror("semctl:	SETALL\n");
		exit(5);
	}
	
	for(i = 0; i < NS; i++){
		if((sem_value = semctl(sem_id,i, GETVAL,0)) == -1){
			perror("semctl : GETVAL\n");
			exit(6);
		}
		printf("\nSemaphore %d has value of %d\n", i , sem_value);
	}
//  From lab, did not make a difference
//         if (semctl(sem_id, 0, IPC_RMID, 0) == -1)
//        {
//                 perror ("semctl: IPC_RMID\n");
//                 exit(5);
//        }
	exit(0);

}
