@ECHO OFF
gcc -O3 -std=c99 -Wall -Wshadow -Wvla -pedantic shell_list.c -o sl234
sl234.exe

EXIT
Use for testing pa1
gcc -O3 -std=c99 -Wall -Wshadow -Wvla -pedantic shell_array.c shell_list.c pa1.c -o pa1
pa1.exe -a pa1_examples/examples/15.b pa1_examples/examples/15AT.b

Use for testing shell_array.c
gcc shell_array.c -o shellArr
shellArr.exe

Use for testing your mom
gcc -O3 -std=c99 -Wall -Wshadow -Wvla -pedantic shell_list.c -o shellList
shellList.exe



