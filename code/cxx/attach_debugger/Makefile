CXX = g++
CXXFLAGS = -g3 -Wall -Wextra
LDFLAGS = 
LDLIBS = 

PROGS =	static_conflict1 static_conflict2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

static_conflict1: initial_delay.o static_conflict.o MyClass.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

static_conflict2: initial_delay.o MyClass.o static_conflict.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

