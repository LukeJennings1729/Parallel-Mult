#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

int main(int argc, char *argv[]){

  //for(int i 0; i < 1000; )
  mpz_t rop;
  mpz_init (rop);
  gmp_randstate_t state;
  gmp_randinit_mt(state);
  mpz_t seed;
  printf("1\n");
  for(int i = 0; i < 10; i++){
  gmp_randseed_ui(state,i);
  printf("1\n");
  fflush(stdout);
  mp_bitcnt_t n = 10;
  printf("1\n");
  fflush(stdout);
  mpz_urandomb(rop, state, 100);
  
  gmp_printf("rob = %Zd\n",rop);
  }
  return 0;
}
