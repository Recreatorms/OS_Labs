#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#define THREAD_COUNT 10
pthread_mutex_t mutex;

int count = 0;
//bool active = true;
void *readThread(void *num) {
	while (1) {
		pthread_mutex_lock(&mutex);
		printf("Thread #%d | TID =  %lx | Count = %d\n", 
					*(int *)num, (long)pthread_self(), count);
		pthread_mutex_unlock(&mutex);    
		if (count == THREAD_COUNT)
			pthread_exit(0);	    
		sleep(1);
	}
}

void *writeThread(void *arg) {
	while (count < THREAD_COUNT) {
		pthread_mutex_lock(&mutex);
		count++;
		printf("Count = %d\n", count);
		pthread_mutex_unlock(&mutex);
		sleep(1);  
	}
	pthread_exit(0);
}

int main() {
	pthread_t threads[THREAD_COUNT + 1];
	int threadArr[THREAD_COUNT];
	pthread_mutex_init(&mutex, NULL);

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
