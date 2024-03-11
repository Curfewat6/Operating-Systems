#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

void getInputs(int *ptr_no_of_processes, int *ptr_time_quantum, int *ptr_burst_time, int *ptr_arrival_time) {
    printf("Enter the number of processes: ");
    scanf("%d", ptr_no_of_processes);

    for(int i = 0; i < *ptr_no_of_processes; i++) {
        printf("Enter Arrival Time for process %d: ", i+1);
        scanf("%d", &ptr_arrival_time[i]);
        printf("Enter Burst Time for process %d: ", i+1);
        scanf("%d", &ptr_burst_time[i]);
    }

    printf("Enter the size of time slice: ");
    scanf("%d", ptr_time_quantum);
}

void calcAverages(int *ptr_no_of_processes, int *ptr_waiting_time, int *ptr_turnaround_time, int *ptr_response_time, float *ptr_average_waiting_time, float *ptr_average_turn_around_time, float *ptr_average_response_time){
    float total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;
    
    for(int process = 0; process < *ptr_no_of_processes; process++){
        total_waiting_time += ptr_waiting_time[process];
        total_turnaround_time += ptr_turnaround_time[process];
        total_response_time += ptr_response_time[process];
    }

    *ptr_average_waiting_time = total_waiting_time / *ptr_no_of_processes;
    *ptr_average_turn_around_time = total_turnaround_time / *ptr_no_of_processes;
    *ptr_average_response_time = total_response_time / *ptr_no_of_processes;
}

void printResults(int *ptr_no_of_processes, int *ptr_burst_time, int *ptr_waiting_time, int *ptr_turnaround_time,int *ptr_arrival_time, int *ptr_response_time, float *ptr_average_waiting_time, float *ptr_average_turn_around_time, float *ptr_average_response_time){
    printf("PROCESS\t\tARRIVAL TIME\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    for (int process = 0; process < *ptr_no_of_processes; process++){
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", process+1, ptr_arrival_time[process], ptr_burst_time[process], ptr_waiting_time[process], ptr_turnaround_time[process], ptr_response_time[process]);
    }
    printf("Average Waiting time: %.2f\n", *ptr_average_waiting_time);
    printf("Average Turnaround time: %.2f\n", *ptr_average_turn_around_time);
    printf("Average Response time: %.2f\n", *ptr_average_response_time);
}

int main() {
    int no_of_processes, time_quantum, burst_time[10], arrival_time[10], remaining_burst[10], waiting_time[10] = {0}, turnaround_time[10] = {0}, response_time[10], currentTime = 0, completed = 0, lastProcess = -1;
    float average_waiting_time, average_turn_around_time, average_response_time;
    bool isSelected[10] = {false};

    printf("CPU Scheduling Method: Round Robin (RR)\n");
    getInputs(&no_of_processes, &time_quantum, burst_time, arrival_time);

    for(int i = 0; i < no_of_processes; i++) {
        remaining_burst[i] = burst_time[i];
    }

    while(completed < no_of_processes) {
        int idx = -1;
        int mn = INT_MAX;

        for(int i = 0; i < no_of_processes; i++) {
            if(arrival_time[i] <= currentTime && remaining_burst[i] > 0) {
                if(!isSelected[i] || (lastProcess == i && isSelected[i])) {
                    if(remaining_burst[i] < mn) {
                        mn = remaining_burst[i];
                        idx = i;
                    }
                    if(lastProcess == -1) {
                        lastProcess = i;
                    }
                }
            }
        }

        if(idx != -1) {
            if(!isSelected[idx]) {
                response_time[idx] = currentTime - arrival_time[idx];
                isSelected[idx] = true;
            }

            if(remaining_burst[idx] <= time_quantum) {
                currentTime += remaining_burst[idx];
                waiting_time[idx] = currentTime - burst_time[idx] - arrival_time[idx];
                turnaround_time[idx] = currentTime - arrival_time[idx];
                remaining_burst[idx] = 0;
                completed++;
                lastProcess = -1;
            } else {
                remaining_burst[idx] -= time_quantum;
                currentTime += time_quantum;
                lastProcess = idx;
            }
        } else {
            currentTime++;
        }
    }

    calcAverages(&no_of_processes, waiting_time, turnaround_time, response_time, &average_waiting_time, &average_turn_around_time, &average_response_time);
    printResults(&no_of_processes, burst_time, waiting_time, turnaround_time, arrival_time, response_time, &average_waiting_time, &average_turn_around_time, &average_response_time);

    return 0;
}
