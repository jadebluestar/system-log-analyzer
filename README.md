# System Log Analyzer (C)

A small terminal-based tool that reads a plain-text log file, summarizes it,
and lets you search it by keyword. Written in C for Linux, as a fundamentals
project (file I/O, structs, pointers, string parsing, modular headers, Makefile).

## Build

```
make
```

## Run

```
./log_analyzer data/sample.log
```

Example output:

```
===== LOG SUMMARY =====

Total entries : 15
INFO          : 5
WARNING       : 3
ERROR         : 7

Most common errors:

1. Sensor timeout - 3
2. Connection failed - 3
3. Memory allocation failed - 1

Enter keyword to search:

> timeout

Found 3 matching entries.

Report written to report.txt
```

## Expected log line format

```
YYYY-MM-DD HH:MM:SS LEVEL Message text here
```

`LEVEL` is one of `INFO`, `WARNING`, `ERROR`. Lines that don't match this shape
are skipped rather than crashing the program.

## Project layout

```
log-analyzer/
├── src/
│   ├── main.c       # reads argv, reads the file line by line, orchestrates
│   ├── parser.c/h    # turns one raw line into a LogEntry struct
│   ├── analyzer.c/h  # counts levels, ranks errors, keyword search, reports
├── data/
│   └── sample.log
├── Makefile
└── README.md
```

## Clean up build artifacts

```
make clean
```
