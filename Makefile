CC = g++

CompileParms = -c -O0 -Wall -std=c++11

OBJS = graph.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o G

graph.o: graph.cc tree.h node.h basic.h
	$(CC) $(CompileParms)  graph.cc