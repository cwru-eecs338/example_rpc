#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "common.h"
#include "main.h"

// This function contains all the code necessary to make the RPC call.
// This is based off of code from the "semaphore_client.c" file that is created
// from calling "rpcgen -a semaphore.x" 
void rpc_read_charm(char *host, charm consume_charm)
{
	CLIENT *clnt;
	void  *result;

	clnt = clnt_create (host, DISPLAY_PRG, DISPLAY_VER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	result = read_charm_1(&consume_charm, clnt);
	if (result == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	clnt_destroy (clnt);
}

void *consumer(void *shared_data) {
	
	struct shared_data_info *shared = (struct shared_data_info *)shared_data;
    int nextc = 0;

    // Consume every delicious
    // marshmallow
    int c;
    for(c = 0; c < CHARMS; c++) {

        // wait(full)
        if(sem_wait(shared->full) == -1) {
            perror("wait(full)");
            pthread_exit(NULL);
        }
        
        // wait(mutex)
        if(sem_wait(shared->mutex) == -1) {
            perror("wait(mutex)");
            pthread_exit(NULL);
        }

        /********************CRITICAL SECTION BEGIN********************/
        
        // Shared memory access
        struct charm consume_charm = (shared->charm_buf)[nextc];
		rpc_read_charm(shared->host, consume_charm); // <-- RPC call "read_charm_1()"
        nextc = (nextc + 1) % shared->BUF_SIZE;
        printf("    Consuming: ");
        print_charm(&consume_charm);
        printf("\n");
        fflush(0);

        /*********************CRITICAL SECTION END*********************/

        // signal(mutex)
        if(sem_post(shared->mutex) == -1) {
            perror("signal(mutex)");
            pthread_exit(NULL);
        }

        // signal(empty)
        if(sem_post(shared->empty) == -1) {
            perror("signal(empty)");
            pthread_exit(NULL);
        }

    }
    
    pthread_exit(NULL);
}
