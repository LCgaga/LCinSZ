SOURCES:=$(wildcard  *.c)
OBJS:=$(patsubst %.c,%.o,$(SOURCES))
CFLAGS:=-Wall -g
ELF:=stu
CC:=gcc
$(ELF):$(OBJS)
	$(CC) $(CFLAGS) -o $(ELF) $(OBJS)
%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $^
.PHONY:clean
clean:
	rm $(OBJS) $(ELF)
