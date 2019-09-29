#include<stdio.h> 
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char *argv[]) {

    int i; // variable to go through for loop 
    pid_t pid_1; //hold PID of children in progress
    char current[3]; 

    for( int i = 0; i < 4; i++) {
        sprintf(current, "%d",i);
        printf("\n%s\n",current);

        char* args[] = {"./Project2_child", argv[1], argv[2], argv[3], current, NULL};
        pid_1 = fork();

        if(pid_1 == 1) {
            printf("\nForked Child %i\n",getpid());
            sleep(random()%(atoi(argv[1])));
            execv(args[0], args); //Should not come back to child program
            printf("Error");
        }
    }
    for(int i = 0; i < 4; i++) {
        wait(NULL);
    }
    exit(0);
}


