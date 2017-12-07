#filecopy
Implement unix command cp

## Usage
Command line: ./filecopy \<source\> \<destination\>

## What it does
1. copy source file to destination file  
	If destination file does not exist, create it.
2. deal with number of arguments issue
3. check whether source file/directory exists
4. check whether source is a directory
5. check whether source and dest are the same file (using inode to identify)
6. ordinary open/close, read/write issues

