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

  char *str;
  
  if (argc != 2) {
    fprintf(stderr, "Enter a \"String\"");
    exit(1);
  }

  str = argv[1];

  printf("There are %d cahracters in str\n", count_digits(str));
  return 0;
}
