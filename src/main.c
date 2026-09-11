#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "analyzer.h"

#define MAX_ENTRIES 5000
#define LINE_BUF_LEN 256

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file '%s'\n", argv[1]);
        return 1;
    }

    static struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[LINE_BUF_LEN];

    while (fgets(line, LINE_BUF_LEN, fp) != NULL && entry_count < MAX_ENTRIES) {
        if (parse_line(line, &entries[entry_count])) {
            entry_count++;
        }
        /* lines that fail to parse are silently skipped */
    }

    fclose(fp);

    print_summary(entries, entry_count);

    char keyword[100];
    printf("\nEnter keyword to search:\n\n> ");
    if (fgets(keyword, sizeof(keyword), stdin) != NULL) {
        size_t len = strlen(keyword);
        if (len > 0 && keyword[len - 1] == '\n') {
            keyword[len - 1] = '\0';
        }
        if (strlen(keyword) > 0) {
            search_keyword(entries, entry_count, keyword);
        }
    }

    write_report(entries, entry_count, "report.txt");

    return 0;
}
