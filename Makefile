libuthread.a: queue.o uthread.o sem.o context.o preempt.o
	ar rcs libuthread.a queue.o uthread.o sem.o context.o preempt.o

%.o: %.c
	gcc -Wall -Werror -Wextra -c $o $<

clean:
	rm -f *.o *.a