//100% natty code
#include <stdio.h>
#include <stdbool.h>

void getInputs(int *ptr_no_of_processes, int *ptr_time_quantum, int *ptr_burst_time, int *ptr_timed) {
    for (int i = 0; i < *ptr_no_of_processes; i++) {
        ptr_timed[i] = 0;
    }

    printf("Enter the number of processes: ");
    scanf("%d", ptr_no_of_processes);

    for(int i = 0; i < *ptr_no_of_processes; i++) {
        printf("Enter the Burst Time for process %d: ", i+1);
        scanf("%d", &ptr_burst_time[i]);
    }

    printf("Enter the size of time slice: ");
    scanf("%d", ptr_time_quantum);

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
    *ptr_average_waiting_time = total_waiting_time / *ptr_no_of_processes;
    *ptr_average_turn_around_time = total_turnaround_time / *ptr_no_of_processes;
    *ptr_average_response_time = total_response_time / *ptr_no_of_processes;
}

void turnMeAround(int *ptr_no_of_processes, int *ptr_burst_time, int *turn_around_time, int *waiting_time){
    for (int process = 0; process < *ptr_no_of_processes; process++) {
        turn_around_time[process] = ptr_burst_time[process] + waiting_time[process];
    }
}

void printVictoryBanner(int *ptr_no_of_processes, int *ptr_burst_time, int *waiting_time, int *turnaround_time, int *response_time, float *ptr_average_response_time, float *ptr_average_turn_around_time, float *ptr_average_waiting_time){
    printf("PROCESS\t\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    for (int process = 0; process < *ptr_no_of_processes; process++){
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\n", process+1, ptr_burst_time[process], waiting_time[process], turnaround_time[process], response_time[process]);
    }
    printf("The Average Waiting time: %.2f\n", *ptr_average_waiting_time);
    printf("The Average Turnaround time: %.2f\n", *ptr_average_turn_around_time);
    printf("The Average Response time: %.2f\n", *ptr_average_response_time);
}

int main() {
    int no_of_processes, time_quantum, burst_time[10], remaining_burst[10], waiting_time[10], turnaround_time[10], response_time[10], timed[10];
    int time = 0;
    float average_waiting_time, average_turn_around_time, average_response_time;
    bool notBursted = true;

    printf("CPU Scheduling Method: Round Robin (RR)\n");
    getInputs(&no_of_processes, &time_quantum, burst_time, timed);

    // Copy the burst time to remaining burst time
    for(int i = 0; i < no_of_processes; i++) {
        remaining_burst[i] = burst_time[i];
    }

    // Start bursting
    while(notBursted){
        notBursted = false;
        for(int j = 0; j < no_of_processes; j++){

            // Get the response time
            if (!(timed[j])){
                response_time[j] = time;
                timed[j] = 1;
            }

            if (remaining_burst[j] <= 0) {
                continue;
            } 
            else if (remaining_burst[j] > time_quantum) {

                remaining_burst[j] -= time_quantum;
                // Make other processes wait and add the time :p
                for(int k = 0; k < no_of_processes; k++){
                    if (k != j && remaining_burst[k] > 0) {
                        waiting_time[k] += time_quantum;
                        turnaround_time[k] += time_quantum;
                    }
                }

                notBursted = true;
            }

            // Process comes here when it has less remaining time than time slice
            else {
                for(int k = 0; k < no_of_processes; k++){
                    if (k != j && remaining_burst[k] > 0) {
                        waiting_time[k] += remaining_burst[j];
                        turnaround_time[k] += remaining_burst[j];
                    }
                }
                remaining_burst[j] = 0;
                notBursted = true;
            }
            time += time_quantum;
        }
        
    }

    // Calculate TurnAround time (waiting time + burst time)
    turnMeAround(&no_of_processes, burst_time, turnaround_time, waiting_time);
    
    // Calculate the all the averages
    calcAverages(&no_of_processes, waiting_time, turnaround_time, response_time, &average_waiting_time, &average_turn_around_time, &average_response_time);

    // Print the results
    printVictoryBanner(&no_of_processes, burst_time, waiting_time, turnaround_time, response_time, &average_response_time, &average_turn_around_time, &average_waiting_time);

    return 0;
}
