OBJS:=$(wildcard *.c)
server:$(OBJS)
	gcc $^ -o server -pthread -I../include
.PHONY:clean
clean:
	rm server
