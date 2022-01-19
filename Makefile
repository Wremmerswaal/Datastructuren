CC = g++

CompileParms = -c -O0 -Wall -std=c++11

OBJS = main.o tree.o node.o basic.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o main

basic.o: basic.cc basic.h
	$(CC) $(CompileParms) basic.cc

node.o: node.cc node.h basic.h
	$(CC) $(CompileParms) node.cc

tree.o: tree.cc tree.h node.h basic.h
	$(CC) $(CompileParms) tree.cc

main.o: main.cc node.h tree.h basic.h
	$(CC) $(CompileParms)  main.cc