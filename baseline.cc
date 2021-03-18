// C includes
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <vector>

using namespace std;
#define DIVUP(m, n) ((m) / (n) + ((m) % (n) > 0))

pthread_mutex_t mutex;

int* buffer;
int num_threads = 16;
int height = 32;
int length = 16;

void* mymalloc(void*);
void* mymemset(void*);

double wtime() {
  struct timeval  v;
  struct timezone z;

  int i = gettimeofday(&v, &z);
  return ((double)v.tv_sec + (double)v.tv_usec * 1.0e-6);
}


typedef struct thread_data{
    int size;
    int rank;
    int height;
    int length;
    int* buffer;
}thedata;

int main(int argc, char **argv) {
    int option;
    bool show_matrix = false;
    bool show_time = false;
    double t0, t1;

    // -n 设置线程数目
    // -h 设置矩阵的行数
    // -l 设置矩阵的列数
    // -s 最后打印矩阵
    while ((option = getopt(argc,argv,"n:h:l:ts")) != -1) {
        switch (option) {
            case 'n':
                num_threads = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 'l':
                length = atoi(optarg);
                break;
            case 't':
                show_time = true;
                break;
            case 's':
                show_matrix = true;
                break;
            default:
                cout << "Usage <-n num-threads> <-h size of matrix's rows> <-l size of matrix's height> <-s show the matrix>" << endl;
                exit(EXIT_FAILURE);
        }
    }

    std::vector<pthread_t*> threads;

    t0 = wtime();
    buffer = (int*)malloc(height * length * sizeof(int));
    t1 = wtime();
    if(show_time){
        cout << "malloc a matrix with size [" << height << ", " << length << "], which cost " << (t1 - t0) * 1.0e3 << "ms" << endl;
    }

    t0 = wtime();
    for (int i=0; i<num_threads; i++) {
        pthread_t* thread = new pthread_t;
        thedata* data_ = new thedata;
        data_->rank = i;
        data_->size = num_threads;
        data_->height = height;
        data_->length = length;
        data_->buffer = buffer;
        pthread_create(thread, NULL, &mymemset, (void *)data_);
        threads.push_back(thread);
    }

    for (int i=0; i<num_threads; i++) {
        pthread_join(*threads[i], NULL);
    }
    t1 = wtime();
    if(show_time){
        cout << num_threads << " threads executes concurrently, which cost " << (t1 - t0) * 1.0e3 << "ms" << endl;
    }

    for (int i=0; i<num_threads; i++) {
        delete threads[i];
    }

    if(show_matrix){
        for(size_t i = 0; i < height; i++){
            for(size_t j = 0; j < length; j++){
                cout << buffer[i * length + j] << " "; 
            }
            cout << endl;
        }
    }
    free(buffer);
    return 0;
}

void* mymalloc(void*){
    buffer = (int*)malloc(height * length * sizeof(int));
    pthread_exit(NULL);
}

void* mymemset(void* ptr){
    thedata* data_ = (thedata*)ptr;
    int k = DIVUP(data_->height, data_->size);
    // pthread_mutex_lock(&mutex);
    for(int i = k * data_->rank; i < (data_->rank + 1) * k && i < data_->height;i++){
        for(int j = 0; j < data_->length;j++){
            data_->buffer[i * data_->length + j] = data_->rank * data_->rank;
        }
    }
    // pthread_mutex_unlock(&mutex);
}