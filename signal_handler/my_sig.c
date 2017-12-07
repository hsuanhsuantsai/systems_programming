//20170203
//Flora Tsai

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <limits.h>

static int count = 0;
long int MAXSTOP = LONG_MAX;

void int_handler(int sig){
	count++;
	if (count == MAXSTOP)
		return;
	printf("You've pressed Ctrl-C %d times.  Aren't you getting the message that I'm invulnerable?\n",count);
}

void usr_handler(int sig){
	printf("We received the SIGUSR1 signal.\n");
}

int main(int argc, char* argv[]){
	if (argc == 2){
		char *ptr;
		MAXSTOP = strtol(argv[1], &ptr, 10);
	}
	(void) signal(SIGINT, int_handler);
	signal(SIGUSR1, usr_handler);

	while(1){
		if (count == MAXSTOP)
			break;
	}
	return 0;
}