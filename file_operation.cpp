#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "file_operation.h"

void copyfile(char* filename, char* outfilename )
{
	char block[1024];
	int in, out,nread;
	in = open(filename, O_RDONLY);
	out = open(outfilename, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
	while((nread = read(in, block,sizeof(block))) > 0)
		write(out, block,nread);
		
}

int removefile(char* filename)
{
	int status = remove(filename);
	return status;	
}

void movefile(char* source, char* destination)
{
	copyfile(source, destination);
	removefile(source);
}
