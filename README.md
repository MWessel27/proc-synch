# Process Synchronization
# Description
Process synchronization for copying a file through shared POSIX memory and a parent and child process.
# Part 1: Getting Started
1. Clone this repo
2. scp the executed files to linprog
    - `scp -r help 'username'@linprog.cs.fsu.edu:/home/majors/'username'/'project directory'`
3. ssh into linprog and navigate to /'project directory'
4. Run `make clean`
5. Run `gcc -std=c99 -Wall -Wextra -D_XOPEN_SOURCE=700 -o trans trans.c -lrt`
6. `trans <in file> <out file>`

# Part 2: Setting up GDB
1. `make clean`
2. `make`
3. Open another terminal session and run gdb

# Helpful tips
- Quitting gdb session:
    - `Ctrl-d`
