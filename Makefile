all: network.o server.o server

network.o: network.c network.h
	gcc -c $< 

server.o: server.c
	gcc -c $<

server: server.o network.o
	gcc $^ -o $@
