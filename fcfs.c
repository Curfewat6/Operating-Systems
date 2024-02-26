//Totally natty
#include <stdio.h>


void getInputs(int *ptr_no_of_processes, int *ptr_burst_time, int *ptr_timed) {
    for (int i = 0; i < *ptr_no_of_processes; i++) {
        ptr_timed[i] = 0;
    }

    printf("Enter the number of processes: ");
    scanf("%d", ptr_no_of_processes);

    for(int i = 0; i < *ptr_no_of_processes; i++) {
        printf("Enter the Burst Time for process %d: ", i+1);
        scanf("%d", &ptr_burst_time[i]);
    }

}
void cameAndserved(int *ptr_no_of_processes, int *ptr_burst_time, int *waiting_time, int *turnaround_time, int *response_time, float *ptr_average_response_time, float *ptr_average_turn_around_time, float *ptr_average_waiting_time){
    printf("PROCESS\t\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");

    for (int process = 0; process < *ptr_no_of_processes; process++){
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\n", process, ptr_burst_time[process], waiting_time[process], turnaround_time[process], response_time[process]);
    }
    printf("The Average Waiting time: %.2f\n", *ptr_average_waiting_time);
    printf("The Average Turnaround time: %.2f\n", *ptr_average_turn_around_time);
    printf("The Average Response time: %.2f\n", *ptr_average_response_time);
}
void calcAverages(int *ptr_no_of_processes, int *ptr_waiting_time, int *ptr_turnaround_time, int *ptr_response_time, float *ptr_average_waiting_time, float *ptr_average_turn_around_time, float *ptr_average_response_time){
    float total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;
    
    // Get the average of all the timings
    for(int process = 0; process < *ptr_no_of_processes; process++){
        total_waiting_time += ptr_waiting_time[process];
        total_turnaround_time += ptr_turnaround_time[process];
        total_response_time += ptr_response_time[process];
    }

    //Calculation of the averages are here
    *ptr_average_waiting_time =  total_waiting_time / *ptr_no_of_processes;
    *ptr_average_turn_around_time =  total_turnaround_time / *ptr_no_of_processes;
    *ptr_average_response_time =  total_response_time / *ptr_no_of_processes;

}


int main(){
    int no_of_processes, burst_time[10], remaining_burst[10], waiting_time[10], turnaround_time[10], response_time[10], timed[10];
    int time = 0;
    float average_waiting_time, average_turn_around_time, average_response_time;

    printf("CPU Scheduling Method: First Come First Serve (FCFS)\nor\nCPU Scheduling Method: First In First Out (FIFO)\n\n");

    getInputs(&no_of_processes, burst_time, timed);
    

    //Start bursting
    for (int process = 0; process < no_of_processes; process++) {
        response_time[process] = time;
        waiting_time[process] = time;
        turnaround_time[process] = waiting_time[process] + burst_time[process];
        time += burst_time[process];
    }
    
    calcAverages(&no_of_processes, waiting_time, turnaround_time, response_time, &average_waiting_time, &average_turn_around_time, &average_response_time);

    cameAndserved(&no_of_processes, burst_time, waiting_time, turnaround_time, response_time, &average_response_time, &average_turn_around_time, &average_waiting_time);

    return 0;
}
