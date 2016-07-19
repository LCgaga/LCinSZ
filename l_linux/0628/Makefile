all:p1 p2
.PHONY:all
p1:p1.o
	gcc p1.o -o p1
p2:p2.o
	gcc p2.o -o p2
%.o:%.c
	gcc -o $@ -c $^

