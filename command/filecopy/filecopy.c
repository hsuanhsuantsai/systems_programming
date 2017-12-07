// Flora Tsai

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SIZE 1024

int main(int argc, char** argv){
	if (argc == 1){
		fprintf(stderr, "%s: missing file operand\n", argv[0]);
		return 1;
	}
	else if (argc == 2){
		fprintf(stderr, "%s: missing destination file operand after ‘%s’\n", argv[0], argv[1]);
		return 1;
	}
	else if (argc != 3){
		fprintf(stderr, "%s: target ‘%s’ is not a directory\n", argv[0], argv[argc-1]);
		return 1;
	}
	
	struct stat s_source;
	struct stat s_dest;
	char source[SIZE];
	char dest[SIZE];
	strcpy(source, argv[1]);
	strcpy(dest, argv[2]);
	stat(source, &s_source);
	stat(dest, &s_dest);

	if (access(source, F_OK) == -1){
		fprintf(stderr, "%s: cannot stat ‘%s’: No such file or directory\n", argv[0], source);
		return 1;
	}
	if (S_ISDIR(s_source.st_mode)){
		fprintf(stderr, "%s: omitting directory ‘%s’\n", argv[0], source);
		return 1;
	}

	int ifd = open(source, O_RDONLY);
	if (ifd == -1){
		fprintf(stderr, "Opening file %s error\n", source);
		return 1;
	}

	if (!strcmp(source, dest) && s_source.st_ino == s_dest.st_ino){
		fprintf(stderr, "%s: ‘%s’ and ‘%s’ are the same file\n", argv[0], source, dest);
		return 1;
	}
	

	if (S_ISDIR(s_dest.st_mode)){
		char *tail;
		const char c = '/';
		tail = strrchr(source, c);
		if (tail == NULL)
			strcat(dest, source);
		else{
			if (source[strlen(source) -1 ] == '/')
				strcat(dest,tail+1);
			else
				strcat(dest,tail);
		}
	}

	int ofd = open(dest, O_WRONLY | O_CREAT, 0664);
	if (ofd == -1){
		fprintf(stderr, "Opening file %s error\n", dest);
		return 1;
	}

	char buf[SIZE];
	ssize_t source_Read;

	while((source_Read = read(ifd, buf, SIZE)) > 0){
		if (write(ofd, buf, source_Read) != source_Read){
			fprintf(stderr, "Write error\n");
			break;
		}
	}

	if (source_Read == -1)
		fprintf(stderr, "Read error\n");

	if (close(ifd) == -1)
		fprintf(stderr, "Closing file %s error\n", source);
	if (close(ofd) == -1)
		fprintf(stderr, "Closing file %s error\n", dest);

	return 0;
}