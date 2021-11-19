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

//we need a mutex to make sure when we adding all the things together at the end we don't conflict when we add up all the partial sums
pthread_mutex_t mutex;

mpz_t main_factor;
char **split_factors;
//how many digits are in each of the smaller factors, this is important for calculating the correct sum at the end
int *len_split;

mpz_t total;
#define MAX_BITS 32768

typedef struct worker_t {
  int thread_num;
  pthread_t thread_id;
} worker_t;

void *thread_compute(void *arg){


  pthread_exit(0);
}

/*
  @param argv[1] number of threads
  @param argv[2] string representing the method in which we read in the numbers
  'r' for random, 'c' command line, and if file then the file's name
  @param argv[3] if c was selected, input integer from command line. If f was selected input file containing first integer
  @param argv[4] same as argv[3] but for the second factor
 */
int main(int argc, char *argv[]){
  
  //here we should initialize the factors and total
  mpz_init(main_factor);
  //mpz_init(factor2);
  char *input1  = (char *)malloc(MAX_BITS * sizeof(char));
  char *input2 = (char *)malloc(MAX_BITS * sizeof(char));
  if(argc < 3){
    fprintf(stderr,"[%s] too few parameters, number of threads, type of input (file,command,or random), [optional file names or integer] \n",argv[0]);
    exit(1);
  }
  int num_threads = strtol(argv[1],NULL,16);
  
  len_split = (int *)malloc(num_threads *sizeof(char));
  

  
  switch(argv[2]){
    case "random":
    case "r":
      
  break;
    //this case meas that we are reading the numbers in from the command line
  case "command":
  case "c":
    //this assumes that the input is only in base 10
    //mpz_set_str(factor1,argv[3],10);
    //mpz_set_str(factor2,argv[4],10);
    
    break;
  default:

    FILE *in_file = NULL;
    in_file1 = fopen(argv[3],"r");
    in_file2 = fopen(argv[4],"r");
    //if in_file is still NULL
    if(in_file1 = NULL){
      fprintf(stderr,"[%s] could not open file %s\n",argv[3]);
      exit(1);
    }
    if(in_file2 = NULL){
      fprintf(stderr,"[%s] could not open file %s\n",argv[4]);
      exit(1);
    }
    
    fscanf(in_file1,"%s",input1);
    fscanf(in_file2,"%s",input2);
    //next we want to get the length of our strings, we will then make the larger one the one which is split up, and the smaller the factor all threads will multiply against
    if(strlen(input1) > strlen(input2)){
      //input2 is the smaller one, therefore we make it main_factor
      mpz_set_str(main_factor,input2,16);
      //now we start splitting up the pieces
    } else {
      mpz_set_str(main_factor,input1,16);
    }
    
    
    //now we want to set the 2 strings that we read in from the file to be our 2 factors

  }

  //here we set up all our threads, storing it an array called thread_info
  thread_info = (worker_t *)malloc(num_threads*sizeof(worker_t));

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

  return 0;
}
