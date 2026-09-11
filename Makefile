CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = src/main.c src/parser.c src/analyzer.c
TARGET = log_analyzer

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) report.txt

.PHONY: clean
