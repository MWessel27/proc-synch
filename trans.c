/*
  Simple program demonstrates to do simple FILE access with
  fopen, fwrite, fseek and fread 
  http://manpages.courier-mta.org/htmlman3/fread.3.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

        char buffer[BUFFER_SIZE]; 
        /* open input file */ 
        fpIn = fopen(argv[1], "r"); 

        /* make sure input file exists */
        if (fpIn == NULL) { 
             fprintf(stderr, "Error opening file %s\n", argv[1]); 
             return -1;
        }

        /* read file into the pipe through the allocated buffer */ 
        while (fgets(buffer, BUFFER_SIZE, fpIn)){
            write(fd[WRITE_END], buffer, strlen(buffer)+1);
            buffer[0]='\0'; 
        } 
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

        /* loop through the file and write to the buffer */
        while(1) {
            char buffer[BUFFER_SIZE];
            if (read(fd[READ_END], buffer, BUFFER_SIZE) != 0) {
                fprintf(fpOut,"%s",buffer);
                fflush(fpOut);
            } else {
                /* break if there are no characters left to read */
                break;
            } 
        }

		/* close the file and write end of the pipe */
        fclose(fpOut);
		close(fd[READ_END]);
	}
   
   return(0);
}
