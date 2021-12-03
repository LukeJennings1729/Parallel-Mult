/*
  Serial implementation of a long multiplication
  algorithm.

  Created: Nov 16, 2021
  Last Edited: Nov 16, 2021
  
  Luke Jennings, Joseph Capper, Zack Gilman
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h> //for makefile, use -lgmp
#include <time.h>
#include <unistd.h>
#include "timer.h"

#define MAX_BITS 1073741824

//integers in GMP are declared as mpz_t sum;

int main(int argc, char *argv[]){
  char *input1  = (char *)malloc(MAX_BITS * sizeof(char));
  char *input2 = (char *)malloc(MAX_BITS * sizeof(char));
  FILE *in_file1, *in_file2;
  struct timeval start, stop;

  //this program is to test how to use the gmp library, and be able to successfully multiply large numbers.

  //here is where we initialize our mps_t variables
  mpz_t big_int1, big_int2;

  if(argc < 3){
    fprintf(stderr,"[%s] USAGE: FILE file1, File file2 \n",argv[0]);
    exit(1);
  }

  in_file1 = fopen(argv[1],"r");
  in_file2 = fopen(argv[2],"r");
  //if in_file is still NULL
  if(in_file1 == NULL){
    fprintf(stderr,"[%s] could not open file 1 %s\n",argv[0], argv[1]);
    exit(1);
  }
  if(in_file2 == NULL){
    fprintf(stderr,"[%s] could not open file 2 %s\n",argv[0], argv[2]);
    exit(1);
  }

  //scan numbers from files
  fscanf(in_file1,"%s",input1);
  fscanf(in_file2,"%s",input2);

  gettimeofday(&start, NULL);
 
  mpz_init(big_int1);
  mpz_init(big_int2);
  mpz_set_str(big_int1,input1,16);
  mpz_set_str(big_int2,input2,16);
 
  //printing out the numbers
  mpz_t output;
  //gmp_printf("int 1:%Zd \nint 2:%Zd\n",big_int1,big_int2);
  mpz_init(output);

  //Do the multiplication
  mpz_mul(output,big_int1,big_int2);

  gettimeofday(&stop, NULL);

  //Print the output
  gmp_printf("%Zd\n",output);

  printf("Time Elapsed: %f\n", diffgettime(start, stop));

  return 0;
  
}
