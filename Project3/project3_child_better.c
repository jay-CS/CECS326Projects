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

	pid_t pid, display, w;
        int ret_value, num_tries, sleeptime, i, status, child_number,fd, count = 0;
        char *fname;
	char *lockfile;

        child_number  = atoi(argv[1]);//child number that apparentlly does nothing
        fname = argv[2];//filename of file to be read, no to be confused with lock file name
	lockfile = "lock1";
        num_tries = atoi(argv[3]);
        sleeptime = atoi(argv[4]);
        pid = getpid();
        srand((unsigned) pid);//changes how rand function works
	ret_value = (int)(pid%256);//lower eight bytes

        
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*				creating lockfile							     */
	sleep(rand()%sleeptime);
	while(((fd = creat(lockfile,0)) == -1) && (errno == EACCES))//{
                if(++count < num_tries) sleep(rand()%sleeptime);
                else{
                printf("\n Unable to generate. \n"); kill(getpid(),child_number);
                }
        //}
		close(fd); printf("\nFile %s has been created \n", lockfile);
		
                
                

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////







	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*				displaying text.dat							     */
	if ((display = fork()) == 0){execlp("/bin/cat", "/bin/cat", fname, (char*)0);printf("\nError in bin/cat exec in child\n");}

                 while ((w=wait(&status)) && w != - 1)
                {
                            if(w != -1) {
                        }
                }
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////






	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*				releasing lockfile							     */
	count = 0; //reseting count
	while (unlink(lockfile) != 0){
                if(++count < num_tries)
                        sleep(sleeptime);
                else
                {
				printf("\nCannot release file\n");
                                exit(-1);
                }
        }
        printf("\nFile is released\n");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

        
	exit(ret_value);//255 is 8bits, so anding it with pid retains lower 8 bits on
	
}
