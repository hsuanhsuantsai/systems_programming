all: mystat

mystat: file_perms.h file_perms.c mystat.c
	gcc -Wall -Werror -pedantic -std=gnu99 file_perms.c mystat.c -o mystat
	

clean:
	rm mystat
