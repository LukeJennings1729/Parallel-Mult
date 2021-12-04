/*
  This program is designed to parallelize the multiplicaction of 2 mpz_t structures
  
  Given that mpz_t multiplication is already optimized based on the size of the integer, our goal
  is to make it so that we can create a parallel program that makes this process even faster
  
  The ideal with this program is to be able to split one factor into smaller chunks, then have each process
  calculate a piece of the product, which is then added together at the end.

  WARNING: ALL NUMBERS MUST BE IN HEXADECIMAL(at least in this version)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <gmp.h>
#include <string.h>
#include <unistd.h>

#include "timer.h"

//we need a mutex to make sure when we adding all the things together at the end we don't conflict when we add up all the partial sums
pthread_mutex_t mutex;

mpz_t main_factor;
char *split_factor;
//how many digits are in each of the smaller factors, this is important for calculating the correct sum at the end
int len_input1, len_input2, len_split;
int num_threads;

mpz_t total;
#define MAX_BYTES 1073741824

typedef struct worker_t {
  int thread_num;
  pthread_t thread_id;
} worker_t;

void *thread_compute(void *arg){

  worker_t *worker = (worker_t *)arg;
  int start, count, leftovers;
  char *substring;
  mpz_t factor_that_splits, output;
  
  leftovers = len_split % num_threads;
  //start = worker->thread_num*len_split/num_threads + min(&(worker->thread_num), &leftovers);
  start = (worker->thread_num*len_split/num_threads) + ( ((worker->thread_num) < (leftovers)) ? (worker->thread_num) : (leftovers) );
  count =  len_split/num_threads;

  if (worker->thread_num < leftovers)
    count++;
  

  substring = (char *)malloc((count+1)*sizeof(char));
  strncpy(substring, (split_factor+start), (count));
  substring[count] = '\0';

  //  printf("[%d] %s\n", worker->thread_num, substring);
  
  mpz_init_set_str(factor_that_splits, substring, 16);

  mpz_init(output);
  mpz_mul(output, main_factor, factor_that_splits);

  //gmp_printf("output = %Zd\n", output);
  
  mpz_mul_2exp(factor_that_splits, output, (len_split-(start+count))*4);

  pthread_mutex_lock(&mutex);
  mpz_add(total, total, factor_that_splits);
  //gmp_printf("[%d] %Zd\n", worker->thread_num, factor_that_splits);
  pthread_mutex_unlock(&mutex);
  
  pthread_exit(0);
}



/*
  @param argv[1] number of threads
  @param argv[2] if c was selected, input integer from command line. If f was selected input file containing first integer
  @param argv[3] same as argv[3] but for the second factor
  @prama argv[4] if not null, if 'd' then will print result in decimal, if 'x' product will be printed in hex
 */
int main(int argc, char *argv[]){
  pthread_mutex_init(&mutex, NULL);
  
  //here we should initialize the factors and total
  mpz_init(main_factor);
  //mpz_init(factor2);
  char *input1  = (char *)malloc(MAX_BYTES * sizeof(char));
  char *input2 = (char *)malloc(MAX_BYTES * sizeof(char));
  FILE *in_file1, *in_file2;
  struct timeval start, stop;

  
  if(argc < 3){
    fprintf(stderr,"[%s] too few parameters, number of threads, file name 1, file name 2 \n",argv[0]);
    exit(1);
  }
  num_threads = strtol(argv[1],NULL,16);
  
  //len_split = (int *)malloc(num_threads * sizeof(char));

  /*
  switch(atoi(argv[2])){
    //case "random":
    case 'r':
      
  break;
    //this case meas that we are reading the numbers in from the command line
    //case "command":
  case 'c':
    //this assumes that the input is only in base 10
    //mpz_set_str(factor1,argv[3],10);
    //mpz_set_str(factor2,argv[4],10);
    
    break;
  default:

  */
  
    in_file1 = fopen(argv[2],"r");
    in_file2 = fopen(argv[3],"r");
    
    //if in_file is still NULL
    if(in_file1 == NULL){
      fprintf(stderr,"[%s] could not open file 1 %s\n",argv[0], argv[3]);
      exit(1);
    }
    if(in_file2 == NULL){
      fprintf(stderr,"[%s] could not open file 2 %s\n",argv[0], argv[4]);
      exit(1);
    }
    
    fscanf(in_file1,"%s",input1);
    fscanf(in_file2,"%s",input2);

    gettimeofday(&start, NULL);
    
    len_input1 = strlen(input1);
    len_input2 = strlen(input2);
    
    //next we want to get the length of our strings, we will then make the larger one the one which is split up, and the smaller the factor all threads will multiply against
    if(len_input1 > len_input2){
      //input2 is the smaller one, therefore we make it main_factor
      mpz_set_str(main_factor,input2,16);

      split_factor = input1;
      len_split = len_input1;
      
      //now we start splitting up the pieces
    } else {
      mpz_set_str(main_factor,input1,16);

      split_factor = input2;
      len_split = len_input2;
    }
    
    
    //now we want to set the 2 strings that we read in from the file to be our 2 factors

  //here we set up all our threads, storing it an array called thread_info
  worker_t *thread_info = (worker_t *)malloc(num_threads*sizeof(worker_t));

  int rc;
  for(int thread = 0; thread < num_threads; thread++){
   thread_info[thread].thread_num = thread;
   rc = pthread_create(&(thread_info[thread].thread_id),NULL,thread_compute,&(thread_info[thread].thread_num));
   if(rc != 0){
     fprintf(stderr,"Could not create thread %d\n",thread);
     exit(1);
    }
  }

  //closing our threads
  for(int thread = 0; thread < num_threads; thread++){
      pthread_join(thread_info[thread].thread_id,NULL);
  }

  gettimeofday(&stop, NULL);

  //Print the output
  if(argv[4] != NULL){
    if(argv[4][0] == 'd'){
      gmp_printf("%Zd\n",total);
    }else if (argv[4][0] == 'x'){
      gmp_printf("%Zx\n",total);
    }
  }


  printf("Time elapse with overhead: %f\n", diffgettime(start, stop));
  
  return 0;
}
