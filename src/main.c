#include "assignment.h"

#define TIME_SLICE 70

char *output_path;
char *input_path;

int main(int argc, char *argv[]) {
    if (argc < 3) return 1;

    // Read input csv and output csv path from command line
    input_path = copyString(argv[1]);
    output_path = copyString(argv[2]);

    PCBQueue *batch = buildArrivingQueue(input_path);
    assert(batch != NULL);

    initializeOutputCSV();

    int maxNumProcesses = batch->size;
    // "Queue" of running PCBs (easier to use the same data type)
    PCBQueue *running = initializePCBQueue(RUNNING);
    // Queue of PCBs that have terminated
    PCBQueue *terminated = initializePCBQueue(TERMINATED);

    // Queue of PCBs ready to be moved to the CPU
    PCBQueue *ready = initializePCBQueue(READY);
    // Queue of PCBs undergoing IO or event waiting
    PCBQueue *waiting = initializePCBQueue(WAITING);

    // Variables to calculate metrics
    int idleTime = 0;
    int numRdyToRun = 0;
    int responseTime = 0;
    int turnAround = 0;
    int waitTime = 0;
    
    int timer = 0;

    /* Loop steps:

        1. Move arriving processes into the ready queue
        2. If applicable transfer running process to another queue
            2.1: process runTime >= cpuTime -> Terminated queue
            2.2: process last IO time >= ioFreq -> waiting queue
            2.3: process time in CPU >= TIME_SLICE -> ready queue 
        3. If applicable, move all eligible nodes from the waiting queue
           to the ready queue
        4. If nothing is running, move head of ready queue -> running queue 
        
    */
    // Keep going until all processes terminate
    while (terminated->size < maxNumProcesses) {

        // Check for incoming processes; Enqueue into ready queue
        // Sends nodes to the ready queue independent of its queue position
        if (transferBulkNodes(ready, batch, timer) && headNotNull(running)) {
            transferHeadNode(ready, running, timer);
        }

        // Check to see if the running node needs to be transitioned
        if (headNotNull(running)) {
            PCBNode *runHead = running->head;
            runHead->runTime++;
            runHead->lastIO++;
            if ((runHead->runTime) >= runHead->cpuTime) {
                // BONUS: Accumulate data for each process
                turnAround += (timer - runHead->arrivalTime);
                transferHeadNode(terminated, running, timer); // 2: RUNNING -> TERMINATED (exit)

            } else if ((runHead->lastIO >= runHead->ioFreq) && (runHead->ioFreq > 0)) {
                runHead->lastIO = 0;
                transferHeadNode(waiting, running, timer); // 3: RUNNING -> WAITING (I/O or Event)
                
            // If ready queue is empty, run for as long as the IO freq or CPU time will allow (round robin?)
            } else if ((timer - runHead->transitionTime) >= TIME_SLICE && ready->size > 0)
                transferHeadNode(ready, running, timer); // 4: RUNNING -> READY (Interrupt / Preemption)

        } else // BONUS: Idle time metric
            idleTime++;

        // Send completed waiting PCBs back to the ready queue
        // Sends any valid node back, not necessarily FCFS
        transferBulkNodes(ready, waiting, timer);

        // Move head of ready queue to the running node
        if (!headNotNull(running) && headNotNull(ready)) {
            // BONUS: Accumulate the amount of time spent in ready queue (waiting time)
            waitTime += (timer - ready->head->transitionTime);
            numRdyToRun++;

            if (!ready->head->responded) { // BONUS: Response time
                ready->head->responded = 1;
                responseTime += (timer - ready->head->arrivalTime);
            }
            // 6: READY -> RUNNING (scheduler dispatch)
            transferHeadNode(running, ready, timer);
        }

        timer++;
    }

    // Timer increments once more after all PCBs are terminated
    timer--;

    // Clean up all of the queues
    terminated = destroyQueue(terminated);
    waiting = destroyQueue(waiting);
    running = destroyQueue(running);
    ready = destroyQueue(ready);
    batch = destroyQueue(batch);

    /* Only the terminated list had any nodes, but all the queues
    have memory allocated for their status as well as the memory
    for the PCBQueue struct itself */

    free(input_path);
    free(output_path);

    // %cpuTime - %idleTime = %cpuUtil
    double cpuUtilization = 100 - ((idleTime / (double)timer) * 100.0);
    double avgResponseTime = (responseTime / (double)maxNumProcesses);
    double avgTurnaround = (turnAround / (double)maxNumProcesses);
    double throughput = (maxNumProcesses / (timer / 1000.0));
    double avgWaitTime = (waitTime / (double)numRdyToRun);

    // Print out all of the metrics
    printf("CPU Utilization: %.2lf%%\nAverage Turnaround: %.2lfms\n", cpuUtilization, avgTurnaround);
    printf("Average Wait Time: %.2lfms\nAverage Response Time: %.2lfms\n", avgWaitTime, avgResponseTime);
    printf("Throughput: %.3lf processes per 100ms\n", throughput / 10);

    return 0;
}
