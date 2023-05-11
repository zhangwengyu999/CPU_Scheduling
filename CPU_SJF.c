// Made by Mike_Zhang (https://mike.ultrafish.io)
// 2023-05-11
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

int* waitingTime;
int* turnaroundTime;
int* completeTime;
int* serviceTime;
int* ganttSequence;
int totalCompleteTime;
int totalWaitingTime;
int totalTurnaroundTime;

// Get the process with the lowest priority in the waiting queue
// in case of tie, the one with the lowest process number will be returned
int getPrio(int* inPr, int* inWait, int num){
    int i;
    int maxPr=-1;
    for (i=0;i<num;i++) {
        if (inPr[i]>maxPr) {
            maxPr=inPr[i];
        }
    }
    int min=maxPr+1;
    int prio=-1;
    for (i=0;i<num;i++) {
        if (inWait[i]==1 && inPr[i]<min)  {
            min=inPr[i];
            prio=i;
        }
    }
    return prio+1;
}

void PR(int* inArrivalTime, int* inBurstTime, int* inPriority, int inN, int isPreemptive) {
    int num = inN;
    int isWaiting[num];
    int burstTime[num];
    int k;
    for (k=0; k<num; k++) {
        burstTime[k]=inBurstTime[k];
        isWaiting[k]=0;
        totalCompleteTime+=inBurstTime[k];
    }
    totalCompleteTime = totalCompleteTime>(inArrivalTime[num-1]+inBurstTime[num-1])?totalCompleteTime:(inArrivalTime[num-1]+inBurstTime[num-1]);
    int currentTime=0; // the clock
    waitingTime = (int*) malloc(sizeof(int)*num);
    waitingTime[0]=0;
    serviceTime=(int*) malloc(sizeof(int)*num);
    serviceTime[0]=0;
    turnaroundTime = (int*) malloc(sizeof(int)*num);
    completeTime = (int*) malloc(sizeof(int)*num);
    ganttSequence = (int*) malloc(sizeof(int)*totalCompleteTime);

    int processInRun=-1;
    int processInArrive;
    while (currentTime<=totalCompleteTime) {
        // complete
        if (processInRun!=-1 && burstTime[processInRun-1]==0) {
            completeTime[processInRun-1]=currentTime;
            turnaroundTime[processInRun-1]=completeTime[processInRun-1]-inArrivalTime[processInRun-1];
            isWaiting[processInRun-1]=0;
            processInRun=-1;
        }
        // arrive
        int i;
        for (i=0;i<num;i++) {
            if (inArrivalTime[i]==currentTime) { // new process arrive
                isWaiting[i]=1;
                if (isPreemptive == 1 && processInRun!=-1 && inPriority[i]<inPriority[processInRun-1]) { // take over
                    isWaiting[processInRun-1]=1;
                    isWaiting[i]=0;
                    processInRun=i+1; // preemptive take over
                }
                break;
            }
        }
        // new run
        if (processInRun==-1) {
            processInRun=getPrio(inPriority, isWaiting, num); // get the boss
            if (processInRun!=-1) {
                isWaiting[processInRun-1]=0;
            }
        }
        // get service time
        if (processInRun!=-1 && inBurstTime[processInRun-1] == burstTime[processInRun-1]) {
            serviceTime[processInRun-1]=currentTime;
        }
        // decrease burst time
        if (processInRun!=-1) {
            burstTime[processInRun-1]--;
        }
        ganttSequence[currentTime]=processInRun;
        currentTime++;
    }
    int i;
    // get waiting time
    for (i=0;i<inN;i++) {
        waitingTime[i]=turnaroundTime[i]-inBurstTime[i];
    }
    for (i=0;i<inN;i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnaroundTime += turnaroundTime[i];
    }
}

void SJF(int* inArrivalTime, int* inBurstTime,int inN){
    int* priority = (int*) malloc(sizeof(int)*inN);
    int* temp = (int*) malloc(sizeof(int)*inN);
    int i;
    for (i=0;i<inN;i++) {
        temp[i]=inBurstTime[i];
    }
    // sort the temp
    int j;
    for (i=0;i<inN;i++) {
        for (j=i+1;j<inN;j++) {
            if (temp[i]>temp[j]) {
                int t = temp[i];
                temp[i]=temp[j];
                temp[j]=t;
            }
        }
    }
    // assign priority
    for (i=0;i<inN;i++) {
        for (j=0;j<inN;j++) {
            if (inBurstTime[i]==temp[j]) {
                priority[i]=j+1;
                break;
            }
        }
    }
    PR(inArrivalTime, inBurstTime, priority, inN, 0); // non-preemptive PR
}

int main() {
    // Set the input Arrival Time (AT) and Burst Time (BT) below
    int arrivalTime[] = {0,1,2,3,4}; // AT, assume arrive in ascending order
    int burstTime[] = {9,5,2,3,6}; // BT

    int n = sizeof(arrivalTime) / sizeof(arrivalTime[0]);
    SJF(arrivalTime, burstTime, n);
    printf("CPU Shortest Job First Scheduling\n");
    printf("> Scheduling Considerations:\n");
    printf("PID\tArrival\tBurst\tWaiting\tService\tTurn.\tComplete\n");
    int j;
    for (j=0;j<n;j++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", j+1,arrivalTime[j],burstTime[j],waitingTime[j],serviceTime[j],turnaroundTime[j],completeTime[j]);
    }
    printf("> Completion Time:\t\t %d\n", totalCompleteTime);
    printf("> Total Waiting Time:\t\t %d\n", totalWaitingTime);
    printf("> Average Queuing Time:\t\t %.2f \n",(float)totalWaitingTime/completeTime[n-1]);
    printf("> Average Waiting Time:\t\t %.2f \n",(float)totalWaitingTime/n);
    printf("> Average Turnaround Time:\t %.2f \n", (float)totalTurnaroundTime/n);
    printf("> Gantt Chart: \n");
    int i;
    for (i=0;i<=totalCompleteTime;i++) {
        if (i<10) {
            printf("t%d  ", i);
        }
        else {
            printf("t%d ", i);
        }
    }
    printf("\n");
    for (i=0;i<totalCompleteTime;i++) {
        if (ganttSequence[i]==0) {
            printf("--  ");
        }
        else {
            printf("P%d  ", ganttSequence[i]);
        }
    }
    printf("End\n");
}