#include "visual.h"

Queue *ready, *term, *waitQ, *running;

Node *constructNode() {
    Node *n = (Node *)malloc(sizeof(Node));
    n->next = NULL;

    return n;
}

Node *copyNode(Node *pcb) {
    Node *n = constructNode();
    n->pid = pcb->pid;
    n->time = pcb->time;
    n->next = NULL;

    return n;
}

Queue *constructQueue() {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->pcb = constructNode();

    return q;
}

Queue *constructQueue1() {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->pcb = NULL;

    return q;
}

Queue *processSimulationOutput(char *output_path) {
    FILE *file = fopen(output_path, "r");

    if (file == NULL) return NULL;
    if (feof(file)) return NULL;
    Queue *q = constructQueue();
    Node *pcb = q->pcb;

    int read = 1;
    fscanf(file, "%*[^\n]\n");
    do {
        fscanf(file, "%d,%d,%19[^,],%19[^\n]\n",
               &pcb->time, &pcb->pid, pcb->iState, pcb->fState);

        if (!feof(file)) {
            pcb->next = constructNode();
            pcb = pcb->next;
        }

    } while (!feof(file) && (read++));

    pcb->next = NULL;
    fclose(file);
    return q;
}

void printFullQueue(Queue *q) {
    for (Node *n = q->pcb; n != NULL; n = n->next) {
        printf("%d,%d,%s,%s\n",
               n->time, n->pid, n->iState, n->fState);
    }
}

void printQueue(Queue *q) {
}

Queue *determineQueue(char *state) {
    if (strcmp(state, "READY") == 0) {
        return ready;
    } else if (strcmp(state, "RUNNING") == 0) {
        return running;
    } else if (strcmp(state, "WAITING") == 0) {
        return waitQ;
    } else
        return term;
}

Node *dequeue(Queue *src) {
    Node *n = src->pcb;
    src->pcb = src->pcb->next;
    n->next = NULL;
    return n;
}

void enqueue(Queue *dest, Node *n) {
    if (dest->pcb == NULL) {
        dest->pcb = n;
        return;
    } else {
        Node *d = dest->pcb;
        while (d->next != NULL) {
            d = d->next;
        }

        d->next = n;
    }
}

Node *removeNode(Queue *src, int pid) {
    if (src->pcb->pid == pid) {
        return dequeue(src);
    }
    Node *n = src->pcb;
    while (n->next != NULL) {
        if (n->next->pid == pid) {
            Node *t;
            t = n->next;
            n->next = t->next;
            t->next = NULL;
            return t;
        }

        n = n->next;
    }

    return NULL;
}

void transferNode(int pid, Queue *dest, Queue *src) {
    Node *n = removeNode(src, pid);
    enqueue(dest, n);
}

void initQueues() {
    running = constructQueue1();
    ready = constructQueue1();
    waitQ = constructQueue1();
    term = constructQueue1();
}

void fillBuffer(char *buff, Queue *src) {
    int counter = 0;
    if (src->pcb == NULL) {
        strcpy(buff, " ");
    } else {
        Node *n = src->pcb;
        while (n != NULL) {
            char b[10];
            sprintf(b, "%d ", n->pid);
            strcat(buff, b);
            n = n->next;
            if (src == ready && counter == 10) {
                strcat(buff, "...");
                break;
            }

            counter++;
        }
    }
}

void repaint(int time, char *new) {
    printNew(new);

    char buff[1024];
    sprintf(buff, "%d", time);
    printTime(buff);
    strcpy(buff, " ");

    fillBuffer(buff, ready);
    printReady(buff);
    strcpy(buff, " ");

    fillBuffer(buff, waitQ);
    printWaiting(buff);
    strcpy(buff, " ");

    fillBuffer(buff, running);
    printRunning(buff);
    strcpy(buff, " ");

    fillBuffer(buff, term);
    printTerminated(buff);
    strcpy(buff, " ");
}

void cleanTerminatedQueue(int last, int time, char *new) {
    while (term->pcb->next != NULL) {
        Node *temp = term->pcb;
        term->pcb = term->pcb->next;
        free(temp);

        repaint(time, new);
        delay(40000);
    }

    if (last) {
        free(term->pcb);
        term->pcb = NULL;
    }
}

int main(int argc, char *argv[]) {
    char *output_path = argv[3];
    Queue *q = processSimulationOutput(output_path);
    int a = 0;
    printf("Fast (0), Slow(1): ");
    scanf("%d", &a);

    int slow = atoi(argv[1]);
    int fast = atoi(argv[2]);

    int waitTime = a > 0 ? slow : fast;

    initVisualizer();
    initQueues();

    Node *pcb = q->pcb;
    int time = 0, counter = 0;
    char *new = (char *)malloc(5);
    strcpy(new, " ");
    while (pcb != NULL) {
        if (time != pcb->time) {
            new = realloc(new, strlen(" ") + 1);
            strcpy(new, " ");
            time = pcb->time;
        }

        if (strcmp(pcb->iState, "NEW") == 0) {
            char buff[20];
            sprintf(buff, "%d ", pcb->pid);
            new = realloc(new, strlen(new) + strlen(buff) + 2);
            strcat(new, buff);
            enqueue(ready, copyNode(pcb));
        } else {
            Queue *src, *dest;
            src = determineQueue(pcb->iState);
            dest = determineQueue(pcb->fState);
            transferNode(pcb->pid, dest, src);
            if (dest == term)
                counter++;
        }

        repaint(time, new);

        pcb = pcb->next;
        delay(waitTime);

        if (counter == 10) {
            counter = 0;
            cleanTerminatedQueue(0, time, new);
        }
    }
	delay(waitTime * 2);
    cleanTerminatedQueue(1, time, new);
    repaint(time, new);

    resetCursor();
    return 0;
}
