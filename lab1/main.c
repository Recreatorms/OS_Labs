#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void getAtexitPid() {
	printf("Started atexit\n Process id: \t\t%x\n Parent process id: \t%x\n\n",
			getpid(),getppid());
}


void forkProcess(pid_t pid) {
    switch(pid) {
        case -1: 
            perror("Fork error\n");
            break;
        case 0:
            printf("Child:\n");
	    pid_t child_pid = getpid();
            pid_t parent_pid = getppid();
            printf("Child PID: \t\t%x\nParent PID:\t\t%x\n\n", child_pid, parent_pid);
            break;
        default:
	    printf("\nNew PID: \t\t%x\n", pid);
            printf("\nParent:\nPID:\t\t\t%x\nChild PID:\t\t%x\n\n", getpid(), pid);
            break;
    }
}

int main() {
    printf("Started main\n PID in main: \t\t%x\n PPID in main: \t\t%x\n", getpid(), getppid());
    if (atexit(getAtexitPid)) 
	    perror("error"); 
    pid_t pid = fork();
    forkProcess(pid);
    return 0;
}
