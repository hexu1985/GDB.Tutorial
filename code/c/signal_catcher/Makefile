CC = gcc
CFLAGS = -g3 -Wall -Wextra

SOURCES = $(shell ls *.c)
TARGET = signal_catcher

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"

