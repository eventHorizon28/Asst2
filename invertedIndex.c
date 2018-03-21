#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<dirent.h>
#include"bst.h"

#define BUFF_SIZE 100

void createNode(char* token, char* filename)
{
	printf("%s %s\n", token, filename);
}

void lowerCase(char* word)
{
	int length = strlen(word);
	int i = 0;
	
	for(i = 0; i < length; ++i)
	{
		word[i] = tolower(word[i]);
	}
}

void tokenize(int index_fd, char* filename)
{
	char buffer[BUFF_SIZE];
	int i = 0, j = 0, temp_file_i = 0;
	int file_i = read(index_fd, buffer, BUFF_SIZE-1);//reads 99 bytes
	char* buff_ptr = buffer;
	int prev_buff_len = 0;
	//while there are valid new reads
	while(file_i + prev_buff_len)
	{
		//insert a NULL terminator to the end of the char array
		buffer[BUFF_SIZE-1] = '\0';

		//iterate thru the array to interchange non-alphanumeric characters to '\0'
		for(i = 0; i < file_i+prev_buff_len; ++i)
		{
			if(!isalnum(buffer[i]))
				buffer[i] = '\0';
		}

		//if numBytes is less than 100
		if((file_i + prev_buff_len) < (BUFF_SIZE-1))
		{
			//createNode out of all the strings
			while(buff_ptr < (buffer + file_i + prev_buff_len))
			{
				if(strlen(buff_ptr) != 0)
				{
					lowerCase(buff_ptr);
					createNode(buff_ptr, filename);
				}

				buff_ptr += strlen(buff_ptr) + 1;
			}
			//verify: setting prev_buff_len to 0
			prev_buff_len = 0;
			break;
		}

		else
		{
			while(buff_ptr < buffer + file_i + prev_buff_len)
			{
				//if buff_ptr is at the last string in the array
				if(buff_ptr == buffer+file_i+prev_buff_len-strlen(buff_ptr))
				{
					prev_buff_len = strlen(buff_ptr);
					//copy the last string to first index
					for(j = 0; buff_ptr+j < (buffer+BUFF_SIZE); ++j)
					{
						buffer[j] = *(buff_ptr+j);
					}

					//make all other block = '\0'
					buff_ptr = buffer+strlen(buffer);
					while(buff_ptr < (buffer+BUFF_SIZE))
					{
						*buff_ptr = '\0';
						buff_ptr += 1;
					}

					//read next lines into buffer+strlen(buff_ptr)
					file_i = read(index_fd, buffer+strlen(buffer), BUFF_SIZE-1);
					buff_ptr = buffer;
					break;
				}

				else
				{
					if(strlen(buff_ptr) != 0)
					{
						lowerCase(buff_ptr);
						createNode(buff_ptr, filename);
					}
					buff_ptr += strlen(buff_ptr) + 1;
				}
			}
		}
	}
}

void traverseDir(char* direc)
{
	DIR * dirp = opendir(direc);
	struct dirent * dp;

	if(!dirp)
	{
		printf("Could not open directory %s: ", direc);
		perror("");
		return;
	}

	dp = readdir(dirp);
	
	//while ( dp != NULL)
		//if dp.d_type = file
			//tokenize(file fd)
		//else
			//traverseDir(directory name)
}

int main(int argc, char** argv)
{
	int out_fd;
	int index_fd;
	char overwrite;

	if(argc != 3)
	{
		printf("ERROR: give output file and directory/file to search\n");
		return 0;
	}

	//opening/creating output file
	out_fd = open(argv[1], O_RDWR);
	if(out_fd == -1)
		out_fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
	else
	{
		printf("output file %s already exists, do you wish to overwrite it? y/n", argv[1]);
		scanf("%c", &overwrite);
		
		while(overwrite != 'y' && overwrite != 'n')
			scanf("%c", &overwrite);

		if(overwrite == 'y')
			;
		else
		{
			printf("exiting program...\n");
			return 0;
		}
	}
	//checking if the argv[2] is directory or file
	index_fd = open(argv[2], O_RDWR);
	if(index_fd == -1)
	{
		if(errno = EISDIR)
		{
			traverseDir(argv[2]);
		}
		
		else
		{
			printf("Could not read the file/directory %s\n", argv[2]);
		}
	}
	else
	{
		tokenize(index_fd, argv[2]);
	}
}
