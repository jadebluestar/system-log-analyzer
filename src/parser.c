#include <stdio.h>
#include <string.h>
#include "parser.h"

/*
 * Expected line format:
 * 2026-09-11 10:15:22 INFO System started
 * |------date------| |time| |lvl| |------ message ------|
 *
 * We read the date (10 chars) and time (8 chars) as one timestamp
 * field, then the level word, then the rest of the line is the message.
 */
int parse_line(const char *line, struct LogEntry *entry)
{
    char date[11];
    char time[9];
    char level[LEVEL_LEN];
    int chars_consumed = 0;

    /*
     * %10s  -> up to 10 non-space chars (the date, e.g. 2026-09-11)
     * %8s   -> up to 8 non-space chars  (the time, e.g. 10:15:22)
     * %9s   -> up to 9 non-space chars  (the level, e.g. WARNING)
     * %n    -> stores how many characters sscanf has consumed so far,
     *          so we know where the message starts.
     */
    int fields = sscanf(line, "%10s %8s %9s%n",
                         date, time, level, &chars_consumed);

    if (fields != 3) {
        return 0; /* line didn't match the expected shape */
    }

    /* Rebuild "date time" into one timestamp field */
    snprintf(entry->timestamp, TIMESTAMP_LEN, "%s %s", date, time);

    /* Copy the level, guaranteed to fit since %9s <= LEVEL_LEN-1 */
    strncpy(entry->level, level, LEVEL_LEN - 1);
    entry->level[LEVEL_LEN - 1] = '\0';

    /* Everything after the level (skip the single space separator) is the message */
    const char *message_start = line + chars_consumed;
    while (*message_start == ' ') {
        message_start++;
    }

    strncpy(entry->message, message_start, MESSAGE_LEN - 1);
    entry->message[MESSAGE_LEN - 1] = '\0';

    /* Strip trailing newline from the message, if present */
    size_t len = strlen(entry->message);
    if (len > 0 && entry->message[len - 1] == '\n') {
        entry->message[len - 1] = '\0';
    }

    return 1;
}
