#include "header.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

extern unsigned int blockSize;
extern unsigned int size;
extern unsigned int assoc;
extern unsigned int replPolicy;
extern unsigned int inclPolicy;
extern unsigned int numberOfSets;
extern unsigned int numberOfWays;
extern struct Block **cache;
extern long long int decimalTag;
extern int decimalIndex;
extern int l1Misses;

extern int **binTreeArray;

//Traces index from bottom child to top parent, flipping bits along the way
void parent_loop(int i){
  i = i + (numberOfWays - 1); //map way index to tree index
  for(int j=0;j<(int)(log2(2*numberOfWays)-1);j++){ //iterate through the loop for as many "levels" in the tree
    if(i % 2){ //index is odd, from the left
      i = (i-1)/2; //get parent index value
      if(!binTreeArray[decimalIndex][i]){ //if bit value at i is 0 (matches left) flip it
        binTreeArray[decimalIndex][i] = 1;
      }
    }
    else if(!(i % 2)){ //index is even, from the right
      i = (i/2)-1; //get parent index value
      if(binTreeArray[decimalIndex][i]){ //if bit value at i is 1 (matches right) flip it
        binTreeArray[decimalIndex][i] = 0;
      }
    }
  }

  for(int k=0;k<(numberOfWays-1);k++){
  }
}

//Traces index, following bits (arrows) through children to bottom, returns index of bottom child
int child_loop(){
  int i = 0;
  for(int k=0;k<(numberOfWays-1);k++){
  }
  for(int j=0;j<(int)(log2(2*numberOfWays)-1);j++){
    if(binTreeArray[decimalIndex][i]){ //value is 1, go right
      i = (2*i) + 2;
    }
    else if(!binTreeArray[decimalIndex][i]){ //value is 0, go left
      i = (2*i) + 1;
    }
  }
  i = i - (numberOfWays - 1);
  return i; //return final i value, the index of least recently used way

}
