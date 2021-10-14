WARNING = -Wall -Wshadow --pedantic
GCC = gcc #edit to apply gcc commands

#stack
stackSRCS = stack.c #files to run in 
OBJS = $(stackSRCS:%.c=%.o)
stack: $(OBJS)
	$(GCC) $(OBJS) -o stack
stackR: stack
	./stack



clean: # remove all machine generated files
	rm -f pa1 *.o output?
