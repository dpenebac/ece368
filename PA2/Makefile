WARNING = -Wall -Wshadow --pedantic
GCC = gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -03 *.c -o pa4 #edit to apply gcc commands
SRCS = pa4.c #files to run in 
OBJS = $(SRCS:%.c=%.o)

sort: $(OBJS) #gcc files -o name of thing
	$(GCC) $(OBJS) -o pa4

testall: test1 test2

test1: pa4
	./pa4 -b examples/ops0.b

test2: pa4
	./pa4 -b examples/ops1.b

test3: pa4
	./pa4 -b examples/ops3.b

testmem: pa4
	valgrind ./pa4 -b examples/ops3.b

clean: # remove all machine generated files
	rm -f pa4 *.o output?
