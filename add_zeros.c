#include "header.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

extern char currentAddressHex[64];
extern char currentAddressBinary[128];

void add_zeros(){

  int i = 0;
  int count;

  while(currentAddressHex[i] != '\0'){
    i++;
  }
  count = 64 - (i * 4);
  while(count>0){
    strcat(currentAddressBinary, "0");
    count--;
  }

}
