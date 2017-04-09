CC = gcc
CFLAGS = -g -O0 -std=c99 -Wall -Wextra -D_XOPEN_SOURCE=700
LIBS = -lrt

all: clean shm-posix-producer shm-posix-consumer unix-pipe file-read-write

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

shm-posix-producer: shm-posix-producer.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

shm-posix-consumer: shm-posix-consumer.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

unix-pipe: unix-pipe.o
	$(CC) $(CFLAGS) -o $@ $<

file-read-write: file-read-write.o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o *~ \#* shm-posix-producer shm-posix-consumer unix-pipe file-read-write file.txt
	@echo "@ Check the following link for -D_XOPEN_SOURCE=700"
	@echo "@ http://stackoverflow.com/questions/5378778/what-does-d-xopen-source-do-mean"
