//20170208
//Flora Tsai

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

#define CHILD (pid_t) 0
#define SIZE 1024

int parser(char** ptr, char* buffer){
	char temp[SIZE];
	int narg = 0;
	int index = 0;
  	for (int i=0; i<SIZE; i++){
    	if (buffer[i] == '\0'){
	      	temp[index++] = '\0';
	      	ptr[narg] = (char*)malloc(sizeof(temp));
	      	strcpy(ptr[narg], temp);
	      	break;
    	}
	    if (buffer[i] == ' '){
	      temp[index] = '\0';
	      ptr[narg] = (char*)malloc(sizeof(temp));
	      strcpy(ptr[narg], temp);
	      index = 0;
	      narg++;
	      continue;
	    }
	    temp[index++] = buffer[i];
	}
	ptr[narg+1] = NULL;
//	for (int i=0; ptr[i] != NULL; i++)
//		printf("%s\n", ptr[i]);
	return 0;
}

int main(int argc, char* argv[]){
	if (argc != 3){
		fprintf(stderr, "Usage: ./my_upipe \"arg1\" \"arg2\"\n");
		return 1;
	}
	pid_t child;
	int fdes[2];

	if ( pipe(fdes) == -1){
		perror("Pipe");
	}

	char* child_p[SIZE];
	char* parent_p[SIZE];
	parser(child_p, argv[1]);
	parser(parent_p, argv[2]);

	if ((child = fork()) == CHILD){
		dup2(fdes[1], fileno(stdout));
		close(fdes[0]);
		close(fdes[1]);
		execvp(child_p[0], child_p);
		exit(5);
	}
	else{
		dup2(fdes[0], fileno(stdin));
		close(fdes[0]);
		close(fdes[1]);
		execvp(parent_p[0], parent_p);
		exit(6);
	}
	return 0;
}