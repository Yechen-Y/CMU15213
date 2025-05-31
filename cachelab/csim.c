#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

static void printfUsage();

typedef struct
{
    int validBit; // 0 is represent invalid
    int flagbit; // 标志位
    int useTimes; // 使用的次数用来实现LRU算法
} line;


// 定义一个cache结构体，用malloc()分配大小，结构体应该包括？set(指针数组？)???

// 如何读入文件内容？

// 替换策略LRU怎么实现

// 使用getopt()获取参数，以设置flag标志位，从而判断是否有必要参数遗漏

int main(int argc, char *argv[])
{   
    int opt, sFlag, EFlag, bFlag, tFlag, vFlag;
    int sNum, ENum, bNum;
    char fileName;

    // Init Flag 
    sFlag = 0;
    EFlag = 0;
    bFlag = 0;
    tFlag = 0;
    vFlag = 0; 

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
    


    printSummary(0, 0, 0);
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