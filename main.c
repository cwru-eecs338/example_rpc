#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "producer.h"
#include "consumer.h"
#include "common.h"

void initialize_counts();

// Max number of "charms" in "bin"
static const int BUF_SIZE = 3;

// Semaphores
sem_t mutex, empty, full;

//Shared Memory
struct charm *charm_buf;
struct shared_data_info shared;

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("USAGE: ./semaphore <RPC SERVER HOSTNAME>\n");
		return 0;
	}
	pthread_t producerThread;
	pthread_t consumerThread;
	pthread_attr_t attr;
	void *status;

	//Creating a set of attributes to send to the threads
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// Create shared memory segment
    // With size "bin size" * "charm size"
	charm_buf = (struct charm *)malloc(BUF_SIZE*sizeof(struct charm));
	
	//Initializes semaphores. See below
    initialize_counts();
	
	// Setup data to be shared between parent (this) thread and
    // child (producer and consumer) threads. There will only be one copy of
	// this struct. So each thread will receive a pointer to the "same copy".
	struct shared_data_info shared = {
        .BUF_SIZE = BUF_SIZE, 
        .mutex = &mutex,
        .empty = &empty,
        .full = &full,
		.host = argv[1],
		.charm_buf = charm_buf
    };
	
    // Fork producer
	if(pthread_create(&producerThread, &attr, producer, (void *)&shared) == -1) {
		perror("pthread_create(producerThread)");
		exit(EXIT_FAILURE);
	}

    // Fork consumer
	if(pthread_create(&consumerThread, &attr, consumer, (void *)&shared) == -1) {
		perror("pthread_create(producer)");
		exit(EXIT_FAILURE);
	}
	
	// Wait for children
	if(pthread_join(producerThread, &status) == -1) {
		perror("pthread_join(producerThread)");
		exit(EXIT_FAILURE);
	}

	if(pthread_join(consumerThread, &status) == -1) {
		perror("pthread_join(producerThread)");
		exit(EXIT_FAILURE);
	}

    // Should never reach this statement, but...
    return EXIT_SUCCESS;
}

// Posix semaphore initialization is a simple call to sem_init()
void initialize_counts() {
    if(sem_init(&mutex, 0, 1) < 0) {
		perror("sem_init(&mutex)");
		exit(EXIT_FAILURE);
	}
	if(sem_init(&empty, 0 , BUF_SIZE) < 0) {
		perror("sem_init(&empty)");
		exit(EXIT_FAILURE);
	}
	if(sem_init(&full, 0 , 0) < 0) {
		perror("sem_init(&full)");
		exit(EXIT_FAILURE);
	}
}
