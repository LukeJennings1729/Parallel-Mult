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
mpz_t aplusb;
mpz_t cplusd;
mpz_t shift;
//factoring represents the steps starting with (a + b)(c + d)
mpz_t factoring;
int z;
mpz_t total;
char *input1;
char *input2;
int len_input1;
int len_input2;
int len_ab;
int len_cd;
char *ab_string;
char *cd_string;
char *a_string;
char *b_string;
char *c_string;
char *d_string;


/* this will parse out the a and b values of the first string */
void *a_parse(void *arg){
  a_string = (char *)malloc((len_ab - z + 1)*sizeof(char));
  //we copy the length of the string-z digits into
  strncpy(a_string,ab_string,len_ab - z);
  a_string[len_ab - z] = '\0';

  mpz_init_set_str(a,a_string,16);

  //in order not to do the initialization later in serial, we parallelize it here
  mpz_init(aplusb);
  pthread_exit(0);
}

void *b_parse(void *arg){
  b_string = (char *)malloc((z+1)*sizeof(char));
  strncpy(b_string,ab_string + (len_ab - z),z);
   b_string[z] = '\0';

   mpz_init_set_str(b,b_string,16);
   pthread_exit(0);
}

/* this will prase out the c and d values of the second string */
void *c_parse(void *arg){
    //c is a special case where potientially the second factor is way too small in comparison to ab
  if(len_cd - z > 0){
    c_string = (char *)malloc((len_cd - z + 1) * sizeof(char));
    strncpy(c_string,cd_string,len_cd - z);
    c_string[len_cd - z] = '\0';

    mpz_init_set_str(c,c_string,16);
  } else {
    //this by default sets c to 0 in the case that the factor cd is less than half the length of ab
    mpz_init(c);
  }
  //here we are initializing the total mpz_t struct in order to not have to do it later in serial
  mpz_init(total);

  pthread_exit(0);
}

void *d_parse(void *arg){
  d_string = (char *)malloc((z+1)*sizeof(char));
  if(len_cd - z > 0){
    strncpy(d_string,cd_string + (len_cd - z),z);
    d_string[z] = '\0';

    mpz_init_set_str(d,d_string,16);
  } else {
    //if c does not exist then we want to set this value to be the entirety of cd 
    mpz_init_set_str(d,cd_string,16);
  }
  //in order to not initialize cplusd in serial later, we do it here instead
  mpz_init(cplusd);

  pthread_exit(0);
}


/* this will shift over the ac partial product by z^2 digits */
void *ac_shift(void *arg){
  //we want to shift over our result of ac by z by z, as that is how many zeros the product would have
  mpz_mul_2exp(shift,ac,2*z*4);

  pthread_exit(0);
}

/* Calculates the product if the A and C numbers */
void *ac_compute(void *arg) {

  //here we multiply a and c to be later used
  mpz_init(ac);
  mpz_mul(ac,a,c);
  pthread_exit(0);
}

/* Calculates the product of the B and D numbers */
void *bd_compute(void *arg) {
  mpz_init(bd);
  mpz_mul(bd,b,d);
  
  pthread_exit(0);
}

void *finish_factoring(void *arg){
  //we need to remove from (a + b)(c + d) the values of ac and bd so 
  mpz_sub(factoring, factoring, ac);
  
  mpz_sub(factoring, factoring, bd);
  
  mpz_mul_2exp(factoring,factoring,4 * z);
  pthread_exit(0);
}

/*
  argv[1] name of first file that contains comically large number
  argv[2] name of second file that also contains comically large number
  argv[3] optional, if "d" then the solution will print in decimal
  if "x" then the solution will print in hexadecimal
 */
int main(int argc, char *argv[]) {

 
  struct timeval start, stop;
  FILE *in_file1, *in_file2;

  //buffers to place the string integer of the file
  char *input1 = (char *)malloc(MAX_BYTES * sizeof(char));
  char *input2 = (char *)malloc(MAX_BYTES * sizeof(char));
  int len_input1, len_input2;

  in_file1 = fopen(argv[1],"r");
  in_file2 = fopen(argv[2],"r");

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

  //here we start the timer as the integer string has been fully scanned in

  gettimeofday(&start, NULL);
  
  len_input1 = strlen(input1);
  len_input2 = strlen(input2);

  
  fclose(in_file1);
  fclose(in_file2);
  //next we want to set the longer number as ab, and the smaller number as cd, from there we determine z, the number of digits a and c are shifted over by //
  if(len_input1 >= len_input2){
    ab_string = input1;
    cd_string = input2;
    
    len_ab = len_input1;
    len_cd = len_input2;
    z = len_input1 / 2;
  } else {
    ab_string = input2;
    cd_string = input1;
    
    len_ab = len_input2;
    len_cd = len_input1;
    z = len_input2 / 2;
  }

  gmp_printf("z = %d\n", z);

  pthread_t *threads = (pthread_t *)malloc(4*sizeof(pthread_t));
  /* from here we start up new threads in order to find the values of
     a, b,c, and d */
  pthread_create(&threads[0],NULL,d_parse,NULL);
  pthread_create(&threads[1],NULL,a_parse,NULL);
  pthread_create(&threads[2],NULL,b_parse,NULL);
  pthread_create(&threads[3],NULL,c_parse,NULL);
  
  pthread_join(threads[0],NULL);
  pthread_join(threads[1],NULL);
  pthread_join(threads[2],NULL);
  pthread_join(threads[3],NULL);

  free(input1);
  free(input2);
  free(ab_string);
  free(cd_string);
  
  /* 
     after parsing through the inputs into a,b,c, and d we now need
     to calculate the 3 chunks, ac, bd, and (a + b)(c + d)
  */
  pthread_create(&threads[0],NULL,ac_compute,NULL);
  pthread_create(&threads[1],NULL,bd_compute,NULL);

  //in this step while the other threads are finding ac and bd, we are
  // doing the adds for a + b and c + d
  //in order to even out work, the inits for the aplusb and cplusd have been
  //moved to the a_parse and d_parse functions
  mpz_init(factoring);
  mpz_add(aplusb,a,b);
  mpz_add(cplusd,c,d);

  mpz_mul(factoring,aplusb,cplusd);

  pthread_join(threads[0],NULL);
  pthread_join(threads[1],NULL);

  //here we are shifting over the ac and the (ad + bc) parts
  pthread_create(&threads[0],NULL,ac_shift,NULL);
  pthread_create(&threads[1],NULL,finish_factoring,NULL);

  pthread_join(threads[0],NULL);
  pthread_join(threads[1],NULL);
  //originally we did the initialization of the total mpz_t struct here, however, we figured this would be better done in parallel earlier on.

  gmp_printf("shift = %Zx\n", shift);
  mpz_add(total, total, shift);
  gmp_printf("factoring = %Zx\n", factoring);
  mpz_add(total, total, factoring);
  gmp_printf("bd = %Zx\n", bd);
  mpz_add(total, total, bd);

  //now that we have our desired total we can stop the timer
  gettimeofday(&stop, NULL);
  
  if(argv[3] != NULL){
    if(argv[3][0] == 'd'){
      gmp_printf("%Zd\n",total);
    }else if (argv[3][0] == 'x'){
      gmp_printf("%Zx\n",total);
    }
  }

  printf("Time elapsed w/o overhead: %f s\n", diffgettime(start, stop));
  // Free arrays

  for(int i; i < 4; i++){
    free(threads[i]);
  }
  free(threads);
  return 0;
}

