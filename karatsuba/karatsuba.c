/*
  Karatsuba implementation using pthreads

  Luke Jennings, Zack Gilman, JT Capper

  Dec 13, 2021
 */

#include <gmp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include <unistd.h>

/* Thread structure holds information about each thread */
typedef struct worker_thread {
  int thread_num;
  pthread_t thread_is;
} worker_thread;

int main(int argc, char *argv[]) {
  return 0;
}
