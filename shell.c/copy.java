// copy the contents of file filename1 to file
// filename2
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
