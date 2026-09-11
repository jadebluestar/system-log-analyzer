#ifndef PARSER_H
#define PARSER_H

#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10
#define MESSAGE_LEN 100

/* One parsed log line */
struct LogEntry {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MESSAGE_LEN];
};

/*
 * Parses one raw log line into a LogEntry.
 * Returns 1 on success, 0 if the line could not be parsed
 * (e.g. malformed / doesn't match the expected format).
 */
int parse_line(const char *line, struct LogEntry *entry);

#endif
