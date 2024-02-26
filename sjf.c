//Non pre-emptive Shortest Job First (SJF) 40% natty
#include <stdio.h>
#include <stdlib.h>

typedef struct LinkedList{
    int processID;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
    struct LinkedList *next;
} Node;

typedef Node *ListNodePtr;
void linkMeUp(ListNodePtr *sPtr, int processid, int burst_time);

void getInputs(int *ptr_no_of_processes){
    //Similar to FCFS/FIFO, SJF does not need time slice

    printf("Enter the number of processes: ");
    scanf("%d", ptr_no_of_processes);

}

//This function creates a new node and inserts it to the link list. It also sorts based on the burst time.
void linkMeUp(ListNodePtr *sPtr, int processid, int burst_time){
    ListNodePtr freshNode = malloc(sizeof(Node));
    if(freshNode != NULL){
        freshNode->processID = processid;
        freshNode->burstTime = burst_time;
        freshNode->next = NULL;

        ListNodePtr prevPtr = NULL;
        ListNodePtr currentPtr = *sPtr;

        //Sort based on the burst time. Keep going until this node's burst time is shorter than the current one
        while(currentPtr != NULL && burst_time > currentPtr->burstTime){
            prevPtr = currentPtr;
            currentPtr = currentPtr->next;
        }
        // come here if no need to sort
        if(prevPtr == NULL){
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

//This function calculates the average waiting time, average turnaround time and average response time
void calcAverages(ListNodePtr *sPtr, int *no_of_processes, float *average_waiting_time, float *average_turn_around_time, float *average_response_time){
    float total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;

    ListNodePtr current = *sPtr;
    while(current!=NULL){
        total_waiting_time += current->waitingTime;
        total_turnaround_time += current->turnaroundTime;
        total_response_time += current->responseTime;
        current = current->next;
    }

    //Calculation of the averages are here
    *average_waiting_time =  total_waiting_time / *no_of_processes;
    *average_turn_around_time =  total_turnaround_time / *no_of_processes;
    *average_response_time =  total_response_time / *no_of_processes;

}

//This function prints the results
void printIt(ListNodePtr *sPtr, float *ptr_average_response_time, float *ptr_average_turn_around_time, float *ptr_average_waiting_time){
    ListNodePtr current = *sPtr;
    printf("PROCESS\t\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    while(current != NULL){
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\n", current->processID, current->burstTime, current->waitingTime, current->turnaroundTime, current->responseTime);
        current = current->next;
    }

    printf("The Average Waiting time: %.2f\n", *ptr_average_waiting_time);
    printf("The Average Turnaround time: %.2f\n", *ptr_average_turn_around_time);
    printf("The Average Response time: %.2f\n", *ptr_average_response_time);
}

void main(){

    // Declare variables
    int no_of_processes;
    int time = 0;
    float average_waiting_time, average_turn_around_time, average_response_time;
    Node* tempPtr = NULL;
    Node* current = NULL;
    printf("CPU Scheduling Method: Shortest Job First (SJF)\n");

    getInputs(&no_of_processes);

    // Get the burst time for each process and sort it based on the burst time. Shortest goes to the head
    for(int process = 0; process < no_of_processes; process++){
        int pid = process + 1;
        printf("Enter the Burst Time for process %d: ", pid);
        int burstTime;
        scanf("%d", &burstTime);
        linkMeUp(&current, pid, burstTime);
    }
    // Start bursting
    tempPtr = current;
    while(tempPtr!=NULL){
        // Since arrival time of all processes are 0, the waiting time and response time will be the same
        tempPtr-> waitingTime = time;
        tempPtr -> responseTime = time;
        tempPtr-> turnaroundTime = tempPtr->burstTime + tempPtr->waitingTime;
        time += tempPtr->burstTime;
        tempPtr = tempPtr->next;
    }
    
    tempPtr = current;
    // Calculate the averages
    calcAverages(&tempPtr, &no_of_processes, &average_waiting_time, &average_turn_around_time, &average_response_time);

    // Print the results
    printIt(&current, &average_response_time, &average_turn_around_time, &average_waiting_time);
    //Free the memory
    ListNodePtr temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}
