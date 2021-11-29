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

#define MAX_BITS 32768

//integers in GMP are declared as mpz_t sum;

int main(int argc, char *argv[]){
  char *input1  = (char *)malloc(MAX_BITS * sizeof(char));
  char *input2 = (char *)malloc(MAX_BITS * sizeof(char));
  FILE *in_file1, *in_file2;
  //this program is to test how to use the gmp library, and be able to successfully multiply large numbers. Things such as parallelization will be done later

  //here is where we initialize our mpz_t, which stores our really large ints
  
  /*
    printf("please input 2 integers:\n");
    char* input1 = (char *)malloc(10000 * sizeof(char));
    char* input2 = (char *)malloc(10000 * sizeof(char));
    scanf("%s",input1);
    scanf("%s",input2);
  */

  //here is where we initialize our mps_t variables
  mpz_t big_int1, big_int2;

  
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
  
  fscanf(in_file1,"%s",input1);
  fscanf(in_file2,"%s",input2);

  mpz_set_str(big_int1,input1,16);
  mpz_set_str(big_int2,input2,16);
  
  /*
  //the parameters for this are the mpz_t, the string, and the base of the string
  mpz_init(big_int1);
  mpz_rand(big_int1);
  sleep(1);
  mpz_init(big_int2);
  mpz_rand(big_int2);
  */
  //printing out the numbers
  mpz_t output;
  gmp_printf("int 1:%Zd \nint 2:%Zd\n",big_int1,big_int2);
  mpz_init(output);

  mpz_mul(output,big_int1,big_int2);
  
  gmp_printf("Output: %Zd\n",output);
  return 0;
  
}
// Should we use arrays to store numbers? (one digit per index)

/*
int count_digits(char *long_int) {

  int digits;
  digits = 0;
  while (long_int[digits] != '\0')
    digits++;
  return digits;
}

int main(int argc, char *argv[]) {

  char *massive_int_1, *massive_int_2;
  //char *product;
  
  if (argc != 3) {
    fprintf(stderr, "Usage: ./%s massive_int_1 massive_int_2\n", argv[0]);
    exit(1);
  }

  massive_int_1 = argv[1];
  massive_int_2 = argv[2];
  
  printf("There are %d digits in massive_int_1\n", count_digits(massive_int_1));
  printf("There are %d digits in massive_int_2\n", count_digits(massive_int_2));
  return 0;
}
*/
