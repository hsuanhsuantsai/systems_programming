# unnamed_pipe
Implement unnamed pipe with parent/child process

## Usage
* How to compile:  
  - gcc -std=c11 my_upipe.c -o my_upipe
  - or use `make` with makefile
* How to run:  
  ./my_upipe "arg1" "arg2"
  
## Running example
  * ./my_upipe "cat /etc/passwd" "grep root"
    - Result: `root:x:0:0:root:/root:/bin/bash`
  * cat /etc/passwd | grep root
    - Result: `root:x:0:0:root:/root:/bin/bash`

