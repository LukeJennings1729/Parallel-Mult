/*
  This program is designed to parallelize the multiplicaction of 2 mpz_t structures
  
  Given that mpz_t multiplication is already optimized based on the size of the integer, our goal
  is to make it so that we can create a parallel program that makes this process even faster
  
  The ideal with this program is to be able to split one factor into smaller chunks, then have each process
  calculate a piece of the product, which is then added together at the end.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <gmp.h>

mpz_t factor1;
mpz_t factor2;
mpz_t total;
#define MAX_BITS 32768 //this is means we will read numbers up to 9,865 digits long)

/*
  @param argv[1] string representing the method in which we read in the numbers
  'r' for random, 'c' command line, and if file then the file's name
  @param argv[2] --optional string of digits representing our first factor (only if 'c' was selected)
  @param argv[3] --optional string of digits representing our first factor (only if 'c' was selected)
 */
int main(int argc, char *argv[]){
  
  //here we should initialize the factors and total
  mpz_init(factor1);
  mpz_init(factor2);
  if(argc < 2){
    fprintf(stderr,"[%s] too few parameters, please submit an input type random|filename|command\n",argv[0]);
    exit(1);
  }

  
  switch(argv[1]){
    case "random":
    case "r":
      
  break;
    //this case meas that we are reading the numbers in from the command line
  case "command":
  case "c":
    //this assumes that the input is only in base 10
    mpz_set_str(factor1,argv[2],10);
    mpz_set_str(factor2,argv[3],10);
    
    break;
  default:
    //create char* to hold the strings that represent our large integers
    char *input1 = (char *)malloc(MAX_BITS * sizeof(char));
    char *input2 = (char *)malloc(MAX_BITS * sizeof(char));

    FILE *in_file = NULL;
    in_file = fopen(argv[1],"r");
    //if in_file is still NULL
    if(in_file = NULL){
      fprintf(stderr,"[%s] could not open file %s\n",argv[1]);
      exit(1);
    }
    //now we want to set the 2 strings that we read in from the file to be our 2 factors

  }  
  return 0;
}
