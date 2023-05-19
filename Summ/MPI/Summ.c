#include "Summ.h"

int main (int argc, char **argv) {
	int rank, commsize, N, extra, left, right;
	double time, tmp, res = 0;
	MPI_Status status;

	if (argc != 2) {
		printf("Enter number of iterations\n");
		exit(EXIT_FAILURE);
	} else {
		sscanf(argv[1], "%d", &N);
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &commsize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	extra = N % commsize;

	if ((extra) && (rank >= extra)) {
		left = N / commsize * rank + rank - extra + 1;
		right = N / commsize * (rank + 1) + rank - extra + 1;
	}
	else {
		left = N / commsize * rank + 1;
		right = N / commsize * (rank + 1);
	}

	time = MPI_Wtime();

	res = CountSum(left, right);
	// printf("rank = %d\n", rank);
	// printf("res = %lf\n", res);

	if (rank) {
		MPI_Send(&res, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
	} else {
		for (int i = 1; i < commsize; i++) {
			MPI_Recv(&tmp, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			// printf("i = %d\n", i);
			// printf("tmp = %lf\n", tmp);
			res += tmp;
		}
	}

	time = MPI_Wtime() - time;;

	if (!rank) {
		printf ("Summ = %lf\n", res);
		printf ("time = %lf\n", time);
	}

	MPI_Finalize();

	exit(EXIT_SUCCESS);
}

double CountSum (int left, int right) {
	double res = 0;

	for (int i = left; i < right + 1; i ++) {
		res += 1.0 / i;
	}

	return res;
}
