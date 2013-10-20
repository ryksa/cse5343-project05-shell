// prints the content of a local file
// named filename to console output
void type(char* filename)
{
	FILE *fp = fopen(filename, "r");
	char ch;
	printf("ELF");
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
