# Process Synchronization
# Part 1: Set up
1. Download the example c code from here: https://campus.fsu.edu/bbcswebdav/pid-8463098-dt-content-rid-51758210_2/xid-51758210_2
2. Unzip the files
3. scp the executed files to linprog
    - `scp -r help 'username'@linprog.cs.fsu.edu:/home/majors/'username'/'project directory'`
4. ssh into linprog and navigate to /'project directory'
5. Run `make clean`
6. Run `gcc -std=c99 -Wall -Wextra -D_XOPEN_SOURCE=700 -o trans trans.c -lrt`

# Part 2: Setting up GDB
1. `make clean`
2. `make`
3. Open another terminal session and run gdb

# Helpful tips
- Quitting gdb session:
    - `Ctrl-d`
