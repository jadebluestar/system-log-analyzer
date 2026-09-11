#ifndef ANALYZER_H
#define ANALYZER_H

#include "parser.h"

#define MAX_UNIQUE_ERRORS 50

/* Holds a distinct error message and how many times it appeared */
struct ErrorCount {
    char message[MESSAGE_LEN];
    int count;
};

/* Prints the INFO/WARNING/ERROR totals and the top error messages */
void print_summary(struct LogEntry entries[], int entry_count);

/* Prints how many entries contain the given keyword (case-sensitive) */
void search_keyword(struct LogEntry entries[], int entry_count, const char *keyword);

/* Writes the same summary shown on screen into a report file */
void write_report(struct LogEntry entries[], int entry_count, const char *filename);

#endif
