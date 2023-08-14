#include "visual.h"


void resetCursor() {
	printf("\x1b[u");	
}

void deleteLine() {
	printf("\x1b[2K");
}

void saveCursor() {
	printf("\x1b[s");
}

void moveCursorUp(int numLines) {
	printf("\x1b[%dA", numLines);
}

void moveCursorDown(int numLines) {
	printf("\x1b[%dB", numLines);
}

void moveCursorRight(int numCol) {
	printf("\x1b[%dC", numCol);
}

void moveCursorLeft(int numCol) {
	printf("\x1b[%dD", numCol);
}

void deleteBottomLine() {
	resetCursor();
	moveCursorUp(1);
	printf("\x1b[2K");
	saveCursor();
}

void input(char *prompt, int *a) {
	printf("%s: ", prompt);
	scanf("%d", a);
	moveCursorUp(1);
	deleteLine();
	saveCursor();
}

void initVisualizer() {
	printf("NEW: \n\nREADY: \n\nWAITING: \nRUNNING: \nTERMINATED: \n\n\nTIME: \n");
	saveCursor();
}

void moveCursorToState(int lines) {
	resetCursor();
	moveCursorUp(lines);
	deleteLine();
}


void printSingle(char *state, char *pid, int lines) {
	moveCursorToState(lines);
	printf("%s: %s\n", state, pid);
	resetCursor();
}	

void printNew(char *pid) {
	printSingle("NEW", pid, NEW);
}

void printReady(char *pid) {
    printSingle("READY", pid, READY);
}

void printRunning(char *pid) {
    printSingle("RUNNING", pid, RUNNING);
}

void printWaiting(char *pid) {
	printSingle("WAITING", pid, WAITING);
}

void printTerminated(char *pid) {
	printSingle("TERMINATED", pid, TERMINATED);
}

void printTime(char *pid) {
	printSingle("TIME", pid, TIME);
}

char *copyString(Node *node) {
    char buff[20];
    sprintf(buff, "%d", node->pid);
    char *str = (char *)malloc(strlen(buff) + 1);
    strcpy(str, buff);
    return str;
}

void delay(int ms) {
    // Storing start time
    clock_t start_time = clock();

    while (clock() < start_time + ms)
        ;
}
