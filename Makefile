PROG = power.o
CC = g++
FLAGS = -g -Wall -std=c++11
OBJS = power.cpp

$(PROG) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(PROG)

clean:
	rm *.o
