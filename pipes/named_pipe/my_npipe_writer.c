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

	//if (mkfifo(myfifo, 0666) < 0)
	//	printf("%s already exists.\n", myfifo);

	if ((fifo = open(myfifo, O_WRONLY)) < 0){
		perror("open: ");
		exit(-1);
	}
	else
		printf("Opening named pipe: %s\n", myfifo);

	printf("Enter Input: ");
	while(fgets(message, SIZE, stdin) != NULL){
		message[strlen(message)-1] = '\0';
		write(fifo, message, strlen(message));
		printf("Writing buffer to pipe...done\n");
		if (!strcmp(message, "exit")){
			printf("Exiting");
			break;
		}
		memset(message, '\0', SIZE);
		printf("Enter Input: ");
	}
	putchar('\n');
	close(fifo);
	return 0;
}