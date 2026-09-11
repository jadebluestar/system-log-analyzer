# System Log Analyzer (C)

A small terminal-based tool that reads a plain-text log file, summarizes it,
and lets you search it by keyword. 

<img width="480" height="270" alt="image" src="https://github.com/user-attachments/assets/6260d8cd-2d88-471b-9a12-bd8a47304383" />

A simplified, hand-rolled version of what `grep`, `awk`, or log-management tools (Splunk, ELK stack, journalctl) do for a living, i.e, turning a raw stream of log lines into "what happened, how often, and where."


## Build

```
make
```

## Run

```
./log_analyzer data/sample.log
```

Example output:

<img width="480" height="270" alt="image" src="https://github.com/user-attachments/assets/67208051-de60-432b-8f32-942e07a96638" />


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
