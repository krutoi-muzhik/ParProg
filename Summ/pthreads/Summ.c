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
	int left;
	int right;
	double sum;
} thread_info_t;

void *threadCount (void *arg) {
	thread_info_t *info = (thread_info_t *) arg;
	double sum = 0;

	for (int n = info->left; n <= info->right; n ++) {
		sum += 1.0 / n; 
	}

	info->sum = sum;

	// printf("id = %lu\n", info->tnum);
	// printf("left = %d\n", info->left);
	// printf("right = %d\n", info->right);
	// printf("sum = %lf\n", info->sum);

	return NULL;
}

int main (int argc, char **argv) {
	size_t nthreads, nprocs, N;
	thread_info_t *info;
	double sum;
	int err;

	nthreads = (size_t) atoi(argv[1]);
	nprocs = (size_t) get_nprocs();
	if (nthreads > nprocs)
		nthreads = nprocs;

	N = (size_t) atoi(argv[2]);

	info = (thread_info_t *) calloc(nthreads, sizeof(thread_info_t));
	for (size_t i = 0; i < nthreads; i ++) {
		info[i].tnum = i;
		info[i].nthreads = nthreads;
		info[i].left = (N / nthreads) * i + 1;
		info[i].right = (N / nthreads) * (i + 1);
		err = pthread_create(&info[i].id, NULL, threadCount, &info[i]);
		if (err < 0) {
			handle_error(err, "pthread_create");
		}
	}

	for (size_t i = 0; i < nthreads; i ++) {
		err = pthread_join(info[i].id, NULL);
		if (err < 0) {
			handle_error(err, "pthread_join");
		}
		sum += info[i].sum;
	}

	printf("Final summ = %lf\n", sum);

	exit(EXIT_SUCCESS);
}