> ***Please carefully read the [Disclaimer of Liability](#disclaimer-of-liability) below first.***  
> All materials in this archive are for reference only. The actual one may vary.

# CPU_Scheduling_Algorithms
Implementation of CPU Scheduling Algorithms in C Programming Language

Containing following five algorithms:

- [FCFS (First Come First Serve)](CPU_FCFS.c)
- [PR (Priority), Linux version, preemptive and non-preemptive](CPU_PR.c)
- [SJF (Shortest Job First)](CPU_SJF.c)
- [SRT (Shortest Remaining Time)](CPU_SRT.c)
- [Round Robin (RR)](CPU_SRT.c)
  - **Tie Breaking Rule**: the new arrival one will be served first.

---

## Before Running
Please configure the input processes data in the `main()` function in each .c file before running.


For example in `CPU_FCFS.c`:

```c
int main() {
    // Set the input Arrival Time (AT) and Burst Time (BT) below
    int arrivalTime[] = {0,1,2,3,4}; // AT, assume arrive in ascending order
    int burstTime[] = {9,5,2,3,6}; // BT
    // ...
}
```

---

## Compile and Run

1. Open the terminal;
2. Change directory to this folder;
3. Compile the desired .c file with `gcc` command, for example:

```bash
gcc CPU_FCFS.c -o CPU_FCFS
```
4. Run the executable file, for example:

```bash
./CPU_FCFS
```
5. The output will be shown in the terminal, for example:

```bash
./CPU_FCFS
CPU First Come First Serve Scheduling
> Scheduling Considerations:
Process Arrival Burst   Waiting Service T.A.    Complete
1       0       9       0       0       9       9
2       1       5       8       9       13      14
3       2       2       12      14      14      16
4       3       3       13      16      16      19
5       4       6       15      19      21      25
> Completion Time:               25
> Total Waiting Time:            48
> Average Queuing Time:          1.92 
> Average Waiting Time:          9.60 
> Average Turnaround Time:       14.60 
> Gantt Chart: 
t0  t1  t2  t3  t4  t5  t6  t7  t8  t9  t10 t11 t12 t13 t14 t15 t16 t17 t18 t19 t20 t21 t22 t23 t24 t25 
P1  P1  P1  P1  P1  P1  P1  P1  P1  P2  P2  P2  P2  P2  P3  P3  P4  P4  P4  P5  P5  P5  P5  P5  P5  End
> Waiting Queue: 
t0:
t1:     P2 
t2:     P2 P3 
t3:     P2 P3 P4 
t4:     P2 P3 P4 P5 
t5:     P2 P3 P4 P5 
t6:     P2 P3 P4 P5 
t7:     P2 P3 P4 P5 
t8:     P2 P3 P4 P5 
t9:     P3 P4 P5 
t10:    P3 P4 P5 
t11:    P3 P4 P5 
t12:    P3 P4 P5 
t13:    P3 P4 P5 
t14:    P4 P5 
t15:    P4 P5 
t16:    P5 
t17:    P5 
t18:    P5 
t19:
t20:
t21:
t22:
t23:
t24:
```

---

## Disclaimer of Liability

**The material and information contained on this website is for general information, reference, and self-learning purposes only. You should not rely upon the material or information on the website as a basis for making any academic, business, legal or any other decisions. You should not copy any material or information on the website into any of your academic, business, legal or any other non-private usages. ZHANG Wengyu will not be responsible for any consequences due to your violations.**


Whilst ZHANG Wengyu endeavours to keep the information up to date and correct, ZHANG Wengyu makes no representations or warranties of any kind, express or implied about the completeness, accuracy, reliability, suitability or availability with respect to the website or the information, products, services or related graphics contained on the website for any purpose. Any reliance you place on such material is therefore strictly at your own risk.


ZHANG Wengyu will not be liable for any false, inaccurate, inappropriate or incomplete information presented on the website.


Although every effort is made to keep the website up and running smoothly, due to the nature of the Internet and the technology involved, ZHANG Wengyu takes no responsibility for and will not be liable for the website being temporarily unavailable due to technical issues (or otherwise) beyond its control or for any loss or damage suffered as a result of the use of or access to, or inability to use or access this website whatsoever.


Certain links in this website will lead to websites which are not under the control of ZHANG Wengyu. When you activate these you will leave ZHANG Wengyu's  website. ZHANG Wengyu has no control over and accepts no liability in respect of materials, products or services available on any website which is not under the control of ZHANG Wengyu.


To the extent not prohibited by law, in no circumstances shall ZHANG Wengyu be liable to you or any other third parties for any loss or damage (including, without limitation, damage for loss of business or loss of profits) arising directly or indirectly from your use of or inability to use, this site or any of the material contained in it.
