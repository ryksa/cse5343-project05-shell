#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define INPUT_SIZE 100
#define CMD_ARGS_SIZE  10
#define  CMD_OK 0
#define CMD_ERR 1

/* parse a string to extract and save all tokens in a string array */
void parse_input(char* input, char *cmd[], int* n);

/* print contents of a file to console output */
void print_file(char* filename);

/* copy contents of filename1 into filename2 */
void copy(char* filename1, char* filename2);

int main(int argc, char *argv[])
{
	char input[INPUT_SIZE];    /* user input read from keyboard */	
	char *cmd[CMD_ARGS_SIZE];  /* parsed user commands */	
	int args_len;              /* number of arguments in user command */
	
	// loop to read and process all user commands
	while(1)
	{
		// display shell prompt
		printf("shell> ");
		
		// read user input from keyboard
		fgets(input, sizeof(input), stdin);
		// remove the trailing newline from input
		input[strlen(input) - 1] = '\0';
		
		// terminate the shell if user entered 'exit'
		if(strcmp(input,"exit") == 0)
		{
			break;
		}
		
		// parse input to extract command and arguments
		parse_input(input,cmd, &args_len);
		
		// process the command
		if(strcmp(cmd[0],"type") == 0)
		{
			// type <filename>
			print_file(cmd[1]);
		}
		else if(strcmp(cmd[0],"copy") == 0)
		{
			// copy <filename1> <filename2>
			copy(cmd[1],cmd[2]);
		}
		else if(strcmp(cmd[0],"delete") == 0)
		{
			// delete <filename>
			if(remove(cmd[1]) != 0)
			{
				printf("error: file %s could not be deleted!\n", cmd[0]);
			}
		}
		else
		{
			// check whether command is an executable program and if yes
			// then run it
			struct stat sb;
			if(stat(cmd[0],&sb) == 0 &&  sb.st_mode & S_IXUSR)
			{
				// command is a valid executable program, so run it
				// in a separate process
				int pid;  /* process id */
				if((pid = fork()) == 0)
				{
					// child process where program is loaded and executed
					execvp(cmd[0],cmd);
				}
				else
				{
					// parent process wait for child process to complete execution
					// of the program
					waitpid(pid,NULL,0);
				}
			}
			else
			{
				// input is neither a shell command nor an executale program
				printf("Invalid command!\n"); 
			}
		}
	}
	return 0;
}

void parse_input(char* input, char *cmd[], int* n)
{
	int i=0;
	*n = 0;
	char* token = strtok(input," ");
	while(token != NULL)
	{
		cmd[i++] = token;
		token = strtok(NULL," ");
	}
	cmd[i] = '\0';
}

void print_file(char* filename)
{
	FILE *fp = fopen(filename, "r");
	char ch;
	
	// display error message if file could not be opened
	if(fp == NULL)
	{
		printf("error: file %s not found!\n", filename);
		return;
	}
	
	// read and print content of the file byte by byte
	while( (ch = fgetc(fp)) != EOF)
	{
		putc(ch,stdout);
	}
	
	// close the file
	fclose(fp);
}

void copy(char* filename1, char* filename2)
{
	FILE *fOut;  /* file output stream */
	FILE *fIn;   /* file input stream */
	
	char ch;     /* byte read from filename1 */
	
	// open filename2 for writing
	fOut = fopen(filename2, "w");
	
	// open filename1 for reading
	fIn = fopen(filename1, "r");
	
	// check whether files has been properly opened for reading/writing
	if(fIn == NULL)
	{
		printf("error: file %s could not be opened for reading!\n", filename1);
		return;
	}
	if(fOut == NULL)
	{
		printf("error: file %s could not be created for writing!\n", filename2);
		return;
	}
	
	// write content from filename1 to filename2 byte by byte
	while( (ch = fgetc(fIn)) != EOF)
	{
		putc(ch,fOut);
	}
	
	// close the input and output files
	fclose(fIn);
	fclose(fOut);
}
