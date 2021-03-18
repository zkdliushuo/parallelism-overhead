#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#define HEIGHT 64
#define LENGTH 16
#define THREADS 16


int main(int argc, char** argv) {
  // omp_set_num_threads(16);
  int world_rank;
  int world_size;

  MPI_Init(&argc, &argv);                      // 初始化MPI上下文
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);  // 获取进程数
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);  // 获取本进城ID号
  int* buffer ;

  if(world_rank == 0) {
    buffer = (int*)malloc(HEIGHT * LENGTH * sizeof(int));
  }

  MPI_Barrier(MPI_COMM_WORLD);
  std::cout << "my id is " << world_rank << std::endl;
  int nTrials = 10, iTrial;
  for(iTrial = 1; iTrial <= nTrials; iTrial++) {
        int i, j;
        int k = HEIGHT / THREADS;
        for(i = world_rank * k ; i < (world_rank + 1) * k; i++){
            for(j = 0;j < LENGTH; j++){
                buffer[i*LENGTH + j] = world_rank;
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
  }
  if(world_rank == 0) {
      int i, j;
      for( i = 0;  i < HEIGHT; i++){
          for( j = 0; j < LENGTH; j++){
              std::cout << buffer[i*LENGTH + j] << " ";
          }
          std::cout << std::endl;
      }

      free(buffer);
  }

  MPI_Finalize();
}
