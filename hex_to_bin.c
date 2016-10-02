#include "header.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

extern char currentAddressHex[64];
extern char currentAddressBinary[128];

void hex_to_bin(){
  int i = 0;
  while(i<64){
    switch(currentAddressHex[i]){
      case '0': strcat(currentAddressBinary, "0000"); break;
      case '1': strcat(currentAddressBinary, "0001"); break;
      case '2': strcat(currentAddressBinary, "0010"); break;
      case '3': strcat(currentAddressBinary, "0011"); break;
      case '4': strcat(currentAddressBinary, "0100"); break;
      case '5': strcat(currentAddressBinary, "0101"); break;
      case '6': strcat(currentAddressBinary, "0110"); break;
      case '7': strcat(currentAddressBinary, "0111"); break;
      case '8': strcat(currentAddressBinary, "1000"); break;
      case '9': strcat(currentAddressBinary, "1001"); break;
      case 'A': strcat(currentAddressBinary, "1010"); break;
      case 'B': strcat(currentAddressBinary, "1011"); break;
      case 'C': strcat(currentAddressBinary, "1100"); break;
      case 'D': strcat(currentAddressBinary, "1101"); break;
      case 'E': strcat(currentAddressBinary, "1110"); break;
      case 'F': strcat(currentAddressBinary, "1111"); break;
      case 'a': strcat(currentAddressBinary, "1010"); break;
      case 'b': strcat(currentAddressBinary, "1011"); break;
      case 'c': strcat(currentAddressBinary, "1100"); break;
      case 'd': strcat(currentAddressBinary, "1101"); break;
      case 'e': strcat(currentAddressBinary, "1110"); break;
      case 'f': strcat(currentAddressBinary, "1111"); break;
      default:  ;
    }
    i++;

  }
}
