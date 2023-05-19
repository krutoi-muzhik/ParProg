#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <stdatomic.h>

#define handle_error(err, msg)	\
	errno = err;				\
	perror(msg);				\
	exit(EXIT_FAILURE)

typedef struct {
	int n;
	pthread_mutex_t mutex;
} pthread_mem_t;

typedef struct {
	size_t num;
	pthread_t id;
	size_t nthreads;
	pthread_mem_t *mem;
} pthread_info_t;

void *threadOper (void *arg) {
	pthread_info_t *info = (pthread_info_t *) arg;
	
	pthread_mutex_lock(&info->mem->mutex);
	if (info->mem->n != info->num) {
		pthread_mutex_unlock(&info->mem->mutex);
		return threadOper(arg);
	}
	info->mem->n ++;
	printf("id = %lu\n", info->num);
	printf("n = %d\n", info->mem->n);
	pthread_mutex_unlock(&info->mem->mutex);

	return NULL;
}

int main (int argc, char **argv) {
	size_t nprocs, nthreads;
	pthread_mem_t *mem;
	pthread_info_t *info;
	int err;

	nthreads = (size_t) atoi(argv[1]);
	nprocs = (size_t) get_nprocs();

	mem = (pthread_mem_t *) calloc(1, sizeof(pthread_mem_t));
	info = (pthread_info_t *) calloc(nthreads, sizeof(pthread_info_t));

	err = pthread_mutex_init(&mem->mutex, NULL);
	if (err) {
		handle_error(err, "pthread_mutex_init");
	}

	for (size_t i = 0; i < nthreads; i ++) {
		info[i].mem = mem;
		info[i].num = i;
		info[i].nthreads = nthreads;
		err = pthread_create(&info[i].id, NULL, threadOper, &info[i]);
		if (err < 0) {
			handle_error(err, "pthread_create");
		}
	}

	for (size_t i = 0; i < nthreads; i ++) {
		pthread_join(info[i].id, NULL);
	}

	printf("nthreads = %lu\n", nthreads);
	printf("Final n = %d\n", mem->n);

	err = pthread_mutex_destroy(&mem->mutex);
	if (err) {
		handle_error(err, "pthread_mutex_destroy");
	}

	exit(EXIT_SUCCESS);
}
