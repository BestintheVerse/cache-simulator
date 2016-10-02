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
extern char currentOpCode[1];
extern bool isWrite;
extern int writeBacks;

extern int **binTreeArray;

int fullNum;

void cache_process(){
  fullNum = 0;
  int cacheHit = 0;
  //Check cache blocks at index to see if hit
  for(int i=0; i<numberOfWays; i++){
    if(cache[decimalIndex][i].isFull){
      if(cache[decimalIndex][i].tag == decimalTag){
        //Hit!
        rank_LRU_hit();
        pseudo_hit();
        cacheHit = 1;
        if(isWrite){
          cache[decimalIndex][fullNum].isDirty = true;
         }
        break;
      }
      fullNum++;
    }
  }

  //If miss, check and see how full cache is
  if(!cacheHit){
    if(isWrite){
      l1WriteMisses++;
    }
    else{
      l1ReadMisses++;
    }
    //If way is completely full
    if(fullNum == numberOfWays){
      //check which replacement policy and call function
      switch(replPolicy){
        case 0: //call LRU
        rank_LRU_miss_replace();
        break;
        case 1: //call FIFO
        replace_FIFO();
        break;
        case 2: //call pseudoLRU
        pseudo_miss_replace();
        break;
        case 3: //call optimal
        break;
      }

    }
    else if(fullNum < numberOfWays){
      //Way is not full, so load block into cache
      rank_LRU_miss_cold();
      pseudo_miss_cold();
      if(cache[decimalIndex][fullNum].isDirty == true){
        writeBacks++;
      }
      if(isWrite){
        cache[decimalIndex][fullNum].isDirty = true;
      }
      if(!isWrite){
        cache[decimalIndex][fullNum].isDirty = false;
      }
      cache[decimalIndex][fullNum].tag = decimalTag;
      cache[decimalIndex][fullNum].isFull = true;
      cache[decimalIndex][fullNum].LRURank = 1;
    }
  }
}

void pseudo_hit(){
  parent_loop(fullNum);
}

void pseudo_miss_cold(){
  parent_loop(fullNum);
}

void pseudo_miss_replace(){
  int lruIndex = 0;
  lruIndex = child_loop();
  cache[decimalIndex][lruIndex].tag = decimalTag;
  if(cache[decimalIndex][lruIndex].isDirty == true){
    writeBacks++;
  }
  if(isWrite){
    cache[decimalIndex][lruIndex].isDirty = true;
  }
  if(!isWrite){
    cache[decimalIndex][lruIndex].isDirty = false;
  }
  parent_loop(lruIndex);
}

void rank_LRU_hit(){
  for(int i=0; i<numberOfWays;i++){
    if(cache[decimalIndex][i].isFull && (cache[decimalIndex][i].LRURank < cache[decimalIndex][fullNum].LRURank)){
      cache[decimalIndex][i].LRURank++;
    }
  }
  cache[decimalIndex][fullNum].LRURank = 1;

}

void rank_LRU_miss_cold(){
  for(int i=0; i<numberOfWays; i++){
    if(cache[decimalIndex][i].isFull){
      cache[decimalIndex][i].LRURank++;
    }
  }
}

void rank_LRU_miss_replace(){
  int i = 0;
  while(i < numberOfWays){
    if(cache[decimalIndex][i].LRURank == numberOfWays){break;}
    i++;
  }
  for(int j=0; j < numberOfWays; j++){
    if(cache[decimalIndex][j].LRURank != numberOfWays){
      cache[decimalIndex][j].LRURank++;
    }
  }
  if(cache[decimalIndex][i].isDirty == true){
    writeBacks++;
  }
  if(isWrite){
    cache[decimalIndex][i].isDirty = true;
  }
  else{
    cache[decimalIndex][i].isDirty = false;
  }
  cache[decimalIndex][i].LRURank = 1;
  cache[decimalIndex][i].tag = decimalTag;
}

void replace_FIFO(){

  if(cache[decimalIndex][0].isDirty == true){
    writeBacks++;
  }

  //shift blocks to left
  for(int i=0; i<(numberOfWays-1); i++){
    cache[decimalIndex][i] = cache[decimalIndex][i+1];
  }
  //load block into last slot
  cache[decimalIndex][numberOfWays-1].tag = decimalTag;
  if(isWrite){
    cache[decimalIndex][numberOfWays-1].isDirty = true;
  }
  else{
    cache[decimalIndex][numberOfWays-1].isDirty = false;
  }





}
