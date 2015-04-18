#ifndef SEMAPHORE_COMMON
#define SEMAPHORE_COMMON

#include <semaphore.h>
#include "main.h"

// Holds information about shared data
// needed by producer and consumer
struct shared_data_info {
    int BUF_SIZE; // Size of shared buffer (in charms)
    sem_t *mutex;    // 'mutex' semaphore
    sem_t *empty;    // 'empty' semaphore
    sem_t *full;     // 'full' semaphore
	char *host;
	struct charm *charm_buf; // shared buffer
};

// This is now declared in semaphore.h, which is
// generated from rpcgen semaphore.x
/*
struct charm {
    enum COLOR
    {
        PINK = 0,
        YELLOW,
        ORANGE,
        GREEN,
        BLUE,
        PURPLE,
        RED
    } color;
    char shape[32];
};
*/

#define CHARMS 7
extern struct charm LUCKY_CHARMS[];

void print_charm(struct charm *);
#endif
