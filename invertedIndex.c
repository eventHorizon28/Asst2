#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<dirent.h>
//#include"bst.h"

//#define BUFF_SIZE 100


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
	int buff_size = 100;
	char * buffer = (char*)malloc(buff_size*sizeof(char));
	int i = 0, j = 0, temp_file_i = 0;
	int file_i = read(index_fd, buffer, buff_size-1);//reads 99 bytes
	char* buff_ptr = buffer;
	int prev_buff_len = 0;
	char * new_buff;

	int null_count = 0;
	//while there are valid new reads
	while(file_i + prev_buff_len)
	{
		//insert a NULL terminator to the end of the char array
		buffer[buff_size-1] = '\0';

		//iterate thru the array to interchange non-alphanumeric characters to '\0'
		for(i = 0; i < file_i+prev_buff_len; ++i)
		{
			if(!isalnum(buffer[i]))
			{
				buffer[i] = '\0';
				null_count++;
			}
		}

		//if 1 string is more than 100 characters
		if(null_count == 0)
		{
			//expandBuffer
			new_buff = (char*) malloc ((2*buff_size) * sizeof(char));

			//copy elements
			for(i = 0; i < buff_size; i++)
			{
				new_buff[i] = buffer[i];
			}

			prev_buff_len = strlen(buffer);
			buff_size *= 2;

			free(buffer);
			buffer = new_buff;

			//scan new stuff
			file_i = read(index_fd, buffer+strlen(buffer), buff_size-1-strlen(buffer));
			buff_ptr = buffer;
			continue;
		}

		//if numBytes is less than 100
		if((file_i + prev_buff_len) < (buff_size-1))
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
					for(j = 0; buff_ptr+j < (buffer+buff_size); ++j)
					{
						buffer[j] = *(buff_ptr+j);
					}

					//make all other block = '\0'
					buff_ptr = buffer+strlen(buffer);
					while(buff_ptr < (buffer+buff_size))
					{
						*buff_ptr = '\0';
						buff_ptr += 1;
					}

					//read next lines into buffer+strlen(buff_ptr)
					file_i = read(index_fd, buffer+strlen(buffer), buff_size-1-strlen(buffer));
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

void processDir(const char *name)
{   int fd;
    DIR *dir;
    struct dirent *entry;
   //IF you have a file run process file  
    if (!(dir = opendir(name)))
    
        return;

while ((entry = readdir(dir)) != NULL) {
	
        if (entry->d_type == DT_DIR) {
char * Lastfile(char *name ){
char *s =name;  
char *last = strrchr(s, '/');   
if (last != NULL) {
    last=last+1;
    return last;
}
if (last == NULL) {
 return s;   
}
	
}char source[3000];
            /* recursively print directory contents, except for . and .. */
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
                /* concatenate current path and subdir name */
            snprintf(source, sizeof(source), "%s/%s", name, entry->d_name);
            //printf("%*s[%s]\n", indent, "", entry->d_name);
            processDir(source);
        } else { 
		fd = open(entry->d_name, O_RDONLY|O_WRONLY);
		  if (fd < 0)
 		  {
	           exit(1);
		  }
		   tokenize(fd,entry->d_name);
			//THiS IS JUST THE FILE
            //printf("%*s- %s\n", indent, "", entry->d_name);
        } 
    }
    closedir(dir);
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
			processDir(argv[2]);
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

//	printSorted();

	close(out_fd);
	close(index_fd);
}
