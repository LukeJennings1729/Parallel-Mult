/*
  Serial implementation of a long multiplication
  algorithm.

  Created: Nov 16, 2021
  Last Edited: Nov 16, 2021
  
  Luke, Joseph, Zack
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Should we use arrays to store numbers? (one digit per index)

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
