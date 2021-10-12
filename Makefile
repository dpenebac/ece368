WARNING = -Wall -Wshadow --pedantic
GCC = gcc #edit to apply gcc commands
SRCS = main.c sort.c #files to run in 
OBJS = $(SRCS:%.c=%.o)

sort: $(OBJS) #gcc files -o name of thing
	$(GCC) $(OBJS) -o pa1

testall: test1 test2 test3

test1: pa1
	./sort inputs/test1 > output1
	diff output1 expected/expected1

test2: pa1
	./sort inputs/test2 > output2
	diff output2 expected/expected2

test3: pa1
	./sort inputs/test3 > output3
	diff output3 expected/expected3

clean: # remove all machine generated files
	rm -f pa1 *.o output?
