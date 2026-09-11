#include <stdio.h>
#include <string.h>
#include "analyzer.h"

/* Counts entries per level and returns them via out-parameters */
static void count_levels(struct LogEntry entries[], int entry_count,
                          int *info, int *warning, int *error)
{
    *info = 0;
    *warning = 0;
    *error = 0;

    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            (*info)++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            (*warning)++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            (*error)++;
        }
    }
}

/*
 * Builds a list of unique ERROR messages with their counts.
 * Returns the number of unique error messages found.
 */
static int count_unique_errors(struct LogEntry entries[], int entry_count,
                                struct ErrorCount errors[])
{
    int unique_count = 0;

    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "ERROR") != 0) {
            continue; /* only interested in ERROR lines here */
        }

        /* Have we already seen this exact message? */
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(errors[j].message, entries[i].message) == 0) {
                errors[j].count++;
                found = 1;
                break;
            }
        }

        if (!found && unique_count < MAX_UNIQUE_ERRORS) {
            strncpy(errors[unique_count].message, entries[i].message, MESSAGE_LEN - 1);
            errors[unique_count].message[MESSAGE_LEN - 1] = '\0';
            errors[unique_count].count = 1;
            unique_count++;
        }
    }

    return unique_count;
}

/* Simple descending bubble sort by count -- fine for small unique_count */
static void sort_errors_by_count(struct ErrorCount errors[], int unique_count)
{
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = 0; j < unique_count - i - 1; j++) {
            if (errors[j].count < errors[j + 1].count) {
                struct ErrorCount temp = errors[j];
                errors[j] = errors[j + 1];
                errors[j + 1] = temp;
            }
        }
    }
}

void print_summary(struct LogEntry entries[], int entry_count)
{
    int info, warning, error;
    count_levels(entries, entry_count, &info, &warning, &error);

    printf("\n===== LOG SUMMARY =====\n\n");
    printf("Total entries : %d\n", entry_count);
    printf("INFO          : %d\n", info);
    printf("WARNING       : %d\n", warning);
    printf("ERROR         : %d\n", error);

    struct ErrorCount errors[MAX_UNIQUE_ERRORS];
    int unique_count = count_unique_errors(entries, entry_count, errors);
    sort_errors_by_count(errors, unique_count);

    printf("\nMost common errors:\n\n");
    if (unique_count == 0) {
        printf("(none)\n");
    } else {
        int shown = unique_count < 5 ? unique_count : 5;
        for (int i = 0; i < shown; i++) {
            printf("%d. %s - %d\n", i + 1, errors[i].message, errors[i].count);
        }
    }
}

void search_keyword(struct LogEntry entries[], int entry_count, const char *keyword)
{
    int matches = 0;

    for (int i = 0; i < entry_count; i++) {
        if (strstr(entries[i].message, keyword) != NULL) {
            matches++;
        }
    }

    printf("\nFound %d matching entries.\n", matches);
}

void write_report(struct LogEntry entries[], int entry_count, const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Could not open '%s' for writing report.\n", filename);
        return;
    }

    int info, warning, error;
    count_levels(entries, entry_count, &info, &warning, &error);

    fprintf(fp, "===== LOG SUMMARY =====\n\n");
    fprintf(fp, "Total entries : %d\n", entry_count);
    fprintf(fp, "INFO          : %d\n", info);
    fprintf(fp, "WARNING       : %d\n", warning);
    fprintf(fp, "ERROR         : %d\n", error);

    struct ErrorCount errors[MAX_UNIQUE_ERRORS];
    int unique_count = count_unique_errors(entries, entry_count, errors);
    sort_errors_by_count(errors, unique_count);

    fprintf(fp, "\nMost common errors:\n\n");
    int shown = unique_count < 5 ? unique_count : 5;
    for (int i = 0; i < shown; i++) {
        fprintf(fp, "%d. %s - %d\n", i + 1, errors[i].message, errors[i].count);
    }

    fclose(fp);
    printf("\nReport written to %s\n", filename);
}
