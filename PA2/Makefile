WARNING = -Wall -Wshadow --pedantic
GCC = gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -03 *.c -o pa4 #edit to apply gcc commands
SRCS = pa4.c #files to run in 
OBJS = $(SRCS:%.c=%.o)

sort: $(OBJS) #gcc files -o name of thing
	$(GCC) $(OBJS) -o pa4

testall: test1 test2 test3

test1: pa4
	./pa4 examples/ops0.b

clean: # remove all machine generated files
	rm -f pa4 *.o output?
