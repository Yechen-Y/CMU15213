#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    unsigned validBit; // 0 is represent invalid
    unsigned tagBit; // 标志位
    unsigned useTimes; // 使用的次数用来实现LRU算法
} line;
typedef line* set;
typedef set* cache;

static void printfUsage();
static int checkCorrect();
static void initCache();
static void freeCache();
static void readFile();
static void loadAndStroeCache(size_t address, int isModify);
static void modifyCache(size_t address);


static int opt, sFlag, EFlag, bFlag, tFlag, vFlag, time;
static int sNum, ENum, bNum;
static int hits, misses, evictions; 
static char* fileName;
static cache myCache;






// 定义一个cache结构体，用malloc()分配大小，结构体应该包括？set(指针数组？)???

// 如何读入文件内容？

// 替换策略LRU怎么实现

// 使用getopt()获取参数，以设置flag标志位，从而判断是否有必要参数遗漏

// 在计算setNum时可以优化

int main(int argc, char *argv[])
{   
    // Init Flag 
    sFlag = 0;
    EFlag = 0;
    bFlag = 0;
    tFlag = 0;
    vFlag = 0; 
    hits = 0;
    misses = 0;
    evictions = 0;
    time = 0;

    // Getopt
    while((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (opt)
        {
        case 'v':
            vFlag = 1;
            break;
        case 's':
            sFlag = 1;
            sNum = atoi(optarg);
            break;
        case 'E':
            EFlag = 1;
            ENum = atoi(optarg);
            break;
        case 'b':
            bFlag = 1;
            bNum = atoi(optarg);
            break;
        case 't':
            tFlag = 1;
            fileName = optarg;
            break;
        case 'h':
        default:
            printfUsage();
            exit(0); // exit the program successfully
            break;
        }
    }

    // check getopt correctness
    if (checkCorrect() == 0) {
        printfUsage();
        exit(0); // exit the program successfully
    }
    initCache();
    readFile();


    printSummary(hits, misses, evictions);
    freeCache();
    return 0;
}


static void printfUsage() {
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

static int checkCorrect() {
    return (sFlag & EFlag & bFlag & tFlag) == 1;
}

static void initCache() {
    unsigned setNum = 1 << sNum;
    myCache = (cache)malloc(setNum * sizeof(set));
    for (int count = 0; count < setNum; count++) {
        myCache[count] = (set)malloc(ENum * sizeof(line));
        for (int count2 = 0; count2 < ENum; count2++) {
            myCache[count][count2].validBit = 0;
            myCache[count][count2].useTimes = time;
            myCache[count][count2].tagBit = 0;
        }
    }

}

static void freeCache() {
    unsigned setNum = 1 << sNum;
    for (int count = 0; count < setNum; count++) {
        free(myCache[count]);
    }
    free(myCache);
}

static void readFile() {
    char operation;
    size_t address;
    int size;
    
    FILE *fptr = fopen(fileName, "r");
    while(fscanf(fptr, " %c %lx,%d", &operation, &address, &size) == 3) {
        time += 1;
        switch (operation)
        {
        case 'I':
            break;
        case 'L':
            loadAndStroeCache(address,0);
            break;
        case 'S':
            loadAndStroeCache(address,0);
            break;
        case 'M': // include load and store
            modifyCache(address);
            break;
        default:
            break;
        }
    }
}

static void loadAndStroeCache(size_t address, int isModify) {
    unsigned setNum = ((address >> bNum) & ((1 << sNum) - 1)); //根据地址决定Cache的组数
    unsigned tag = (address >> (sNum + bNum)); //根据地址决定当前地址的tag

    for(int count = 0; count < ENum; count++) {
        // hit situation
        if (myCache[setNum][count].validBit == 1 && myCache[setNum][count].tagBit == tag){
            myCache[setNum][count].useTimes = time;
            hits += 1;
            if (vFlag) {
                if (isModify) {
                    printf("hit ");
                } else {
                    printf("hit\n");
                }
            }
            return;
        }
    }

    //miss situation
    //如果有空位则插入否则替换useTimes最少的那个line
    unsigned lessUseNum = 0;
    unsigned lessUse = myCache[setNum][0].useTimes;
    misses += 1;
    for (int count = 0; count < ENum; count++) {

        if (myCache[setNum][count].validBit == 0) {
            myCache[setNum][count].validBit = 1;
            myCache[setNum][count].useTimes = time;
            myCache[setNum][count].tagBit = tag;
            if (vFlag) {
                if (isModify) {
                    printf("miss ");
                } else {
                    printf("miss\n");
                }
            }
            return;
        }
        if (myCache[setNum][count].useTimes < lessUse) {
            lessUseNum = count;
            lessUse = myCache[setNum][lessUseNum].useTimes;
        }
    }
    evictions += 1;
    myCache[setNum][lessUseNum].tagBit = tag;
    myCache[setNum][lessUseNum].useTimes = time;
    if(vFlag) {
        if(isModify) {
            printf("miss eviction ");
        } else {
            printf("miss eviction\n");
        }
    }

}

static void modifyCache(size_t address) {
    loadAndStroeCache(address, 1);
    hits += 1;
    printf("hit\n");
}
