#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 10

pthread_cond_t condition;
pthread_mutex_t mutex;
int count = 0;

void *readThread(void *num) {
        while (1) {
		pthread_mutex_lock(&mutex);
        	pthread_cond_wait(&condition, &mutex);
        	printf("Thread #%d | TID = %lx | Count = %d\n",
                        	*(int *)num, (long)pthread_self(), count);

		sleep(1);	
		pthread_mutex_unlock(&mutex);
		
      		if (count == THREAD_COUNT) 
			pthread_exit(0);
	} 
}

void *writeThread(void *arg) {
       	while (count < THREAD_COUNT) {
		pthread_mutex_lock(&mutex);
       		count++; 
		printf("Count = %d\n", count);
		pthread_cond_broadcast(&condition);
       		sleep(1);
		pthread_mutex_unlock(&mutex);
       	}
	pthread_exit(0);
}

int main() { 
	pthread_t threads[THREAD_COUNT + 1]; 
	int threadArr[THREAD_COUNT];
	pthread_mutex_init(&mutex, NULL);
       	pthread_cond_init(&condition, NULL);

	for (int i = 0; i < THREAD_COUNT; i++) {
	       	threadArr[i] = i;
		pthread_create(&threads[i], NULL, readThread, &threadArr[i]);
       	}

        pthread_create(&threads[THREAD_COUNT], NULL, writeThread, NULL);

        for (int i = 0; i < THREAD_COUNT + 1; i++)
            	pthread_join(threads[i], NULL);
	pthread_mutex_destroy(&mutex);
        printf("end\n");
	return 0;
}