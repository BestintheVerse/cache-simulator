

typedef int bool;
enum { false, true };

unsigned int blockSize;
unsigned int size;
unsigned int assoc;
unsigned int replPolicy;
unsigned int inclPolicy;
unsigned int numberOfSets;
unsigned int numberOfWays;
char currentTag[64];
char currentIndex[64];

char currentAddressHex[64];
char currentAddressBinary[128];
char currentOpCode[1];
int writeBacks;

int **binTreeArray;

long long int decimalTag;
int decimalIndex;
int l1ReadMisses;
int l1WriteMisses;
bool isWrite; //0 - r, 1 - w

struct Block {
  unsigned long long int tag;
  unsigned int LRURank;
  bool isDirty;
  bool isFull;
};

struct Block **cache;

void hex_to_bin();
void add_zeros();
void parse_binary(int tagWidth, int indexWidth);
void cache_process();
void replace_FIFO();
void rank_LRU_hit();
void rank_LRU_miss_cold();
void rank_LRU_miss_replace();
void parent_loop(int i);
int child_loop();
void pseudo_hit();
void pseudo_miss_cold();
void pseudo_miss_replace();
