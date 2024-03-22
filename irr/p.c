//Improved Round Robin CPU Scheduling Algorithm (Paper 1)
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Declare constants
#define COMPLETED 1
#define MINIMUM_PRIORITY_SCORE 1
#define MAXIMUM_PRIORITY_SCORE 3
#define SHORT_BURST 0.2
#define HIGH_PRIORITY 0.3
#define LOW_PRIORITY 0.2
#define PRIORITY_OFFSET 0.2
#define EMPTY 0

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
    int responseTime;
    bool cooked;
    bool arrived;
    bool timed;
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
        freshNode-> arrived = false;
        freshNode-> timed = false;
        freshNode-> next = NULL;

        // Initialise two pointers to traverse and sort linked list
        ListNodePtr prevPtr = NULL;
        ListNodePtr currentPtr = *sPtr;

        // Use insertion sort algorithm to sort the linked list by arrival time
        while(currentPtr!=NULL && arrivalTime >= currentPtr->arrivalTime){
            //In the case where the arrival time is the same, sort by process ID. Lower process ID gets priority
            if(arrivalTime == currentPtr->arrivalTime && processid < currentPtr->processID){
                prevPtr = currentPtr;
                currentPtr = currentPtr->next;
            } else {
                prevPtr = currentPtr;
                currentPtr = currentPtr->next;
            }
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
int inputIsInvalid(int *userInput){
    if(*userInput < MINIMUM_PRIORITY_SCORE || *userInput > MAXIMUM_PRIORITY_SCORE){
        printf("Invalid input. Priority must be between %d and %d\n", MINIMUM_PRIORITY_SCORE, MAXIMUM_PRIORITY_SCORE);
        return 1;   // Invalid input
    }
    return 0;       // Valid input
}

//This function checks if the input is correct
bool isValidInput(int input) {
    if (input < EMPTY) {
        printf("Invalid input. Please enter a positive integer.\n");
        return false;
    }
    return true;
}

int sumOfGantt(int *ganttTime, int *contextSwitch){
    int sum = 0;
    for(int i = 0; i < *contextSwitch; i++){
        sum += ganttTime[i];
    }
    return sum;
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
void printResults(ListNodePtr *sPtr, float *average_waiting_time, float *average_turn_around_time, int *contextSwitch){
    printf("PROCESS\t\tBURST TIME\tARRIVAL TIME\tPRIORITY|\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    ListNodePtr current = *sPtr;
    while(current != NULL){
        printf("P%d\t\t%d\t\t%d\t\t%d\t|\t%d\t\t%d\t\t\t\t%d\n", current->processID, current->burstTime, current->arrivalTime, current->priority, current->waitingTime, current->turnaroundTime, current->responseTime);
        current = current->next;
    }

    //Simply print the averages. That's about it...
    printf("Average Waiting Time: %.2f\n", *average_waiting_time);
    printf("Average Turnaround Time: %.2f\n", *average_turn_around_time);
    printf("Context Switches: %d\n", *contextSwitch);
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

//This function simulates the improved RR function by finishing off processes with low burst times
void miniBurst(ListNodePtr *sPtr, int *time, int *completedProcesses, int shortBurst, int *contextSwitch, int ganttPid[], int ganttTime[]){
    ListNodePtr current = *sPtr;
    while(current != NULL){
        if (current->remainingBurst <= shortBurst && !(current->cooked) && (current->arrivalTime <= *time)){
            printf("[Mini burst] Process %d has arrived at %d\n", current->processID, *time);
            printf("Process %d is mini bursting with %d remaining\n", current->processID, current->remainingBurst);
            *time += current->remainingBurst;
            printf("[Mini burst] time is now %d\n", *time);
            ganttPid[(*contextSwitch) + 1] = current->processID;
            ganttTime[(*contextSwitch) + 1] = current->remainingBurst;
            current->remainingBurst = COMPLETED;
            current->cooked = true;
            current->completionTime = *time;
            current->turnaroundTime = TAT(&current->completionTime, &current->arrivalTime);
            current->waitingTime = current->turnaroundTime - current->burstTime;
            (*completedProcesses)++;
            (*contextSwitch)++;
            
        }
        current = current->next;
    }

}

// This function simulates the 'Improved RR CPU Scheduling Algorithm'. Each process has its share of CPU time according to it's priority
void burst(ListNodePtr *sPtr, int *low_slice, int* medium_slice, int *high_slice, int *time, int *contextSwitch, int no_of_processes){
    //Declare variables
    int ganttPid[100];
    int ganttTime[100];
    ListNodePtr current = *sPtr;
    ListNodePtr start = *sPtr;
    int timeQuantum, cpuClock = 0, complete = 0;
    int shortBurst = *medium_slice * SHORT_BURST;

    // Iterate through the linked list and burst the processes
    while(current != NULL){
        ListNodePtr temp = start;
        
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
        printf("Process %d has remaining burst time of: %d\n", current->processID, current->remainingBurst);
        // finish off processes with low burst times
        miniBurst(&temp, time, &complete, shortBurst, contextSwitch, ganttPid, ganttTime);

        // Check if the process has finished its execution and arrived. If it finished or hasn't arrived, then move to the next process
        if((!(current->cooked)) && (current->arrivalTime <= *time)){
            if(!(current->timed)){
                current->responseTime = *time - current->arrivalTime;
                current->timed = true;
            }
            // printf("[*]Process %d has arrived at %d\n", current->processID ,*time);
            // printf("\t-->Process %d is bursting with %d remaining\n", current->processID, current->remainingBurst);
            current->remainingBurst = cpuTime(&current->remainingBurst, &timeQuantum, time, &cpuClock, &current->priority);
            // printf("After normal burst time %d\n", *time);
            (*contextSwitch)++;
            // printf("Context switch %d\n", *contextSwitch);
            ganttPid[*contextSwitch] = current->processID;
            ganttTime[*contextSwitch] = *time - sumOfGantt(ganttTime, contextSwitch);

            // If this process is completed, mark its completion time and calculate turn around time
            if ((current->remainingBurst) == COMPLETED){
                current->cooked = true;
                current->completionTime = *time;
                current->turnaroundTime = TAT(&current->completionTime, &current->arrivalTime);
                current->waitingTime = current->turnaroundTime - current->burstTime;
                complete++;
                
                if(current->next != NULL && *time < current->next->arrivalTime){
                    *time = current->next->arrivalTime;
                }
            }   
        }
        
        // Algorithm will enter here when all processes have executed to completion
        if(complete == no_of_processes){
            printf("All processes are done\n");                
            
            //Print gantt chart
            for(int i = 0; i < *contextSwitch+ 1; i++){
                printf("%d (%d)", ganttPid[i], ganttTime[i]);
                if(i != *contextSwitch){
                    printf("|   ");
                }
            }

            printf("\n\n");
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
int cpuTime(int *burstTime, int *time_quantum, int *time, int *clock, int *priority){
    
    // Checks if the burst time is less.
    if(*burstTime < *time_quantum){
        *time += *burstTime;
        *burstTime = 0;
    }

    // If not, then proceed normally
    else {
        // Implement improved Round Robin here.
        // If process is approaching completion, give it more time
        switch (*priority){
            case 3:
                if((*burstTime > *time_quantum) && (*burstTime <= (*time_quantum + (*time_quantum * HIGH_PRIORITY)))){
                    *time += *burstTime;
                    *burstTime = 0;
                }
            default:
                if((*burstTime > *time_quantum) && (*burstTime <= (*time_quantum + (*time_quantum * LOW_PRIORITY)))){
                     *time += *burstTime;
                    *burstTime = 0;
                }
        }

        if (*burstTime != EMPTY){
            *burstTime -= *time_quantum;
            *time += *time_quantum;
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
    int contextSwitch = -1;
    float average_waiting_time, average_turn_around_time;
    Node* tempPtr = NULL;
    Node* current = NULL;

        //Begin
        printf("CPU Scheduling Method: Improved Round Robin (Paper 1)\n");

        //Get the number of processes and time slice. Print out the necessary time quantum information
        getInputs(&no_of_processes, &medium_time_slice);
        low_time_slice = medium_time_slice - (medium_time_slice * PRIORITY_OFFSET);
        high_time_slice = medium_time_slice + (medium_time_slice * PRIORITY_OFFSET);
        printf("\nTime Quantum for High Priority Processes: %d\n", high_time_slice);
        printf("Time Quantum for Medium Priority Processes: %d\n", medium_time_slice);
        printf("Time Quantum for Low Priority Processes: %d\n", low_time_slice);


        //Get the arrival time burst time and priority for each process and assign it to the linked list
        for(int process = 0; process < no_of_processes; process++){
            pid = process + 1;

            do {
                printf("Enter the Burst Time for process %d: ", pid);
                scanf("%d", &burstTime);
            } while(!isValidInput(burstTime));

            do {
                printf("Enter the Arrival Time for process %d: ", pid);
                scanf("%d", &arrivalTime);
            } while(!isValidInput(arrivalTime));
            
            do {
                printf("Enter the priority for process %d [high to low, 3 to 1]: ", pid);
                scanf("%d", &priority);
            } while(inputIsInvalid(&priority));
           
            // Assign to node and link it up with existing nodes
            linkNodes(&current, pid, burstTime, arrivalTime, priority);

        }
        printf("\n");
        // Initialise temporary pointer and start bursting
        tempPtr = current;

        // Set timer to the first process' arrival time (simulate idle time skip)
        time = tempPtr->arrivalTime;
        burst(&tempPtr, &low_time_slice, &medium_time_slice, &high_time_slice, &time, &contextSwitch, no_of_processes);


        // Calculate the averages
        tempPtr = current;
        calcAverages(&tempPtr, &no_of_processes, &average_waiting_time, &average_turn_around_time);

        //Print the results
        printResults(&current, &average_waiting_time, &average_turn_around_time, &contextSwitch);
        
        //FREE THEM SLAVES (memory)
        ListNodePtr temp;
        while (current != NULL) {
            temp = current;
            current = current->next;
            free(temp);
        }
    
    return 0;
}
