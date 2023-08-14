#ifndef ASSIGNMENT_H
    #define ASSIGNMENT_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <assert.h>

    // Define process states
    #define NEW "NEW"
    #define READY "READY"
    #define RUNNING "RUNNING"
    #define WAITING "WAITING"
    #define TERMINATED "TERMINATED"

    // Structure modelling a PCB
    typedef struct Node {
        struct Node *next;
        int transitionTime;
        int responseTime;
        int arrivalTime;
        int responded;
        int cpuTime;
        int runTime;
        int lastIO;
        int ioFreq;
        int ioTime;
        int pid;
    } PCBNode;

    typedef struct Queue {
        PCBNode *head;  // should have made this "node" :(
        char *status;
        int size;
    } PCBQueue;

    // pcb_queue.c
    PCBQueue *buildArrivingQueue(char *file_path);
    PCBQueue *initializePCBQueue(char *status);
    PCBNode *dequeue(PCBQueue *queue);
    void transferHeadNode(PCBQueue *dest, PCBQueue *source, int timer);
    int transferBulkNodes(PCBQueue *dest, PCBQueue *src, int timer);
    void *destroyQueue(PCBQueue *queue);
    void printPCBNode(PCBNode *node);
    void printQueue(PCBQueue *queue);
    int headNotNull(PCBQueue *queue);

    // input_process.c
    FILE *readTestCase();
    char *copyString(char *other_str);
    void appendToOutput(PCBNode *node, char *iState, char *fState, int timer);
    void discardColumnHeaders(FILE *);
    void initializeOutputCSV();
    


#endif