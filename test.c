//Improved Round Robin CPU Scheduling Algorithm (Paper 1)
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Declare constants
#define minimumPriorityScore 1
#define maximumPriorityScore 3
#define highPriority 0.3
#define lowPriority 0.2
#define priorityOffset 0.2

// This is the structure of the linkedlist
typedef struct LinkedList{
    int processID;
    int priority;
    int arrivalTime;
    int burstTime;
    int remainingBurst;
    int waitingTime;
    int turnaroundTime;
    int completionTime;
    bool cooked;
    int timed;
    struct LinkedList *next;
} Node;

typedef Node *ListNodePtr;

//This function gets the number of processes and the time quantum from the user
void getInputs(int *ptr_no_of_processes, int *ptr_time_quantum){
    printf("Enter the number of processes: ");
    scanf("%d", ptr_no_of_processes);
    printf("Enter Time Quantum\n");
    scanf("%d", ptr_time_quantum);
}

//This function creates nodes everytime a new process is input and links it up with existing nodes, sorted by arrival time
void linkNodes(ListNodePtr *sPtr, int processid, int burstTime, int arrivalTime, int priority){
    // Allocate memory for the node
    ListNodePtr freshNode = malloc(sizeof(Node));

    // Check if there is available memory in the first place
    if (freshNode != NULL){
        // Assign the respective values to the node
        freshNode-> processID = processid;
        freshNode-> priority = priority;
        freshNode-> arrivalTime = arrivalTime;
        freshNode-> burstTime = burstTime;
        freshNode-> remainingBurst = burstTime;
        freshNode-> cooked = false;
        freshNode-> timed = 0;
        freshNode-> next = NULL;

        // Initialise two pointers to traverse and sort linked list
        ListNodePtr prevPtr = NULL;
        ListNodePtr currentPtr = *sPtr;

        while(currentPtr!=NULL && arrivalTime > currentPtr->arrivalTime){
            prevPtr = currentPtr;
            currentPtr = currentPtr->next;
        }

        if (prevPtr == NULL){
            freshNode->next = *sPtr;
            *sPtr = freshNode;
        } else {
            prevPtr->next = freshNode;
            freshNode->next = currentPtr;
        }
    } else {
        printf("Process %d failed to insert. No memory available for the moment.\n", processid);
    }
}

//This function checks if the input is correct
int inputIsInvalid(int *ptr_of_priority){
    if(*ptr_of_priority < minimumPriorityScore || *ptr_of_priority > maximumPriorityScore){
        return 1;   // Invalid input
    }
    return 0;
}

//This function calculates the average waiting time and average turn around time
void calcAverages(ListNodePtr *sPtr, int *no_of_processes, float *average_waiting_time, float *average_turn_around_time){
    float total_waiting_time = 0, total_turnaround_time = 0;
    ListNodePtr current = *sPtr;
    while(current!=NULL){
        total_waiting_time += current->waitingTime;
        total_turnaround_time += current->turnaroundTime;
        current = current->next;
    }
    *average_waiting_time = total_waiting_time / *no_of_processes;
    *average_turn_around_time = total_turnaround_time / *no_of_processes;

}

//This function prints the final table results
void printResults(ListNodePtr *sPtr, float *average_waiting_time, float *average_turn_around_time){
    printf("PROCESS\t\tBURST TIME\tARRIVAL TIME\tPRIORITY|\tWAITING TIME\tTURNAROUND TIME\n");
    ListNodePtr current = *sPtr;
    while(current != NULL){
        printf("P%d\t\t%d\t\t%d\t\t%d\t|\t%d\t\t%d\n", current->processID, current->burstTime, current->arrivalTime, current->priority, current->waitingTime, current->turnaroundTime);
        current = current->next;
    }

    printf("Average Waiting Time: %.2f\n", *average_waiting_time);
    printf("Average Turnaround Time: %.2f\n", *average_turn_around_time);
}

//This function calculates the turn around time
int TAT(int* completionTime, int *arrivalTime){
    return *completionTime - *arrivalTime;
}

//This function is used to perform corrections on the wait time
void finaliseWait(ListNodePtr *sPtr){
    ListNodePtr current = *sPtr;
    while(current!=NULL){
        current-> waitingTime -= current->arrivalTime;
        current = current->next;
    }
}

//This function calculates the waiting time for the processes during execution. Not after
void waitUp(ListNodePtr *sPtr, int *time, int *pid, bool *completed){
    ListNodePtr current = *sPtr;
    while(current != NULL){
        if(current->processID != *pid && !(current->cooked)){
            current->waitingTime += *time;
        }
        current = current->next;
    }
}

