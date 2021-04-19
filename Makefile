all : grep clean
color.o : color.c color.h
	        gcc -Wall -c -g color.c
basic.o : basic.c basic.h
	        gcc -Wall -c -g basic.c
grep.o : grep.c grep.h
	        gcc -Wall -c -g grep.c
rexp.o : rexp.c rexp.h
	        gcc -Wall -c -g rexp.c
main.o : main.c color.c color.h grep.c grep.h basic.c basic.h rexp.c rexp.h
	        gcc -Wall -c -g main.c
grep : color.o grep.o main.o basic.o rexp.o
	        gcc -Wall -g color.o grep.o main.o basic.o rexp.o -o grep
clean :
	        rm *.o