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

#define MAX_BYTES 1073741824

pthread_mutex_t mutex;
mpz_t a;
mpz_t b;
mpz_t c;
mpz_t d;
mpz_t ac;
mpz_t bd;
int z;
mpz_t total;
char *input1;
char *input2;
int len_input1;
int len_input2;
char *ab_string;
char *cd_string;

typedef struct scan_thread {
  int thread_num;
  pthread_t thread_id;
  FILE *file;
} scan_thread;

typedef struct worker_thread {
  int thread_num;
  pthread_t thread_id;
} worker_thread;

/* scans file for input 1 */
void *scan_file(void *arg){
  scan_thread *t = (scan_thread *)arg;
  if (t->thread_num == 1)
    fscanf(t->file, "%s", input1);
  else
    fscanf(t->file, "%s", input2);
}

/* this will parse out the a and b values of the first string */
void *ab_parse(void *arg){

}
/* this will prase out the c and d values of the second string */
void *cd_parse(void *arg){

}
/* this will shift over the ac partial product by z^2 digits */
void *ac_shift(void *arg){

}

/* Calculates the product if the A and C numbers */
void *ac_compute(void *arg) {

}

/* Calculates the product of the B and D numbers */
void *bd_compute(void *arg) {

}

/*
  argv[1] name of first file that contains comically large number
  argv[2] name of second file that also contains comically large number
 */
int main(int argc, char *argv[]) {

  int rc;
  struct timeval start, stop;
  pthread_t *thread_info;
  scan_thread *scan_t_info;
  FILE *files;
  size_t i;
  FILE *in_file1, *in_file2;

  // Initialize mutex
  pthread_mutex_init(&mutex, NULL);
  
  char *input1 = (char *)malloc(MAX_BYTES * sizeof(char));
  char *input2 = (char *)malloc(MAX_BYTES * sizeof(char));
  int len_input1, len_input2;

  in_file1 = fopen(argv[1],"r");
  in_file2 = fopen(argv[2],"r");

  scan_t_info = (scan_thread *)malloc(2*sizeof(scan_thread));

  // Create threads to scan files
  for (i=1; i<=2; i++) {
    scan_t_info[i].thread_num = i;
    rc = pthread_create(&(scan_t_info->thread_id), NULL, scan_file, &(scan_t_info[i].thread_num));
    if (rc!=0) {
      fprintf(stderr, "Could not create thread\n");
      exit(1);
    }
  }

  //closing our scan threads
  for(i=1; i<=2; i++)
      pthread_join(scan_t_info[i].thread_id,NULL);
  free(scan_t_info);

  printf("input1 = %s\ninput2 = %s\n", input1, input2);

  // Free arrays
  free(files);
  free(thread_info);
  free(input1);
  free(input2);

  // Destroy mutex
  pthread_mutex_destroy(&mutex);
  return 0;
}


int main(int argc, char *argv[]) {
  return 0;
}
