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

extern char currentAddressHex[64];
extern char currentAddressBinary[128];
extern char currentTag[64];
extern char currentIndex[64];
extern char currentOpCode[1];
extern int writeBacks;


extern long long int decimalTag;
extern int decimalIndex;
extern int l1ReadMisses;
extern int l1WriteMisses;
extern bool isWrite; //0 - r, 1 - w

char * filename;


int main(int argc, char **argv){
  unsigned int tagWidth;
  unsigned int indexWidth;
  unsigned int offsetWidth;
  l1ReadMisses = 0;
  l1WriteMisses = 0;
  writeBacks = 0;

  int l1Reads = 0;
  int l1Writes = 0;
  /*check for correct number of args*/
  if(argc != 9){
    printf("=====\nPlease enter arguments correctly. Format: \nsim_cache <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L2SIZE> <L2_ASSOC> <REPL_POLICY> <INCLUSION> <TRACE_FILE>\n=====\n");
    return 0;
  }

  FILE *out = fopen("output.txt" , "w");

  /*Print cache specifications and store args*/
  printf("\n===== Simulator configuration =====\n");
  fprintf(out, "===== Simulator configuration =====\n");
  printf("BLOCKSIZE:             %s\n",argv[1]);
  fprintf(out, "BLOCKSIZE:             %s\n",argv[1]);
  blockSize = atoi(argv[1]);
  printf("L1_SIZE:               %s\n",argv[2]);
  fprintf(out, "L1_SIZE:               %s\n",argv[2]);
  size = atoi(argv[2]);
  printf("L1_ASSOC:              %s\n",argv[3]);
  fprintf(out, "L1_ASSOC:              %s\n",argv[3]);
  assoc = atoi(argv[3]);
  printf("L2_SIZE:               %s\n",argv[4]);
  fprintf(out, "L2_SIZE:               %s\n",argv[4]);
  printf("L2_ASSOC:              %s\n",argv[5]);
  fprintf(out, "L2_ASSOC:              %s\n",argv[5]);
  replPolicy = atoi(argv[6]);
  switch (replPolicy) {
    case 0:
      printf("REPLACEMENT POLICY:    LRU\n" );
      fprintf(out, "REPLACEMENT POLICY:    LRU\n" );
      break;
    case 1:
      printf("REPLACEMENT POLICY:    FIFO\n" );
      fprintf(out, "REPLACEMENT POLICY:    FIFO\n" );
      break;
    case 2:
      printf("REPLACEMENT POLICY:    Pseudo\n" );
      fprintf(out, "REPLACEMENT POLICY:    Pseudo\n" );
      break;
    case 3:
      printf("REPLACEMENT POLICY:    optimal\n" );
      fprintf(out, "REPLACEMENT POLICY:    optimal\n" );
      break;
  }
  //TODO: Add switch case for inclusion policy
  inclPolicy = atoi(argv[7]);
  switch (inclPolicy) {
    case 0:
      printf("INCLUSION PROPERTY:    non-inclusive\n" );
      fprintf(out, "INCLUSION PROPERTY:    non-inclusive\n" );
      break;
    case 1:
      printf("INCLUSION PROPERTY:    inclusive\n" );
      fprintf(out, "INCLUSION PROPERTY:    inclusive\n" );
      break;
    case 2:
      printf("INCLUSION PROPERTY:    exclusive\n" );
      fprintf(out, "INCLUSION PROPERTY:    exclusive\n" );
      break;
  }
  filename = argv[8];
  printf("trace_file:            %s\n",argv[8]);
  fprintf(out, "trace_file:            %s\n",argv[8]);

  /*calculate the number of sets, ways, index width, offset width, and tag width */
  numberOfSets = size / (blockSize * assoc);
  numberOfWays = assoc;
  indexWidth = log2(numberOfSets);
  offsetWidth = log2(blockSize);
  tagWidth = 64 - indexWidth - offsetWidth;

  /* Create Cache instance */
  cache = calloc(numberOfSets, sizeof(struct Block *));
  for(int i=0; i< numberOfSets; i++){
    cache[i] = calloc(numberOfWays, sizeof(struct Block));
  }

  binTreeArray = (int**)calloc(numberOfSets, sizeof(int *));
  for(int i=0; i< numberOfSets; i++){
    binTreeArray[i] = (int*)calloc(numberOfWays-1, sizeof(int ));
  }

  /* Open trace file */
  FILE *fp = fopen(filename, "r");
  /* Process reads and writes into the cache */
  char buff[64];

  //========== Main Loop ===============
  while(fgets(buff, sizeof buff, fp) != NULL){
    //Clear all arrays for next memory location
    memset(currentAddressBinary, 0, 128);
    /* seperate op code and address */
    currentOpCode[0] = buff[0];

    //Fill address array
    int i = 2;
    int j = 0;
    while(i < 64){
      currentAddressHex[j] = buff[i];
      i++;
      j++;
    }

    //Count Reads and Writes
    switch(currentOpCode[0]){
      case 'r':
        isWrite = false;
        l1Reads++;
        break;
      case 'w':
        isWrite = true;
        l1Writes++;
        break;
    }
    for(int i=0; i<64; i++){
      if(currentAddressHex[i] == '\n')
        currentAddressHex[i] = '\0';
    }

    //Add leading zeros to binary address
    add_zeros();
    //Convert address to binary
    hex_to_bin();
    //Parse the address
    parse_binary(tagWidth, indexWidth);
    //Convert binary tag to int
    decimalTag = strtol(currentTag, NULL, 2);
    //Convert binary index to int
    decimalIndex = strtol(currentIndex, NULL, 2);
    //Process cache request
    cache_process();

  }
  //========== End Main Loop =============

  printf("===== Simulation results (raw) =====\n");
  fprintf(out, "===== Simulation results (raw) =====\n");
  printf("a. number of L1 reads:        %d\n" , l1Reads);
  fprintf(out, "a. number of L1 reads:        %d\n" , l1Reads);
  printf("b. number of L1 read misses:  %d\n" , l1ReadMisses);
  fprintf(out, "b. number of L1 read misses:  %d\n" , l1ReadMisses);
  printf("c. number of L1 writes:       %d\n" , l1Writes);
  fprintf(out, "c. number of L1 writes:       %d\n" , l1Writes);
  printf("d. number of L1 write misses: %d\n" , l1WriteMisses);
  fprintf(out, "d. number of L1 write misses: %d\n" , l1WriteMisses);
  printf("e. L1 miss rate:              %f\n", (float) (l1ReadMisses + l1WriteMisses) / (l1Reads + l1Writes));
  fprintf(out, "e. L1 miss rate:              %f\n", (float) (l1ReadMisses + l1WriteMisses) / (l1Reads + l1Writes));
  printf("f. number of L1 writebacks:   %d\n" , writeBacks);
  fprintf(out, "f. number of L1 writebacks:   %d\n" , writeBacks);
  printf("g. number of L2 reads:        0\n");
  fprintf(out, "g. number of L2 reads:        0\n");
  printf("h. number of L2 read misses:  0\n");
  fprintf(out, "h. number of L2 read misses:  0\n");
  printf("i. number of L2 writes:       0\n");
  fprintf(out, "i. number of L2 writes:       0\n");
  printf("j. number of L2 write misses: 0\n");
  fprintf(out, "j. number of L2 write misses: 0\n");
  printf("k. L2 miss rate:              0\n");
  fprintf(out, "k. L2 miss rate:              0\n");
  printf("l. number of L2 writebacks:   0\n" );
  fprintf(out, "l. number of L2 writebacks:   0\n" );
  printf("m. total memory traffic:      %d\n" , l1ReadMisses + l1WriteMisses + writeBacks );
  fprintf(out, "m. total memory traffic:      %d\n" , l1ReadMisses + l1WriteMisses + writeBacks );
  printf("\nResults also written to output.txt for validation\n\n");

  /* End program */
  fclose(fp);
  fclose(out);
  free(cache);
  free(binTreeArray);
  return 0;

}
