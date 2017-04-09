/*
    Description: Process synchronization for copying a file through shared POSIX memory
        and a parent and child process.
    Author: Mikal Wessel
    Date: 04/09/2017
    FSU ID: mdw15d 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h> 

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

    /* Initialize vars */
    pid_t pid;
    int fd[2];

    int shm_fd;
	void *ptr;
    const char *name = "mdw15d_cop4610";
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

        /* open input file */ 
        fpIn = fopen(argv[1], "r"); 

        /* make sure input file exists */
        if (fpIn == NULL) { 
             fprintf(stderr, "Failed to read from file %s\n", argv[1]); 
             return -1;
        }

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

        /* Seek to the beginning of the file */
        fseek(fpIn, SEEK_SET, 0);

        /* Read and display data */
        fread(ptr, 1, SIZE, fpIn);

        /**
        * Now write to the shared memory region.
        *
        * Note we must increment the value of ptr after each write.
        */
        
        sprintf(ptr,"%s",(char *)ptr);
        ptr += strlen(ptr);

        /* close input file */ 
        fclose(fpIn);

		/* close write end of pipe */
		close(fd[WRITE_END]);
	}
	else { 
        /* child process */
		/* close end of the pipe since it is unused */
		close(fd[WRITE_END]);

        /* open input file */ 
        fpOut = fopen(argv[2], "r"); 

        /* make sure output file does not exist already */
        if (fpOut != NULL) { 
            printf("Output file [%s] already exists, please remove it before continuing.\n", argv[2]);                                                                                                                                                                
            return -1;
        }

        /* open the output file for writing and create if it does not exist */
        fpOut = fopen(argv[2], "w+");

        /* make sure output file is created */
        if (fpOut == NULL) { 
             fprintf(stderr, "Failed to create file %s\n", argv[2]); 
             return -1;
        }

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
