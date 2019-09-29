# include <stdio.h> 
# include <stdlib.h> 
# include <errno.h>
# include <fcntl.h>
# include <sys/types.h> 
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>

int main(int argc, char *argv[])
{	
	if(argc !=4){ printf("Invalid number of inputs\n"); exit(0);}


        pid_t pid, w;// pid of parent/child which will cause the program to eventually exec or continue
        int k, status, num_tries, sleeptime;//child number,status,number of tries to acquire and release, maximum amount of sleeptime possible
        char *fname;//filename
        char child_number[3];//child number represented as a char array
        fname = argv[1];//filename input
        num_tries = atoi(argv[2]);//number of tries input
        sleeptime = atoi(argv[3]);//max sleeptime input
	
	/*			input	validation			*/
        if(argc !=4){ printf("Invalid number of inputs\n"); exit(0);}
	if(num_tries <= 0){ printf("Number tries needs to be a positive integer greater than 0\n"); exit(0);}
	if(sleeptime <= 0){ printf("Sleeptime needs to be a positive integer greater than 0\n"); exit(0);}
           


        unlink("lock1");//destroy any leftover lock1 files that may have been left from previous runs.

        for (k=0;k<3;++k){//create three children

          if((pid = fork()) == 0){//if child exec else continue
            sprintf(child_number, "%d",k);
								  //fname   numtries sleeptime NULL
            execl("project3_child","project3_child", child_number, argv[1], argv[2], argv[3], (char *)0);
          }
          else{
            printf("Child %d was forked \n",pid);
        }

	}
        while ((w=wait(&status)) && w != - 1){//wait for children to die.
            if(w != -1){
              printf("Wait on PID: %d returns status of: %04X\n",w,status);
	     }
	  }

        exit(0);
}
