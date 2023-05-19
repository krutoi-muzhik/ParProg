#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <errno.h>

#define handle_error(err, msg)	\
	errno = err;				\
	perror(msg);				\
	exit(EXIT_FAILURE)

typedef struct {
	pthread_t id;
	size_t nthreads;
	size_t tnum;
} thread_info_t;

static void *threadOper (void *arg) {
	thread_info_t *info = (thread_info_t *) arg;
	printf ("Thread %lu Total threads %lu\n", info->tnum, info->nthreads);
	return NULL;
}

int main (int argc, char **argv) {
	size_t nthreads, nprocs;
	int err;
	thread_info_t *info;


	nthreads = (size_t) atoi(argv[1]);
	nprocs = (size_t) get_nprocs();
	if (nthreads > nprocs)
		nthreads = nprocs;

	info = (thread_info_t *) calloc(nthreads, sizeof(thread_info_t));

	for (size_t i = 0; i < nthreads; i ++) {
		info[i].tnum = i;
		info[i].nthreads = nthreads;
		err = pthread_create(&info[i].id, NULL, threadOper, &info[i]);
		if (err) {
			handle_error(err, "pthread_create");
		}
	}


	for (size_t i = 0; i < nthreads; i ++) {
		err = pthread_join(info[i].id, NULL);
		if (err) {
			handle_error(err, "pthread_join");
		}
	}


	free(info);
	exit(EXIT_SUCCESS);
}
