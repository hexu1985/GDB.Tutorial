CXX = g++
CXXFLAGS = -g3 -Wall -Wextra

SOURCES = $(shell ls *.cc)
TARGET = exception_catcher

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"

