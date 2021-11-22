WARNING = -Wall -Wshadow --pedantic
GCC = gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -O3 #edit to apply gcc commands
SRCS = pa3.c #files to run in 
OBJS = $(SRCS:%.c=%.o)

pa3: $(OBJS) #gcc files -o name of thing
	gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -O3 pa3.c -o pa3

testBuild: test1

test1: pa3
	./pa3 examples/4_5.b examples/answers/4_5.t examples/answers/4_5.f examples/answers/4_5.p
	

clean: # remove all machine generated files
	rm -f pa4 *.o output?