// This function simulates the 'Improved RR CPU Scheduling Algorithm'. Each process has its share of CPU time according to it's priority
//TODO: implement the pre-emption for the priority [Not done]
//TODO: Calculate turn around time during execution [NOT DONE]
void burst(ListNodePtr *sPtr, int *low_slice, int* medium_slice, int *high_slice, int *time, int no_of_processes){

    //Declare variables
    ListNodePtr current = *sPtr;
    ListNodePtr start = *sPtr;
    int timeQuantum, cpuClock = 0, complete = 0;

    // Iterate through the linked list and burst the processes
    while(current != NULL){
        ListNodePtr temp = current;
        
        // Assign time quantums based on the priority
        switch(current->priority){
            case 1:
                timeQuantum = *low_slice;
                break;
            case 2:
                timeQuantum = *medium_slice;
                break;
            case 3:
                timeQuantum = *high_slice;
                break;
        }
        
        // Check if the process has finished its execution. If it has, then move to the next process
        if(!(current->cooked)){
            // Dispatch the process from ready queue to CPU for time quantum. Other process wait.
            current->remainingBurst = cpuTime(&current->remainingBurst, &timeQuantum, time, &cpuClock, &current->timed, &current->priority);
            waitUp(&temp, &cpuClock, &current->processID, &current->cooked);

            // If this process is completed, mark its completion time and calculate turn around time
            if ((current->remainingBurst) == 1){
                current->cooked = true;
                current->completionTime = *time;
                current->turnaroundTime = TAT(&current->completionTime, &current->arrivalTime);
                complete++;
            } else {
            }
        }

        // Algorithm will enter here when all processes have executed to completion
        if(complete == no_of_processes){
            printf("All processes are done\n");
            finaliseWait(&start);
            break;
        }

        // Go next
        if(current->next == NULL){
            current = start;
        } else {
            current = current->next;
        }
    }
}

// This function simulates the CPU time for each process during the relative time quantum
int cpuTime(int *burstTime, int *time_quantum, int *time, int *clock, int *timed, int *priority){

    // Checks if the burst time is less.
    if(*burstTime < *time_quantum){
        *time += *burstTime;
        *burstTime = 0;
    } 
    // If not, then proceed normally
    else {
        *burstTime -= *time_quantum;
        *time += *time_quantum;

        // Implement improved Round Robin here.
        // If process is approaching completion, give it more time
        switch (*priority){
            case 1:
                if((*burstTime > *time_quantum) && (*burstTime <= (*time_quantum + (*time_quantum * highPriority)))){
                    *time += *burstTime;
                    *burstTime -= *time_quantum;
                }
            default:
                if((*burstTime > *time_quantum) && (*burstTime <= (*time_quantum + (*time_quantum * lowPriority)))){
                    *time += *burstTime;
                    *burstTime -= *time_quantum;
                }
        }
    }

    //Calculate time spent in the CPU
    *clock = *time - *clock;
    if (*burstTime <= 0) {
        return 1;
    }
    return *burstTime;
}

int main(){
    //Declare variables and pointers
    int pid, no_of_processes, burstTime, arrivalTime, priority, low_time_slice, medium_time_slice, high_time_slice;
    int time = 0;
    int valid = 1;
    float average_waiting_time, average_turn_around_time;
    Node* tempPtr = NULL;
    Node* current = NULL;

        //Begin
        printf("CPU Scheduling Method: Improved Round Robin (Paper 1)\n");

        //Get the number of processes and time slice. Print out the necessary time quantum information
        getInputs(&no_of_processes, &medium_time_slice);
        low_time_slice = medium_time_slice - (medium_time_slice * priorityOffset);
        high_time_slice = medium_time_slice + (medium_time_slice * priorityOffset);
        printf("\nTime Quantum for High Priority Processes: %d\n", high_time_slice);
        printf("Time Quantum for Medium Priority Processes: %d\n", medium_time_slice);
        printf("Time Quantum for Low Priority Processes: %d\n", low_time_slice);


        //Get the arrival time burst time and priority for each process and assign it to the linked list
        for(int process = 0; process < no_of_processes; process++){
            pid = process + 1;
            printf("Enter the Burst Time for process %d: ", pid);
            scanf("%d", &burstTime);
            printf("Enter the Arrival Time for process %d: ", pid);
            scanf("%d", &arrivalTime);
            printf("Enter the priority for process %d [high to low, 3 to 1]: ", pid);
            scanf("%d", &priority);
            if(inputIsInvalid(&priority)){
                printf("Priority needs to be from 1-3. Please try again.\n");
                return 0;
            }
            // Assign to node and link it up with existing nodes
            linkNodes(&current, pid, burstTime, arrivalTime, priority);
        }

        // Initialise temporary pointer and start bursting
        tempPtr = current;
        burst(&tempPtr, &low_time_slice, &medium_time_slice, &high_time_slice, &time, no_of_processes);


        // Calculate the averages
        tempPtr = current;
        calcAverages(&tempPtr, &no_of_processes, &average_waiting_time, &average_turn_around_time);

        //Print the results
        printResults(&current, &average_waiting_time, &average_turn_around_time);
        
        //FREE THEM SLAVES (memory)
        ListNodePtr temp;
        while (current != NULL) {
            temp = current;
            current = current->next;
            free(temp);
        }
    
    return 0;
}
