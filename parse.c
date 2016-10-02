
#include "header.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

extern char currentAddressBinary[128];
extern char currentTag[64];
extern char currentIndex[64];

void parse_binary(int tagWidth, int indexWidth){
  //Parse tag
  for(int i=0; i<tagWidth; i++){
    currentTag[i] = currentAddressBinary[i];
  }
  currentTag[tagWidth] = '\0';
  //Parse index
  int k = 0;
  for(int i=(tagWidth); i<(indexWidth + tagWidth); i++){
    currentIndex[k++] = currentAddressBinary[i];
  }
  currentIndex[indexWidth + tagWidth] = '\0';

}
