#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 16 // tamanho do vetor
#define ROOT 0 // rank raiz

int main(int argc, char** argv) {
  int rank, size;
  float *array = NULL;
  float local_max, global_max;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size < 3) { 
    printf("Erro: o programa deve ser executado com mais de 2 processos.\n");
    MPI_Finalize();
    exit(1);
  }

  if (rank == ROOT) { 
    array = (float*) malloc(sizeof(float) * ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; i++) {
      array[i] = (float) rand() / (float) RAND_MAX; 
    }
    MPI_Scatter(array, ARRAY_SIZE/size, MPI_FLOAT, MPI_IN_PLACE, ARRAY_SIZE/size, MPI_FLOAT, ROOT, MPI_COMM_WORLD);
  } else {
    array = (float*) malloc(sizeof(float) * ARRAY_SIZE/size);
    MPI_Scatter(NULL, ARRAY_SIZE/size, MPI_FLOAT, array, ARRAY_SIZE/size, MPI_FLOAT, ROOT, MPI_COMM_WORLD);
  }

  local_max = array[0];
  for (int i = 1; i < ARRAY_SIZE/size; i++) {
    if (array[i] > local_max) {
      local_max = array[i];
    }
  }

  MPI_Reduce(&local_max, &global_max, 1, MPI_FLOAT, MPI_MAX, ROOT, MPI_COMM_WORLD);

  if (rank == ROOT) { 
    printf("O maior elemento do vetor é: %.2f\n", global_max);
    free(array);
  }

  MPI_Finalize();
  return 0;
}
