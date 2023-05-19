#include <mpi.h>
#include <stdio.h>

int main (int argc, char **argv) {
	int rank, commsize, var = 0;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &commsize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (!rank) {
		printf ("sent var(%d) = %d\n", rank, var);
		MPI_Send(&var, 1, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
		MPI_Recv(&var, 1, MPI_DOUBLE, commsize - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("received var(%d) = %d\n", rank, var);
	} else {
		MPI_Recv(&var, 1, MPI_DOUBLE, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("received var(%d) = %d\n", rank, var);

		var ++;

		if (rank < commsize - 1)
			MPI_Send(&var, 1, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
		else
			MPI_Send(&var, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
		printf ("sent var(%d) = %d\n", rank, var);
	}

	MPI_Finalize();

	return 0;
}
