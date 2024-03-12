#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Declare Constants
#define TOTAL_BLOCKS 32
#define START_BLOCK 8
#define END_BLOCK 31
#define MAX_FILES (END_BLOCK - START_BLOCK + 1)

//This is the structure of the file
typedef struct {
    char name[50];
    int size;
    int blockCount;
    int blocks[MAX_FILES];
} File;

int disk[TOTAL_BLOCKS] = {0}; // 0 indicates block is free

void allocateFile(File *file) {
    int neededBlocks = file->size;
    file->blockCount = 0;

    while (neededBlocks > 0) {
        int block = rand() % (END_BLOCK - START_BLOCK + 1) + START_BLOCK;
        if (disk[block] == 0) {
            disk[block] = 1; 
            file->blocks[file->blockCount++] = block;
            neededBlocks--;
        }
    }
}

int main() {
    int numFiles;
    srand(time(NULL));

    printf("File allocation method: INDEXED\n");
    printf("Total blocks: %d\n", TOTAL_BLOCKS);
    printf("File allocation start at block: %d\n", START_BLOCK);
    printf("File allocation end at block: %d\n", END_BLOCK);
    printf("Size (kB) of each block: 1\n\n");

    
    printf("Enter no of files: ");
    scanf("%d", &numFiles);

    File files[MAX_FILES] = {{0}};

    for (int i = 0; i < numFiles; i++) {
        printf("Enter the name of file #%d: ", i + 1);
        scanf("%s", files[i].name);
        printf("Enter the size (kB) of file #%d: ", i + 1);
        scanf("%d", &files[i].size);

        if (files[i].size < 1 || files[i].size > (END_BLOCK - START_BLOCK + 1)) {
            fprintf(stderr, "Error: File size must be between 1 and %d\n", (END_BLOCK - START_BLOCK + 1));
            return EXIT_FAILURE;
        }

        allocateFile(&files[i]);
    }

    printf("\nFILE_NAME\tNO_OF_BLOCKS\tBLOCKS_OCCUPIED\n");
    for (int i = 0; i < numFiles; i++) {
        printf("%s\t%d\t\t", files[i].name, files[i].blockCount);
        for (int j = 0; j < files[i].blockCount; j++) {
            printf("%d", files[i].blocks[j]);
            if (j < files[i].blockCount - 1) {
                printf("-");
            }
        }
        printf("\n");
    }

    printf("\nFile allocation completed. Exiting.\n");
    return 0;
}
