#!/bin/bash
gcc -O3 -std=c99 -Wall -Wshadow -Wvla -pedantic shell_array.c shell_list.c pa1.c -o pa1

./pa1 -a examples/15.b examples/15AT.b
diff examples/15AT.b examples/15sa.b

./pa1 -a examples/1K.b examples/1KAT.b
diff examples/1KAT.b examples/1Ksa.b

./pa1 -l examples/15.b examples/15LT.b
diff examples/15LT.b examples/15sl.b

./pa1 -l examples/1K.b examples/1KLT.b
diff examples/1KLT.b examples/1Ksl.b

echo Array
echo
valgrind -s pa1 -a examples/15.b examples/15AT.b

echo List
echo
valgrind -s --leak-check=full pa1 -l examples/15.b examples/15LT.b

echo
echo null
time ./pa1 -a examples/fake.b examples/fakeAT.b
time ./pa1 -l examples/fake.b examples/fakeLT.b

echo
echo empty
time ./pa1 -a examples/empty.b examples/emptyAT.b
time ./pa1 -l examples/empty.b examples/emptyLT.b

diff examples/empty.b examples/emptyAT.b
diff examples/empty.b examples/emptyLT.b

echo
echo FILE SIZE: 15
time ./pa1 -a examples/15.b examples/15AT.b
time ./pa1 -l examples/15.b examples/15LT.b

diff examples/15LT.b examples/15AT.b

echo
echo FILE SIZE: 1K
time ./pa1 -a examples/1K.b examples/1KAT.b
time ./pa1 -l examples/1K.b examples/1KLT.b

diff examples/1KLT.b examples/1KAT.b

echo
echo FILE SIZE: 10K
time ./pa1 -a examples/10K.b examples/10KAT.b
time ./pa1 -l examples/10K.b examples/10KLT.b

diff examples/10KLT.b examples/10KAT.b

echo Array 10K
echo
valgrind -s pa1 -a examples/10K.b examples/10KAT.b

echo List 10K
echo
valgrind -s --leak-check=full pa1 -l examples/10K.b examples/10KLT.b

echo
echo FILE SIZE: 100K
time ./pa1 -a examples/100K.b examples/100KAT.b
time ./pa1 -l examples/100K.b examples/100KLT.b

diff examples/100KLT.b examples/100KAT.b

echo 
echo FILE SIZE: 1M
time ./pa1 -a examples/1M.b examples/1MAT.b
time ./pa1 -l examples/1M.b examples/1MLT.b

diff examples/1MLT.b examples/1MAT.b
valgrind -s --leak-check=full pa1 -a examples/1M.b examples/10KAT.b
valgrind -s --leak-check=full pa1 -l examples/1M.b examples/10KLT.b
