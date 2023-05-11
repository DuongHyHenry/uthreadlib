# Target library
lib	:=	libuthread.a
objs := queue.o uthread.o sem.o context.o preempt.o
CC := gcc
CFLAGS := -Wall -Werror -Wextra
CFLAGS += -c 

libuthread.a: $(objs)
	ar rcs libuthread.a $(objs)

%.o: %.c
	$(CC) $(CFLAGS) $o $<

clean:
	rm -f *.o *.a