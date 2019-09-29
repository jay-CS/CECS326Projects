# include <stdio.h> 
# include <sys/types.h> 
# include <sys/ipc.h> 
# include <sys/sem.h> 
# include <unistd.h> 
# include <stdlib.h> 
# include <time.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>

/* Not needed on MAC OS */
// union semun {
// 	int val;
// 	struct sem_id_ds *buf;
// 	ushort *array;
// };


void set_sembuf_struct(struct sembuf *s, int num, int op, int flg) 
 { 
 	s->sem_num = (short) num; 
 	s->sem_op = op; 
	s->sem_flg = flg; 
 	return; 
 }; 
 

int main(int argc, char *argv[]) 
{ 
	pid_t pid, ppid, childpid, w;                 		 // Current process, Parent Process, and Child Process ID, and wait status 
	pid_t parent = getpid();							 // Used to make sure the child process finishes before the parent is killed
	int i, j, sem_opt;				 					 // loop counter variables, and sempaphore protection option
	int sem_id, sem_ret, sem_value; 					 // Semaphore ID and value
	int children, status, delay;						 // Number of children, child status, and the delay 
	char buf_num[8192], *c_ptr;					 		 // Character buffer and pointer 
	key_t ipc_key; 										 // Key
	struct semid_ds sem_buf;							 	

	/* Check for correct amount of arguments */
	if (argc != 4)
	{
		printf("Invalid input.  The number of arguments must be 4.\n");
		exit(1);
	}

	/* Check to see if  second input paramesters are valid "s" or "n"*/
	/* No semaphore protection */		
	if ((strcmp(argv[2], "n")) == 0 || strcmp(argv[2],"N") == 0) 
	{
		sem_opt = 0;
	}
	/* Semaphore protection */
	else if ((strcmp(argv[2], "s")) == 0 || (strcmp(argv[2],"S") == 0)) 
 	{
 		sem_opt = 1;
 	}
 	else
	{
		printf("Invalid input for the second argument. %s should be 'n' or 's'.\n", argv[1]);
		exit(1);
	}

	/* Make sure that the child value and delay value are both valid and reasonable integers */
	if( (sscanf(argv[1], "%i", &children)) != 1 || (sscanf(argv[3], "%i", &delay) != 1) )
  	{
		printf("Error: children and delay arguments must be integers\n");
		exit(1);
  	}
  	if(children < 1 || children > 10)
  	{
  		printf("Error: number of children must between 1 and 10\n");
		exit(1);
  	}
  	if(delay < 1 || delay > 20000)
  	{
  		printf("Error: Delay should be between 1 and 300\n");
		exit(1);
	}


	/* Declare semaphore wait and signal, P(V), S(V) and intialize Semaphore elements*/
	struct sembuf semwait[1]; 
 	struct sembuf semsignal[1];
 
 	set_sembuf_struct(semwait, 0, -1, 0); 
 	set_sembuf_struct(semsignal, 0, 1, 0); 

	/*Generate random key from pathname*/
	ipc_key = ftok(".", 'S'); 			
	delay = atoi(argv[3]);
	
	/* Create semaphore unless it already exist*/
	if ((sem_id = semget(ipc_key, 1, IPC_CREAT | IPC_EXCL | 0666)) == -1) 
	{
        perror ("semget: IPC | 0666");
        exit(1);
	}

	/* Increment the semaphore */
	/* Else remove the sempahore  */
	if(semop(sem_id, semsignal, 1) == -1) 
	{
	    printf("%ld: semaphore increment failed - %s\n", (long)getpid(), strerror(errno)); 
	        if (semctl(sem_id, 0, IPC_RMID) == -1) 
	            printf ("%ld: could not delete semaphore - %s\n", (long)getpid(), strerror(errno)); 
	            exit(1); 
	}
	
	/* Create children */
	childpid = 0;
	for(i = 1; i < children; i++)
	{
		if(childpid = fork()) break; 
	}

	/* Decrement Sempahore to allow access into the CS*/
	if(sem_opt)
	{
		while (( (sem_ret = semop(sem_id, semwait, 1) ) == -1) && (errno ==EINTR)); 
		    if (sem_ret == -1) 
		        printf ("%ld: semaphore decrement failed - %s\n", (long)getpid(), strerror(errno)); 
	}



	/* Place the message inside the buffer */
	sprintf(buf_num,"i: %d: process ID: %6ld parent ID: %6ld child ID: %6ld\n",i,(long)getpid(), (long)getppid(), (long)childpid);
	c_ptr = buf_num;
	setbuf(stdout, NULL);

	/* Go through buffer and place each character in the stream */
	/* usleep() sleeps processmfor 'N' amount of microseconds"*/
	while (*c_ptr != '\0')
	{
		fputc(*c_ptr, stderr);
		usleep(delay);
		c_ptr++;
	}

	/* Increment semaphore to protect CS*/ 
	if(sem_opt)
	{
		while ( ( (sem_ret = semop(sem_id, semsignal, 1) ) == -1) && (errno == EINTR) ); 
			if (sem_ret == -1) 
			    printf ("%ld: semaphore increment failed - %s\n", (long)getpid(), strerror(errno));
	}

	/* Wait for the children to finish executing */
	waitpid(childpid, &status, 0);

	/* Remove seamphore after parent process finishes*/
	if(parent == getpid())
	{
        if(semctl(sem_id, 0, IPC_RMID) == -1)
        {
            printf("%ld: couldn't delete semaphore - %s\n", (long)getpid(), strerror(errno));
            exit(1);
        }
    }
  exit(0);
} 
