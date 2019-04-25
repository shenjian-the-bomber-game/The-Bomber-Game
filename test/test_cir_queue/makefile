CC := g++
RM := rm -f
OBJS := test.o
TARGETS := test
CXXFLAGS := -std=c++11 -Wall -Wextra

all : $(TARGETS)

test : test.o
	$(CC) $(CXXFLAGS) -o test test.o

clean : 
	$(RM) $(TARGETS) $(OBJS)

.PHONY : all clean