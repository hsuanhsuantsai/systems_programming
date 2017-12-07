//20170208
//Flora Tsai

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define CHILD (pid_t) 0
#define SIZE 1024

int main(){
	static char message[SIZE];
	char* myfifo = "MYFIFO";
	int fifo;
	int n;

	if (mkfifo(myfifo, 0666) < 0)
		printf("%s already exists.\n", myfifo);
	else
		printf("Creating named pipe: %s\n", myfifo);

	if ((fifo = open(myfifo, O_RDONLY)) < 0){
		perror("open: ");
		exit(-1);
	}

	printf("Waiting for input...Got it: ");
	while((n = read(fifo, message, sizeof(message))) > 0){
		printf("'%s'\n", message);
		if (!strcmp(message, "exit")){
			printf("Exiting");
			break;
		}
		memset(message, '\0', SIZE);
		printf("Waiting for input...Got it: ");
	}
	putchar('\n');
	close(fifo);
	return 0;
}