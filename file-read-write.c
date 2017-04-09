/*
  Simple program demonstrates to do simple FILE access with
  fopen, fwrite, fseek and fread 
  http://manpages.courier-mta.org/htmlman3/fread.3.html
*/

#include <stdio.h>
#include <string.h>

int main()
{
   FILE *fp;
   char c[] = "this is tutorialspoint";
   char buffer[100];

   /* Open file for both reading and writing */
   fp = fopen("file.txt", "w+");

   /* Write data to the file */
   fwrite(c,  1, strlen(c) + 1, fp);

   /* Seek to the beginning of the file */
   fseek(fp, SEEK_SET, 0);

   /* Read and display data */
   fread(buffer, 1, strlen(c)+1, fp);
   printf("%s\n", buffer);
   fclose(fp);
   
   return(0);
}
