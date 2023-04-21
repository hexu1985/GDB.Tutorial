CC = gcc
CFLAGS = -g3 -Wall -Wextra

SOURCES = $(shell ls *.c)
TARGET = test4

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"

