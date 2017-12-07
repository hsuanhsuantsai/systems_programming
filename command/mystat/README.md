# mystat
Implement unix command stat

## Usage
Command line: 
1. ./mystat \<pathname\> (default)
2. ./mystat \<-c, --format\> \<format sequences\> \<pathname\>  
Note that format sequences must be put inside ""
-format subset : %i, %g, %h, %u, %D 
				(inode number, Group ID of owner, Number of hard links, User ID of owner, Device number in hex)
## Other
I use file_perms.h and file_perms.c, which are downloaded from http://man7.org/tlpi/code/online/dist/files/file_perms.h.html in order to deal with permission output.

