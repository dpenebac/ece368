WARNING = -Wall -Wshadow --pedantic
GCC = gcc #edit to apply gcc commands

#stack
SRCS = stack.c #files to run in 
OBJS = $(SRCS:%.c=%.o)

s: $(OBJS)
	$(GCC) $(OBJS) -o stack

testStack: stack
	./stack

clean: # remove all machine generated files
	rm -f pa1 *.o output?
