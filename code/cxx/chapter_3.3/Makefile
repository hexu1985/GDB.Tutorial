CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
LDFLAGS = -lpthread

SOURCES = $(shell ls *.cpp)
TARGET = chapter_3.3

all: $(TARGET)
	@echo "TARGET = $(TARGET)" 

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY:
clean:
	$(RM) $(TARGET) a.out core *.o
	@echo "clean OK!"

