WARNING = -Wall -Wshadow --pedantic
GCC = gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -03 *.c -o pa4 #edit to apply gcc commands
SRCS = pa4.c #files to run in 
OBJS = $(SRCS:%.c=%.o)

sort: $(OBJS) #gcc files -o name of thing
	$(GCC) $(OBJS) -o pa4

testBuild: test1 test2 test3 test4 testBuildmem testErr

test1: pa4
	./pa4 -b examples/ops0.b examples/test.b
	./pa4 -e examples/test.b

test2: pa4
	./pa4 -b examples/ops1.b examples/test.b
	./pa4 -e examples/test.b

test3: pa4
	./pa4 -b examples/ops2.b examples/test.b
	./pa4 -e examples/test.b

test4: pa4
	./pa4 -b examples/ops3.b examples/test.b
	./pa4 -e examples/test.b

testErr: pa4
	./pa4 -l examples/ops3.b examples/test.b
	./pa4 -e examples/test.b

testt: pa4
	./pa4 -t examples/ops3.b examples/test.b

testBuildmem: pa4
	valgrind --leak-check=full ./pa4 -b examples/ops3.b examples/test.b
	echo
	valgrind --leak-check=full ./pa4 -e examples/test.b

testTree: pa4
	./pa4 -e examples/tree0.b
	./pa4 -e examples/tree1.b
	./pa4 -e examples/tree2.b
	./pa4 -e examples/tree3.b
	./pa4 -e examples/invalidtree0.b
	./pa4 -e examples/invalidtree1.b
	./pa4 -e examples/invalidtree2.b

testRand: pa4
	./pa4 -test examples/ops3.b examples/test.b
	./pa4 -e examples/test.b

clean: # remove all machine generated files
	rm -f pa4 *.o output?
