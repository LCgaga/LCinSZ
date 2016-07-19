OBJS:=$(wildcard *.c)
server:$(OBJS)
	gcc $^ -o server -pthread -I../include -lcrypt
.PHONY:clean
clean:
	rm server
