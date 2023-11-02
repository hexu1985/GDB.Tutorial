CC = gcc
CFLAGS = -g3 -Wall -Wextra 

SOURCES = $(shell ls *.c)
TARGET = sieve

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread -lm

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"

