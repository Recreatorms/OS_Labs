#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int fd[2]; 

    if (pipe(fd) == -1) {
        perror("Failed pipe open");
        exit(0);
    }
    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("Fork failed");
            exit(0); 
        case 0:
            // child 
	    close(fd[1]); 
            sleep(5);

            long int child_pipe_buf[2];
            time_t timer = time(NULL);
            printf("Child time: \t%s",
                   ctime(&timer));

            read(fd[0], &child_pipe_buf[0], sizeof(long int));
	    read(fd[0], &child_pipe_buf[1], sizeof(long int));
           
	    printf("Parent time: \t%s", ctime(&child_pipe_buf[0]));
            printf("Parent pid: %lu\n", child_pipe_buf[1]);

            close(fd[0]);
            exit(0);
        default:
            //parent 
	    close(fd[0]);

            long int parent_pipe_buf[2];
            parent_pipe_buf[0] = time(NULL);
            parent_pipe_buf[1] = getpid();

            write(fd[1], parent_pipe_buf, sizeof(parent_pipe_buf));
            close(fd[1]);

            if (waitpid(pid, 0, 0) == -1) {
                perror("waitpid");
                exit(0);
            }
    }
    return 0;
}
