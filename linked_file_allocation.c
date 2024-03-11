//OS LAB Linked File Allocation
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

//Define constants
#define MAX_BLOCKS 32
#define BLOCK_SIZE 1
#define MAX_FILES 24
#define MAX_LENGTH 100

//This is the linked list where each linked list contains all the data blocks of a file.
// 1 file = 1 linked list
typedef struct LinkedList{
    int blockNumber;
    struct LinkedList *next;
} Node;

typedef Node *ListNodePtr;

//Just simply print the thing...
void printIntro(){
    printf("File allocation method: LINKED LIST");
    printf("Total blocks: %d\n", MAX_BLOCKS);
    printf("File allocation start at block: 8\n");
    printf("File allocation end at block: 31\n");
    printf("Size (kB) of each block: %d\n", BLOCK_SIZE);
}

//Check if that datablock is free
bool blockIsUnavailable(int value, int *available_blocks){
    for(int numbers = 0; numbers < MAX_BLOCKS; numbers++){
        if(available_blocks[numbers] == value){
            return false;
        }
    }
    return true;
}

void getInputs(int *no_of_files){
    printf("Enter the number of files: ");
    scanf("%d", no_of_files);
    getchar();
}

void linkTheList(ListNodePtr *sPtr, char fileName, int fileSize, int *available_blocks){
    int randomNumber;
    int n = MAX_FILES;

    while(fileSize != 0){

        do{
            randomNumber = rand() % 24 + 8;
            //printf("random number: %d\n", randomNumber);
        } while(blockIsUnavailable(randomNumber, available_blocks));

        int index = -0.2;
      //  printf("n: %d\n", n);
        //Get the index of the chosen number in available blocks array
        for(int i = 0; i < n; i++){
            if(available_blocks[i] == randomNumber){
                index = i;
               // printf("removing block %d\n", available_blocks[i]);
                break;
            }
        }
        
        //Erase it from the available blocks array
        if (index != -0.2){
            for (int j = index; j < n - 1; j++){
                available_blocks[j] = available_blocks[j+1];
            }
            n--;
        }

        ListNodePtr freshNode = malloc(sizeof(Node));
        if(freshNode != NULL){
            freshNode->blockNumber = randomNumber;
            freshNode->next = NULL;

            freshNode->next = *sPtr;
            *sPtr = freshNode;
        } else {
            printf("failed to insert. No memory available for the moment.\n");
        }
        fileSize--;
        
    }
    
}

//Empty memory
void freeList(Node* head) {
    Node* temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Print results
void printList(Node* head) {
    if (head == NULL) {
        return;
    }
    printList(head->next);
    if (head->next != NULL) {
        printf("%d-", head->blockNumber);
    } else {
        printf("%d", head->blockNumber);
    }
}

int main(){
    //Declare and initialise variables
    srand(time(0));
    int fileCount, fileSize;
    int dataBlock[MAX_FILES] = {8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
    int size_array[MAX_FILES];
    char fileName[MAX_LENGTH];
    char fileNames[MAX_FILES][MAX_LENGTH];
    Node* storage[MAX_FILES];
    Node* tempPtr = NULL;
    
    printIntro();
    printf("\n"); // Just echoing a linefeed here
    //Get inputs
    getInputs(&fileCount);
    printf("\n"); // Just echoing a linefeed here

    // Start allocating the files
    for(int file = 0; file < fileCount; file++){
        Node* head = NULL;
        printf("Enter the name of file #%d: ", file+1);
        fgets(fileName, MAX_LENGTH, stdin);
        fileName[strcspn(fileName, "\n")] = 0;
        strcpy(fileNames[file], fileName);
        printf("Enter the size (kB) of file #%d: ", file+1);
        scanf("%d", &fileSize);
        getchar();
        size_array[file] = fileSize;
        printf("\n"); // Just echoing a linefeed here
        linkTheList(&head, fileName, fileSize, dataBlock);
        storage[file] = head;
    }


    // print results
    printf("FILE_NAME\tNO_OF_BLOCKS\tBLOCKS_OCCUPIED\n");
    for(int file = 0; file < fileCount; file++){
        printf("%s\t\t%d\t", fileNames[file], size_array[file]);  // Print the file name from the array
        printList(storage[file]);
        printf("\n");
    }

    for(int file = 0; file < fileCount; file++){
        freeList(storage[file]);
    }
    
    return 0;
}
