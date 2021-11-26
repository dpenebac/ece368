WARNING = -Wall -Wshadow --pedantic
GCC = gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -O3 #edit to apply gcc commands
SRCS = pa3.c #files to run in 
OBJS = $(SRCS:%.c=%.o)

pa3: $(OBJS) #gcc files -o name of thing
	gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -O3 pa3.c -o pa3

testAll: test1 test2 test3 test4

test1: pa3
	./pa3 examples/4_5.b examples/answers/4_5.t examples/answers/4_5.f examples/answers/4_5.p
	diff examples/answers/4_5.t examples/4_5.t
	diff examples/answers/4_5.f examples/4_5.f
	diff examples/answers/4_5.p examples/4_5.p

test2: pa3
	./pa3 examples/5_4.b examples/answers/5_4.t examples/answers/5_4.f examples/answers/5_4.p
	diff examples/answers/5_4.t examples/5_4.t
	diff examples/answers/5_4.f examples/5_4.f
	diff examples/answers/5_4.p examples/5_4.p

test3: pa3
	./pa3 examples/5_5.b examples/answers/5_5.t examples/answers/5_5.f examples/answers/5_5.p
	diff examples/answers/5_5.t examples/5_5.t
	diff examples/answers/5_5.f examples/5_5.f
	diff examples/answers/5_5.p examples/5_5.p

test4: pa3
	./pa3 examples/7_7.b examples/answers/7_7.t examples/answers/7_7.f examples/answers/7_7.p
	diff examples/answers/7_7.t examples/7_7.t
	diff examples/answers/7_7.f examples/7_7.f
	diff examples/answers/7_7.p examples/7_7.p
	

clean: # remove all machine generated files
	rm -f pa4 *.o output?
