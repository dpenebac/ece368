WARNING = -Wall -Wshadow --pedantic
GCC = gcc #edit to apply gcc commands

#stack
bstSRCS = bst.c #files to run in 
OBJS = $(bstSRCS:%.c=%.o)
bst: $(OBJS)
	$(GCC) $(OBJS) -o bst

run:
	bst

clean: # remove all machine generated files
	rm -f pa1 *.o output?
