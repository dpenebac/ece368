WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR)
TESTFLAGS = -DTEST_ADD -DTEST_SUB -DTEST_MUL -DTEST_DIV 

SRCS = add.c div.c mul.c  main.c sub.c

OBJS = $(SRCS:%.c=%.o)

main: $(OBJS) 
	$(GCC) $(TESTFLAGS) $(OBJS) -o main

.c.o: 
	$(GCC) $(TESTFLAGS) -c $*.c 

testall: testadd testsub testmul testdiv

testadd: main
	./main 4 5 A > add1.out
	diff add1.out add1.correct
	./main -2 17 A > add2.out
	diff add2.out add2.correct

testsub: main
	./main 4 5 S > sub1.out
	diff sub1.out sub1.correct
	./main 7 26 S > sub2.out
	diff sub2.out sub2.correct

testmul: main
	./main 4 5 M > mul1.out
	diff mul1.out mul1.correct
	./main 7 26 M > mul2.out
	diff mul2.out mul2.correct

testdiv: main
	./main 240 5 D > div1.out
	diff div1.out div1.correct
	./main 70 26 D > div2.out
	diff div2.out div2.correct

coverage: testall
	gcov $(SRCS)

clean: # remove all machine generated files
	rm -f main *.o *.out *gcda *gcno *gcov

