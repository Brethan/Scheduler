#ifndef VISUAL_H
#define VISUAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NEW 10
#define READY 8
#define WAITING 6
#define RUNNING 5
#define TERMINATED 4
#define TIME 1

typedef struct Node {
    struct Node *next;
    int time;
    int pid;
    char iState[20];
    char fState[20];
} Node;

typedef struct Q {
    Node *pcb;
} Queue;

void resetCursor();
void deleteLine();
void saveCursor();

void moveCursorUp(int numLines);
void moveCursorDown(int numLines);
void moveCursorRight(int numCol);
void moveCursorLeft(int numCol);
void moveCursorToState(int numLines);

void deleteBottomLine();

void printSingle(char *state, char *pid, int lines);
void printNew(char *pid);
void printReady(char *pid);
void printWaiting(char *pid);
void printRunning(char *pid);
void printTerminated(char *pid);
void printTime(char *pid);

void initVisualizer();
void input(char *prompt, int *a);
void delay(int ms);

char *copyString(Node *node);

#endif
