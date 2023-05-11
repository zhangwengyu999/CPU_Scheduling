// Made by Mike_Zhang (https://mike.ultrafish.io)
// 2023-05-11
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

typedef struct Queue {
    int size;
    int capacity;
    int front;
    int rear;
    int *data;
} Queue;
Queue* newQueue(int inCapacity) {
    Queue* self=(Queue*) malloc(sizeof(Queue));
    self->size = 0;
    self->capacity = inCapacity;
    self->front = 0;
    self->rear = -1;
    self->data=(int*) malloc(sizeof(int)*inCapacity);
    int i;
    for (i=0;i<self->capacity;i++) {
        self->data[i]=-1;
    }
    return self;
}
int isQueueEmpty(Queue* self) {
    if(self->size == 0)
        return 1;
    return -1;
}
int isQueueFull(Queue* self) {
    if (self->size == self->capacity)
        return 1;
    return -1;
}
int enqueue(Queue* self, int inVal) {
    if(isQueueFull(self) == 1)
        return -1;
    else{
        self->size++;
        self->rear++;
        if(self->rear == self->capacity) {
            self->rear = 0;
        }
        self->data[self->rear] = inVal;
        return 1;
    }
}
int dequeue(Queue* self) {
    if(isQueueEmpty(self) == 1)
        return -1;
    else {
        self->size--;
        int temp = self->data[self->front];
        self->data[self->front]=-1;
        self->front++;

        if(self->front==self->capacity) {
                self->front=0;
        }
        return temp;
    }
}
int front(Queue* self) {
    if(isQueueEmpty(self) == 1)
        return -1;
    else {
        int temp = self->data[self->front];
        return temp;
    }
}
int* getList(Queue* self) {
    int* temp=(int*) malloc(sizeof(int)*self->size);
    int prt=self->front;
    int i;
    for (i=0; i<self->size; i++) {
        temp[i]=self->data[prt];
        prt++;
        if (prt==self->capacity) {
            prt=0;
        }
    }
    return temp;
}
// --------------------------------------------

int* waitingTime;
int* turnaroundTime;
int* completeTime;
int* serviceTime;
int* ganttSequence;
int totalCompleteTime;
int totalWaitingTime;
int totalTurnaroundTime;

void RR(int inNum, int inArrivalArray[], int inServeArray[], int inQuantum) {
    int num=inNum;
    Queue* inArriveQueue=newQueue(num); // queue for processes in arriving
    Queue* waitingQueue=newQueue(num); // queue for processes in waiting
    int burstTime[num]; // burst time
    int k;
    for (k=0; k<num; k++) {
        burstTime[k]=inServeArray[k];
        totalCompleteTime+=inServeArray[k];
    }
    totalCompleteTime = totalCompleteTime>(inArrivalArray[num-1]+inServeArray[num-1])?totalCompleteTime:(inArrivalArray[num-1]+inServeArray[num-1]);
    int currentTime=0; // the clock
    int quantum=inQuantum; // the designed quantum
    int currentQuantum=0; // the current quantum
    waitingTime = (int*) malloc(sizeof(int)*num);
    waitingTime[0]=0;
    serviceTime=(int*) malloc(sizeof(int)*num);
    serviceTime[0]=0;
    turnaroundTime = (int*) malloc(sizeof(int)*num);
    completeTime = (int*) malloc(sizeof(int)*num);
    ganttSequence = (int*) malloc(sizeof(int)*totalCompleteTime);
    int* lastEnqueueTime = (int*) malloc(sizeof(int)*num);

    int i;
    for (i=0; i<num; i++) {
        enqueue(inArriveQueue, i+1);
    }
    int processInRun; // store the current process in service
    int processInArrive=front(inArriveQueue); // first process in come
    dequeue(inArriveQueue);
    enqueue(waitingQueue, processInArrive);
    lastEnqueueTime[processInArrive-1]=inArrivalArray[processInArrive-1];

    while(isQueueEmpty(waitingQueue)==-1 || isQueueEmpty(inArriveQueue)==-1) {
        processInRun=dequeue(waitingQueue);
        processInArrive=front(inArriveQueue);
        if (processInRun==-1) {
            currentTime=inArrivalArray[processInArrive-1];
            processInRun=processInArrive;
            dequeue(inArriveQueue);
        }
        // get the service time
        if (burstTime[processInRun-1] == inServeArray[processInRun-1]) {
            serviceTime[processInRun-1]=currentTime;
        }
        // get the waiting time
        if (currentTime-lastEnqueueTime[processInRun-1]>0) {
            waitingTime[processInRun-1]+=currentTime-lastEnqueueTime[processInRun-1];
        }
        // get time duration in this round
        int duration;
        int isQuantumUp;
        if (burstTime[processInRun-1]<=quantum) {
            duration=burstTime[processInRun-1];
            isQuantumUp=0;
        } else {
            duration=quantum;
            isQuantumUp=1;
        }
        // gantt chart
        int j;
        for (j=0;j<duration;j++) {
            ganttSequence[currentTime+j]=processInRun;
        }
        // check arrival in the duration
        if (processInArrive!=-1) {
            int nextArrTime=inArrivalArray[processInArrive-1];
            while (1) {
                if (nextArrTime>currentTime+duration) {
                    break;
                }
                else if (nextArrTime<=currentTime+duration && nextArrTime>currentTime) {
                    enqueue(waitingQueue, processInArrive);
                    lastEnqueueTime[processInArrive-1]=nextArrTime;
                    dequeue(inArriveQueue);
                }
                processInArrive=front(inArriveQueue);
                if (processInArrive==-1) {
                    break;
                }
                nextArrTime=inArrivalArray[processInArrive-1];
            }
        }
        // finish
        if (isQuantumUp==0) {
            burstTime[processInRun-1]=0;
            completeTime[processInRun-1]=currentTime+duration; // get the complete time
            processInRun=0;
        }
        // quantum time up
        if (isQuantumUp==1) {
            burstTime[processInRun-1]-=duration;
            enqueue(waitingQueue, processInRun);
            lastEnqueueTime[processInRun-1]=currentTime+duration;
            processInRun=0;
        }
        currentTime+=duration;
    }
    for (i=0;i<num;i++) {
        turnaroundTime[i] = completeTime[i] - inArrivalArray[i];
    }
    for (i=0;i<num;i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnaroundTime += turnaroundTime[i];
    }
}

int main() {
    // Set the input Arrival Time (AT), Burst Time (BT), and Quantum (Q) below
    // Tie Breaking Rule: the new arrival one will be served first
    int arrivalTime[] = {0,1,2,3,4}; // AT, assume arrive in ascending order
    int burstTime[] = {9,5,2,3,6}; // BT
    int q=4; // Q

    int n = sizeof(arrivalTime) / sizeof(arrivalTime[0]);
    RR(n, arrivalTime, burstTime, q);
    printf("CPU Round Robin Scheduling (Tie breaking rule: new one first)\n");
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