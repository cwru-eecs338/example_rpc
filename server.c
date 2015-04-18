/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "semaphore.h"
#include "common.h"

struct charm *
next_charm_1_svc(next_charm_args *argp, struct svc_req *rqstp)
{
	static struct charm produce_charm;

	/*
	 * insert server code here
	 */
	produce_charm = (argp->charm_arg)[argp->index_arg];
	printf("Producing: ");
	print_charm(&produce_charm);
	printf("\n");
	fflush(0);
	
	return &produce_charm;
}

void *
read_charm_1_svc(charm *argp, struct svc_req *rqstp)
{
	static char * result;

	/*
	 * insert server code here
	 */
	printf("    Consuming: ");
	print_charm(argp);
	printf("\n");
	fflush(0);

	return (void *) &result;
}
