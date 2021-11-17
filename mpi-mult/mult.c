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


//integers in GMP are declared as mpz_t sum;


int main(int argc, char *argv[]){
  //this program is to test how to use the gmp library, and be able to successfully multiply large numbers. Things such as parallelization will be done later

  //here is where we initialize our mpz_t, which stores our really large ints

  printf("please input 2 integers:\n");
  char* input1 = (char *)malloc(1000 * sizeof(char));
  char* input2 = (char *)malloc(1000 * sizeof(char));
  scanf("%s",input1);
  scanf("%s",input2);

  //here is where we initialize our mps_t variables
  mpz_t big_int1, big_int2;
  //the parameters for this are the mpz_t, the string, and the base of the string
  mpz_init_set_str(big_int1,input1,10);
  mpz_init_set_str(big_int2,input2,10);

  //printing out the numbers
  mpz_t output;
  mpz_init(output);

  mpz_mul(output,big_int1,big_int2);
  
  gmp_printf("%Zd\n",output);
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
