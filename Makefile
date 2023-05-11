# Target library
lib	:=	libuthread.a
objs := queue.o uthread.o context.o sem.o preempt.o

libuthread.a: $(objs)
	ar rcs libuthread.a $(objs)

%.o: %.c
	gcc -Wall -Werror -Wextra -c $o $<

clean:
	rm -f *.o *.a