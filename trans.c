/*
  Simple program demonstrates to do simple FILE access with
  fopen, fwrite, fseek and fread 
  http://manpages.courier-mta.org/htmlman3/fread.3.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>     // added by Gang-Ryung
#include <sys/types.h>  // added by Gang-Ryung

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

int main(int argc, char **argv)
{
    /* make sure that the user provides both an input and output file to the program */
    if(argc != 3) {
        printf("\nIncorrect usage of program. Correct usage is:\n");
        printf("trans <in file> <out file>\n\n");
        exit(-1);
    }

    pid_t pid;
    int fd[2];

    int shm_fd;
	void *ptr;
    const char *name = "MWOS";
    const int SIZE = 4096;

   	/* create the pipe */
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

    /* now fork a child process */
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

    FILE *fpIn, *fpOut;

	if (pid > 0) {  
        /* parent process */
		/* close the unused end of the pipe */
		close(fd[READ_END]);

        //char buffer[BUFFER_SIZE]; 
        /* open input file */ 
        fpIn = fopen(argv[1], "r"); 

        /* make sure input file exists */
        if (fpIn == NULL) { 
             fprintf(stderr, "Error opening file %s\n", argv[1]); 
             return -1;
        }

        /* read file into the pipe through the allocated buffer */ 
        // while (fgets(buffer, BUFFER_SIZE, fpIn)){
        //     write(fd[WRITE_END], buffer, strlen(buffer)+1);
        //     buffer[0]='\0'; 
        // } 
        /* create the shared memory segment */
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

        /* configure the size of the shared memory segment */
        ftruncate(shm_fd,SIZE);

        /* now map the shared memory segment in the address space of the process */
        ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED) {
            printf("Map failed\n");
            return -1;
        }

        /**
        * Now write to the shared memory region.
        *
        * Note we must increment the value of ptr after each write.
        */
        
        const char *message0= "Studying ";
        sprintf(ptr,"%s",message0);
        ptr += strlen(message0);

        /* close input file */ 
        fclose(fpIn);

		/* close write end of pipe */
		close(fd[WRITE_END]);
	}
	else { 
        /* child process */
		/* close end of the pipe since it is unused */
		close(fd[WRITE_END]);

        /* open the output file for writing and create if it doesnt exist */
        fpOut = fopen(argv[2], "w+");

        /* open the shared memory segment */
        shm_fd = shm_open(name, O_RDONLY, 0666);
        if (shm_fd == -1) {
            printf("shared memory failed\n");
            exit(-1);
        }

        /* now map the shared memory segment in the address space of the process */
        ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED) {
            printf("Map failed\n");
            exit(-1);
        }

        /* now read from the shared memory region */
        //printf("%s",(char *)ptr);  // type-cast Gang-Ryung
        fwrite(ptr,  1, strlen(ptr) + 1, fpOut);

        /* remove the shared memory segment */
        if (shm_unlink(name) == -1) {
            printf("Error removing %s\n",name);
            exit(-1);
        }

		/* close the file and write end of the pipe */
        fclose(fpOut);
		close(fd[READ_END]);
	}
   
   return(0);
}
