#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    unlink("fifo");

    if (mkfifo("fifo", 0666) == -1) {
        perror("FIFO create failure");
        exit(0);
    }

    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("Fork failure");
            exit(0);
        case 0:
            // Child
            ;
	    int fd_fifo = open("fifo", O_RDONLY);
            if (fd_fifo == -1) {
                fprintf(stderr, "FIFO open failure\n");
                exit(0);
            }

            long int child_pipe_buf[2];
            if (read(fd_fifo, &child_pipe_buf[0], sizeof(long int)) == -1 ||
                read(fd_fifo, &child_pipe_buf[1], sizeof(long int)) == -1) {
                fprintf(stderr, "fifo read failure\n");
                exit(0);
            }

            sleep(5);
            time_t timer = time(0);
            printf("Child time:\t%s", ctime(&timer));
            printf("Parent time:\t%s", ctime(&child_pipe_buf[0]));
	    
            printf("Parent pid: %lu\n", child_pipe_buf[1]);

            close(fd_fifo);
            exit(0);
        default:
            // Parent
            ;
	    fd_fifo = open("fifo", O_WRONLY);
            if (fd_fifo == -1) {
                fprintf(stderr, "FIFO open failure\n");
                exit(0);
            }

            long int parent_pipe_buf[2];
            parent_pipe_buf[0] = time(NULL);
            parent_pipe_buf[1] = getpid();

            write(fd_fifo, parent_pipe_buf, sizeof(parent_pipe_buf));
            close(fd_fifo);

            if (waitpid(pid, 0, 0) == -1) {
                perror("waitpid");
                exit(0);
            }
    }

    return 0;
}
