//20170118
//Name: Yi-Hsuan Tsai


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include "file_perms.h"

#define SIZE 1024

int printInfo(const struct stat *file, char *format){
	if (format){	//print out required info with formatting options
		for (int i=0; i<5; i++){
			if (format[2*i+1] == 'i')
				printf("%ld", file->st_ino);
			if (format[2*i+1] == 'g')
				printf("%ld", (long)file->st_gid);
			if (format[2*i+1] == 'h')
				printf("%ld", file->st_nlink);
			if (format[2*i+1] == 'u')
				printf("%ld", (long)file->st_uid);
			if (format[2*i+1] == 'D')
				printf("%lx", file->st_dev);
		}
		printf("\n");

		return 0;
	}

	char file_type[SIZE];
	switch (file->st_mode & S_IFMT){
		case S_IFSOCK: strcpy(file_type,"socket");					break;
		case S_IFLNK: strcpy(file_type,"symbolic link");			break;
		case S_IFREG: strcpy(file_type,"regular file");				break;
		case S_IFBLK: strcpy(file_type,"block special file");		break;
		case S_IFDIR: strcpy(file_type,"directory");				break;
		case S_IFCHR: strcpy(file_type,"character special file");	break;
		case S_IFIFO: strcpy(file_type,"FIFO");						break;
		default:	  strcpy(file_type,"unknown file");				break;
	}
	printf("  Size: %-10lld	BLocks: %-11lldIO Block: %-7ld%s\n", 
		(long long)file->st_size, (long long)file->st_blocks, (long)file->st_blksize, file_type);
	printf("Device: %lxh/%lud Inode: %-12ldLinks: %ld	", 
		file->st_dev, file->st_dev, file->st_ino, file->st_nlink);
	if (S_ISBLK(file->st_mode) || S_ISCHR(file->st_mode))
		printf("Device type: %x,%x", major(file->st_rdev), minor(file->st_rdev));
	printf("\n");

	struct passwd *pwd = getpwuid(file->st_uid);
	struct group *grp = getgrgid(file->st_gid);
	char c;
	if (S_ISSOCK(file->st_mode))
		c = 's';
	else if (S_ISLNK(file->st_mode))
		c = 'l';
	else if (S_ISBLK(file->st_mode))
		c = 'b';
	else if (S_ISDIR(file->st_mode))
		c = 'd';
	else if (S_ISCHR(file->st_mode))
		c = 'c';
	else
		c = '-';
	
	printf("Access: (%04lo/%c%s)  Uid: (%ld/%s)   Gid: (%ld/%s)\n",
		(long)file->st_mode%4096, c, filePermStr(file->st_mode,8), (long)file->st_uid,
		pwd->pw_name, (long)file->st_gid, grp->gr_name);

	struct tm * atm = localtime(&file->st_atime);
	struct tm * mtm = localtime(&file->st_mtime);
	struct tm * ctm = localtime(&file->st_ctime);
	char taccess[SIZE];
	char tmodify[SIZE];
	char tchange[SIZE];
	strftime(taccess,sizeof(taccess),"%F %X",atm);
	strftime(tmodify,sizeof(tmodify),"%F %X",mtm);
	strftime(tchange,sizeof(tchange),"%F %X",ctm);
	char azone[SIZE];
	char mzone[SIZE];
	char czone[SIZE];
	strftime(azone,sizeof(azone),"%z",atm);
	strftime(mzone,sizeof(mzone),"%z",mtm);
	strftime(czone,sizeof(czone),"%z",ctm);
	printf("Access: %s.%09lu %s\n", taccess, file->st_atim.tv_nsec, azone);
	printf("Modify: %s.%09lu %s\n", tmodify, file->st_mtim.tv_nsec, mzone);
	printf("Change: %s.%09lu %s\n", tchange, file->st_ctim.tv_nsec, czone);
	printf(" Birth: -\n");
	
	return 0;
}

int main(int argc, char** argv){
	if (argc == 1){
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		fprintf(stderr, "Usage: %s <-c, --format> <format sequences> <pathname>\n", argv[0]);
		return 1;
	}

	int flag = 0;
	if (!strcmp(argv[1],"-c") || !strcmp(argv[1],"--format") )
		flag = 1;

	struct stat file;

	if (flag){	//with -c flag
		struct stat temp;
		if (lstat(argv[2], &temp) != -1){	//argv[2] should be flag subset string
			printf("%s\n", argv[2]);
			return 1;
		}
		int i = 3;
		while(i < argc){
			if (lstat(argv[i], &file) == -1){
				perror("stat");
				return 1;
			}
			printInfo(&file,argv[2]);
			i++;
		}
	}
	else{
		int i = 1;
		while(i < argc){
			if (lstat(argv[i], &file) == -1){
				perror("stat");
				return 1;
			}
			if (S_ISLNK(file.st_mode)){	//print out the filename a soft link points to
				char *real = malloc(file.st_size + 1);
				readlink(argv[i], real, sizeof(real));
				printf("  File: '%s' -> '%s'\n", argv[i], real);
				free(real);
			}
			else
				printf("  File: '%s'\n", argv[i]);
			printInfo(&file,NULL);
			i++;
		}
	}
	
	return 0;
}