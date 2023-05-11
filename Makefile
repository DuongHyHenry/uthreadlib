# Target library
lib	:=	libuthread.a
objs := queue.o uthread.o sem.o context.o preempt.o
CC := gcc
CFLAGS := -Wall -Werror -Wextra
CFLAGS += -c

libuthread.a: queue.o uthread.o sem.o context.o preempt.o
	ar rcs libuthread.a queue.o uthread.o sem.o context.o preempt.o

%.o: %.c
	gcc -Wall -Werror -Wextra $o $<

clean:
	rm -f *.o *.a