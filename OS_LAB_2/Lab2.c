#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <signal.h>
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;
  

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];
    //Checks if the given command line has < or > or not. 
    //If true, this means that the command line should be limited to data before 
    //< or >. This is used for I/O feature.
	if ((readChar=='<')||(readChar=='>')){break;} 
    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}


int main(int argc,char **argv[]) {    	
	int q=fork(); //This command forks into child process.
	if (q<0){ printf("Failure\n");}// If q<0, this means fork is not executed correctly.
	if (q==0){ // If q=0, this means that it is the child process.
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;
	int s;
	int a=0; // This variable is used for I/O feature to check whether < or > symbol is in command line or not.
	int size;
	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}
		
		/* END: TAKING INPUT */
		// This if statement checks whether !! is entered as input or not
		if (line[0]=='!'&&line[1]=='!'){
		// If !! is entered, then data from file program.txt is read. 
		// Program.txt is used for storing history of command prompt.
		//progarm.txt is stored in home folder.
   		FILE *fptr;

   		if ((fptr = fopen("program.txt","r")) == NULL){
       	printf("Error! opening file");

       	// File exits if the file pointer returns NULL.
       	exit(1);
   		}
		// Value of previous command from history file 'program.txt' is stored in line array.
		//Line array stores the command prompt input.
   		fscanf(fptr,"%s", &line);
    		fclose(fptr); // Closes the file.
   		}
   		
   		// This if statement checks if previous history is present or not.
   		if (line[0]==' '){
   			printf("No previous command");
   			exit(1);}
   		// This part of code writes value of line to program.txt file.
   		// Program.txt file is used as history file.	
   		FILE *fp;
		fp = fopen("program.txt","w");
	   	if(fp == NULL) //Checks if file opened or not.
		   {
		      printf("Error!");   
		      exit(1);             
		   }
		   fprintf(fp,"%s",line); //Stores value of line in file. Line contains command prompt input.
		   fclose(fp);

		// This part checks whether < or > is present in command line or not.
		// If present, value of a is updated accordingly.
		for(i =0; i < strlen(line); i++){
			if (line[i]=='<'){a=1;}
			else if (line[i]=='>'){a=2;}}
			
		line[strlen(line)] = '\n';
		tokens = tokenize(line);
		// If first array value of token is null, this means no data is entered on command prompt.
		//As a result, NO INPUT GIVEN is printed.      	
       	if (tokens[0]==NULL){       	
       	printf("No Input Given\n");
       	break;
       	}
       	//Else part executes if some input is given.
       	else{
       	//If a=2, then > sign was used in command line. Output is written to input.txt file.
       	if (a==2){
       	int fd;
       	//input.txt is the file to where data is written.
		fd=open("input.txt",O_CREAT|O_TRUNC|O_RDWR,0777);

		dup2(fd,1); // 1 means standard output.
		close(fd);
		// This command executes command line instructions.
		execvp(tokens[0],tokens);}
		//If a=1, then < sign was used in command line. Input is read from output.txt file.
		else if(a==1){
		int fd1;
		fd1=open("./output.txt",0666|O_RDONLY); //RDONLY meaning read only.
		dup2(fd1,0); // 0 means standard input.
		close(fd1);		
		execvp(tokens[0],tokens);}
		// If no < or > sign present, simply execute instructions.
		else{execvp(tokens[0],tokens);}}
		//Input.txt and output.txt are stored in same location as code file.		
				
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
	
	
	
	}}
	// Parent part. Parent waits until child is executed.
	else wait();
	return 0;
}
