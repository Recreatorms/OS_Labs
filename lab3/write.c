#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#define FILE "read.c"

int shm_id;

typedef struct value {
    char str[20];
    pid_t pid;
} shmem_value;

void funcExit(int sig) {
    struct shmid_ds *buf = 0;
    shmctl(shm_id, IPC_RMID, buf);
    exit(0);
}

int main() {
    signal(SIGINT, funcExit);
    key_t IPCkey = ftok(FILE, 1);

    shm_id = (shmget(IPCkey, sizeof(shmem_value), IPC_CREAT | 0666));
    if (shm_id == -1) {
        printf("Can't create shared memory\n");
        exit(0);
    }

    shmem_value *sended_value; 

    sended_value = (shmem_value *) shmat(shm_id, NULL, 0);
    if (sended_value == (shmem_value *)-1) {
        printf("shmat error\n");
	exit(0);
    }

    time_t timer = time(0);
    //time_t buf_timer = timer;

    if (sended_value->pid != 0) {
        printf("Sending process has already been started. Error.\n");
        exit(0);
    }

    while(1) {
        timer = time(0);
        //if (timer != buf_timer) {
        //    buf_timer = timer;
        //    sprintf(addr, "time_Writer = %spid_Writer = %d\n", ctime(&timer), getpid());
        //}
	shmem_value tmp_value;
	strncpy((char*) tmp_value.str, ctime(&timer), 19);
	tmp_value.str[19] = '\0';
	tmp_value.pid = getpid();
	*sended_value = tmp_value;
    }
    return 0;
}
