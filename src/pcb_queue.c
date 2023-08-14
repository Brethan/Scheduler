#include <time.h>

#include "assignment.h"

/* Frees all of the memory allocated to the queue:
PCBNode Linked list, Status string, and the PCBQueue itself */
void *destroyQueue(PCBQueue *queue) {
    if (queue == NULL) return NULL;

    if (queue->size > 0) {
        // Free PCB linked list
        PCBNode *c = queue->head;
        while (c != NULL) {
            PCBNode *t = c;
            c = c->next;
            free(t);
        }
    }

    // Free queue
    free(queue->status);
    free(queue);

    return NULL;
}

/* Print out the contents of the queue in a similar way
as the test_case_n.csv file. Purely for debugging */
void printQueue(PCBQueue *queue) {
    assert(queue != NULL);
    PCBNode *node;
    node = queue->head;

    while (node != NULL) {
        printf("PID: %d; Arrival Time: %d; CPU Time: %d; IO Freq: %d; IO Time: %d; Status: %s\n",
               node->pid, node->arrivalTime, node->cpuTime, node->ioFreq, node->ioTime,
               queue->status);
        node = node->next;
    }
}

/* Appends a PCBNode to the end of a PCBQueue */
void enqueue(PCBQueue *queue, PCBNode *node) {
    assert(node != NULL);

    if (queue->head == NULL) {
        queue->head = node;

    } else {  // Traverse to the end of the linked list
        PCBNode *c = queue->head;
        while (c->next != NULL) {
            c = c->next;
        }

        // Attach PCB to the list
        c->next = node;
    }

    queue->size++;

    return;
}

/* Removes the current head of the PCB list and
if it exists, sets the next node of the queue as the
new head */
PCBNode *dequeue(PCBQueue *queue) {
    assert(queue != NULL);
    assert(headNotNull(queue));

    PCBNode *node = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    node->next = NULL;
    return node;
}

int headNotNull(PCBQueue *queue) {
    return queue->head != NULL ? 1 : 0;
}

/* Move the head node from one queue to another. 
Additionally, will trigger the output to the CSV */
void transferHeadNode(PCBQueue *dest, PCBQueue *source, int timer) {
    assert(!(dest == NULL && source == NULL));
    assert(headNotNull(source));

    PCBNode *srcNode = dequeue(source);
    // Write to the output CSV
    appendToOutput(srcNode, source->status, dest->status, timer);

    srcNode->transitionTime = timer;
    enqueue(dest, srcNode);
    return;
}

/* Creates a queue with all the necessary default 
values set and memory allocated */
PCBQueue *initializePCBQueue(char *status) {
    PCBQueue *q = (PCBQueue *)malloc(sizeof(PCBQueue));
    assert(q != NULL);
    q->head = NULL;
    q->size = 0;
    q->status = copyString(status);

    return q;
}

/* Creates a PCB with all the necessary default
values and memory allocated */
PCBNode *constructNode() {
    PCBNode *p = (PCBNode *)malloc(sizeof(PCBNode));
    assert(p != NULL);

    p->transitionTime = 0;
    p->runTime = 0;
    p->lastIO = 0;
    return p;
}

int waitNodeCondition(PCBNode *src, int timer) {
    return (src != NULL) && ((timer - src->transitionTime) >= src->ioTime);
}

int newNodeCondition(PCBNode *src, int timer) {
    return (src != NULL) && (timer == src->arrivalTime);
}

int decodeCondition(PCBNode *src, int timer, int key) {
    if (key)
        return waitNodeCondition(src, timer);
    else 
        return newNodeCondition(src, timer);
}

int transferBulkNodes(PCBQueue *dest, PCBQueue *src, int timer) {
    PCBQueue q = {NULL, src->status, 0};
    PCBNode *temp = src->head;
    int key = strcmp(src->status, WAITING) == 0 ? 1 : 0;
    
    while (decodeCondition(src->head, timer, key)) {
        temp = dequeue(src);
        temp->next = NULL;
        enqueue(&q, temp);
    } 


    PCBNode *node = src->head;
    while (node != NULL && node->next != NULL) {
        if (decodeCondition(node->next, timer, key)) {
            temp = node->next;
            node->next = temp->next;
            temp->next = NULL;
            enqueue(&q, temp);
        }

        node = node->next;
    }

    int numTransfers = q.size;
    while (headNotNull(&q)) {
        transferHeadNode(dest, &q, timer);
    }

    return numTransfers ? 1 : 0;
}

/* Reads the test_case_n.csv file and creates a "batch"
queue which will be used to start the simulation */
PCBQueue *buildArrivingQueue(char *file_path) {
    FILE *file = readTestCase(file_path);
    assert(file != NULL);

    // Generate the initial queue
    PCBQueue *queue = initializePCBQueue(NEW);
    PCBNode *node = constructNode();
    queue->head = node;

    int read = 0;

    do {
        read = fscanf(
            file,
            "%d,%d,%d,%d,%d",
            &node->pid,
            &node->arrivalTime,
            &node->cpuTime,
            &node->ioFreq,
            &node->ioTime);

        queue->size++;
        if (read == 5 && !feof(file)) {
            node->next = constructNode();
            node = node->next;
        } else if (read != 5 && !feof(file)) {
            printf("File format incorrect.\n");
            return NULL;
        }

        if (ferror(file)) {
            printf("Error reading file.\n");
            return NULL;
        }

    } while (!feof(file));

    node->next = NULL;

    fclose(file);

    return queue;
}
