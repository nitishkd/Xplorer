#ifndef _file_operation_h
#define _file_operation_h

void copyfile(char* filename, char* outfilename );

int removefile(char* filename);

void movefile(char* source, char* destination);

void copy_dir(char *source, char* dest);

void copy_dir_wrapper(char* source, char* dest);

#endif
