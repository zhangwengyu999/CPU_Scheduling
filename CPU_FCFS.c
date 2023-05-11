// Made by Mike_Zhang (https://mike.ultrafish.io)
// 2023-05-11
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

int totalWaitingTime;
int totalTurnaroundTime;
int* waitingTime;
int* turnaroundTime;
int* completeTime;
int* serviceTime;
int* ganttSequence;
int** waitingQueueSequence;

void FCFS(int* inArrivalTime, int* inBurstTime, int inN) {
    waitingTime = (int*) malloc(sizeof(int)*inN);
    waitingTime[0]=0;

    serviceTime=(int*) malloc(sizeof(int)*inN);
    serviceTime[0]=0;

    turnaroundTime = (int*) malloc(sizeof(int)*inN);
    completeTime = (int*) malloc(sizeof(int)*inN);

    int i;
    for (i=1;i<inN;i++) {
        serviceTime[i] = serviceTime[i-1] + inBurstTime[i-1];
        if (serviceTime[i]<inArrivalTime[i]) {serviceTime[i]=inArrivalTime[i];}
        waitingTime[i] = serviceTime[i] - inArrivalTime[i];
        if (waitingTime[i]<0) {waitingTime[i]=0;}
    }
    for (i=0;i<inN;i++) {
        turnaroundTime[i] = inBurstTime[i] + waitingTime[i];
    }
    for (i=0;i<inN;i++) {
        completeTime[i] = turnaroundTime[i] + inArrivalTime[i];
    }
    for (i=0;i<inN;i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnaroundTime += turnaroundTime[i];
    }
}

int main() {
    // Set the input Arrival Time (AT) and Burst Time (BT) below
    int arrivalTime[] = {0,1,2,3,4}; // AT, assume arrive in ascending order
    int burstTime[] = {9,5,2,3,6}; // BT

    int n = sizeof(arrivalTime) / sizeof(arrivalTime[0]);
    FCFS(arrivalTime, burstTime, n);
    printf("CPU First Come First Serve Scheduling\n");
    printf("> Scheduling Considerations:\n");
    printf("Process\tArrival\tBurst\tWaiting\tService\tT.A.\tComplete\n");
    int j;
    for (j=0;j<n;j++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", j+1,arrivalTime[j],burstTime[j],waitingTime[j],serviceTime[j],turnaroundTime[j],completeTime[j]);
    }
    printf("> Completion Time:\t\t %d\n", completeTime[n-1]);
    printf("> Total Waiting Time:\t\t %d\n", totalWaitingTime);
    printf("> Average Queuing Time:\t\t %.2f \n",(float)totalWaitingTime/completeTime[n-1]);
    printf("> Average Waiting Time:\t\t %.2f \n",(float)totalWaitingTime/n);
    printf("> Average Turnaround Time:\t %.2f \n", (float)totalTurnaroundTime/n);
    ganttSequence = (int*) malloc(sizeof(int)*completeTime[n-1]);
    int seqIndex;
    int i;
    for (i=0;i<n;i++) {
        int start = serviceTime[i];
        int end = completeTime[i];
        int j;
        seqIndex = start;
        for (j=start;j<end;j++) {
            ganttSequence[seqIndex] = i+1;
            seqIndex++;  
        }
    }
    printf("> Gantt Chart: \n");
    for (i=0;i<=completeTime[n-1];i++) {
        if (i<10) {
            printf("t%d  ", i);
        }
        else {
            printf("t%d ", i);
        }
    }
    printf("\n");
    for (i=0;i<completeTime[n-1];i++) {
        if (ganttSequence[i]==0) {
            printf("--  ");
        }
        else {
            printf("P%d  ", ganttSequence[i]);
        }
            
    }
    printf("End\n");
    waitingQueueSequence=(int**) malloc(sizeof(int*)*completeTime[n-1]);
    for (i=0;i<=completeTime[n-1];i++) {
        int at;
        int st;
        waitingQueueSequence[i]=(int*) malloc(sizeof(int)*n);
        int* waitQueue = waitingQueueSequence[i];
        int p;
        for (p=0;p<n;p++) {
            at = arrivalTime[p];
            st = serviceTime[p];
            if (at==st) {
                waitQueue[p]=0;
                continue;
            }
            else if (i>=at && i<st) {
                waitQueue[p]=p+1;
            }
            else {
                waitQueue[p]=0;
            }
        }
    }
    printf("> Waiting Queue: \n");
    int k;
    for (k=0;k<completeTime[n-1];k++) {
        printf("t%d:\t", k);
        int q;
        for (q=0;q<n;q++) {
            if (waitingQueueSequence[k][q]<=n && waitingQueueSequence[k][q]>0) {
                printf("P%d ", waitingQueueSequence[k][q]);
            }
        }
        printf("\n");
    }
}
