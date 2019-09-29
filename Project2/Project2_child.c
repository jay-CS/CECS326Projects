#include<stdio.h> 
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char *argv[]) {
    
    //if child(#) = 0, add argv[2] and argv[3]
    if(atoi(argv[4]) == 0) {
        printf("\n I am a child number %i, with pid %i, the sum is %i\n", atoi(argv[4]),getpid(), atoi(argv[2]) + atoi(argv[3]));
    }

    //if child(#) = 1, subtract argv[2] and argv[3]
    if(atoi(argv[4]) == 1) {
        printf("\n I am a child number %i, with pid %i, the sum is %i\n", atoi(argv[4]),getpid(), atoi(argv[2]) - atoi(argv[3]));
    }

    //if child(#) = 2,  multiply argv[2] and argv[3]
    if(atoi(argv[4]) == 2) {
        printf("\n I am a child number %i, with pid %i, the sum is %i\n", atoi(argv[4]),getpid(), atoi(argv[2]) * atoi(argv[3]));
    }
    
    //if child(#) = 3, divide argv[2] and argv[3]
    if(atoi(argv[4]) == 3) {
        printf("\n I am a child number %i, with pid %i, the sum is %i\n", atoi(argv[4]),getpid(), atoi(argv[2]) / atoi(argv[3]));
    }

    exit(0); //Exit the program 
}