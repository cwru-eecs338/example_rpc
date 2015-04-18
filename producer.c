#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common.h"
#include "main.h"

// This function contains all the code necessary to make the RPC call.
// This is based off of code from the "semaphore_server.c" file that is created
// from calling "rpcgen -a semaphore.x" 
struct charm *rpc_next_charm(char *host, int c)
{
	CLIENT *clnt;
	struct charm  *result;
	next_charm_args *arg;
	arg = (next_charm_args *) malloc(sizeof(next_charm_args));
	arg->index_arg = c;
	memcpy(arg->charm_arg, LUCKY_CHARMS, CHARMS*sizeof(struct charm));
	
	clnt = clnt_create (host, DISPLAY_PRG, DISPLAY_VER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	result = next_charm_1(arg, clnt);
	if (result == (struct charm *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	clnt_destroy (clnt);
	
	return result;
}

void *producer(void *shared_data) {
	
	struct shared_data_info *shared = (struct shared_data_info *)shared_data;
    int nextp = 0;

    // Produce the charms
    int c;
    for(c = 0; c < CHARMS; c++) {

        // wait(empty)
        if(sem_wait(shared->empty) == -1) {
            perror("wait(empty)");
            pthread_exit(NULL);
        }
        
        // wait(mutex)
        if(sem_wait(shared->mutex) == -1) {
            perror("wait(mutex)");
            pthread_exit(NULL);
        }

        /********************CRITICAL SECTION BEGIN********************/
        
        struct charm *produce_charm = rpc_next_charm(shared->host, c); // <-- RPC call "next_charm_1()"
        printf("Producing: ");
        print_charm(produce_charm);
        printf("\n");
		fflush(0);
        shared->charm_buf[nextp] = *produce_charm; // Shared memory access
        nextp = (nextp + 1) % shared->BUF_SIZE;

        /*********************CRITICAL SECTION END*********************/

        // signal(mutex)
        if(sem_post(shared->mutex) == -1) {
            perror("signal(mutex)");
            pthread_exit(NULL);
        }

        // signal(full)
        if(sem_post(shared->full) == -1) {
            perror("signal(full)");
            pthread_exit(NULL);
        }

    }

    pthread_exit(NULL);
}
