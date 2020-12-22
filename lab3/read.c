#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

#define FILE "read.c"

typedef struct value {
        char str[20];
        pid_t pid;
} shmem_value;

int shmem_id;

int main() {
    key_t IPCkey = ftok(FILE, 1);
    if (IPCkey == -1) {
        printf("IPCkey error\n");
        exit(0);
    }

    shmem_id = shmget(IPCkey, sizeof(shmem_value), IPC_EXCL | 0666);
    if (shmem_id == -1) {
        printf("shmem id errno:%d\n", errno);
        exit(0);
    }

    shmem_value *received_value = (shmem_value *) shmat(shmem_id, NULL, 0);
    if (received_value == (shmem_value *)-1) {
        printf("SharedMemory link error\n");
        exit(0);
    }

    while(1) {
        time_t timer = time(0);
        shmem_value buff_value;
        strncpy((char *) buff_value.str, ctime(&timer), 19);
        buff_value.str[19] = '\0';
        buff_value.pid = getpid();

        printf("\ntime_Reader = %s\npid_Reader = %d\n", buff_value.str, buff_value.pid);
        printf("\ntime_Writer = %s\npid_Writer = %d\n", received_value->str, received_value->pid);
        sleep(1);
    }
    return 0;
}

