CC = gcc
CFLAGS = -g3 -Wall -Wextra

SOURCES = $(shell ls *.c)
TARGET = swapflaw

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"

