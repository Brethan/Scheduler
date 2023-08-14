#include "assignment.h"
#define COLUMN_HEADERS "Time of Transition,PID,Old State,New State\n"

extern char *output_path;
extern char *input_path;

char *copyString(char *other_str) {
    char *str = (char *)malloc(strlen(other_str) + 1);
    strcpy(str, other_str);

    return str;
}

void discardColumnHeaders(FILE *file) {
    int c;
    while (1) {
        c = fgetc(file);
        // Read until the first newline character
        if (feof(file) || c == '\n') {
            break;
        }
    }
}

/* Create the output CSV file with the necessary
column headers */
void initializeOutputCSV() {
    FILE *file = fopen(output_path, "w");
    assert(file != NULL);

    fprintf(file, COLUMN_HEADERS);

    fclose(file);

    return;
}

/* Appends the last transfer between state queues to the output CSV */
void appendToOutput(PCBNode *node, char *iState, char *fState, int timer) {
    FILE *file = fopen(output_path, "a");
    assert(file != NULL);

    fprintf(file, "%d,%d,%s,%s\n",
            timer, node->pid, iState, fState);

    fclose(file);
    return;
}

/* Reads the test case CSV and returns a pointer
to the file stream */
FILE *readTestCase() {
    FILE *file = fopen(input_path, "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    discardColumnHeaders(file);
    return file;
}
