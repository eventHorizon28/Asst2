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

#define HEAPSIZE 50

typedef struct _Heap{
	char* filename;
	int reps;
} Heap;

typedef struct _BSTNode{
	char* word;
	struct _BSTNode * left;
	struct _BSTNode * right;
	Heap * occurs;
	int heapsize;
} BSTNode;

BSTNode * root = NULL;


int compareStrings(char *str1, char *str2)
{
	int capflag = 0;
	int i = 0;
	while ((str1[i] != '\0') || (str2[i] != '\0'))
	{
	
		//(str1 has higher precedence than str2) ? 1 : -1
		if(isalpha(str1[i]) && isalpha(str2[i]))
		{
			if (str1[i] < str2[i])
			{
				return 1;
			}


			if (str1[i] > str2[i])
			{
				return -1;
			}
		}

		if(isalpha(str1[i]) && isdigit(str2[i]))
			return 1;
		if(isdigit(str1[i]) && isalpha(str2[i]))
			return -1;
		if(isdigit(str1[i]) && isdigit(str2[i]))
			return (str1[i]<str2[i]) ? 1 : 0;


		//SPECIAL case: filename
		if(str1[i] == '.' && str2[i] != '.')
			return -1;
		if(str1[i] != '.' && str2[i] == '.')
			return 1;

		i++;
	}

	
	if(strlen(str1) == strlen(str2))
		return 0;
	else if(strlen(str1) > strlen(str2))
		return -1;
	else
		return 1;
	
}

int compareOccurs(Heap ptr1, Heap ptr2)
{
	//1 if the first(left) child has to siftup, -1 if the second(right) child has to
	if(ptr1.reps > ptr2.reps)
		return 1;
	if(ptr2.reps < ptr2.reps)
		return -1;

	return compareStrings(ptr1.filename, ptr2.filename);
}

void printSorted(BSTNode * temp)
{
	int i;
	int j = 0;

	if(temp == NULL)
		return;

	if(temp->left != NULL)
	{
		printSorted(temp->left);
		temp->left = NULL;
	}
	
	for(i = 0; i < temp->heapsize; ++i)
	{
		if(temp->occurs[i].reps == -1)
			break;
		printf("word: %s file: %s reps: %d\n", temp->word, temp->occurs[i].filename, temp->occurs[i].reps);
		free(temp->occurs[i].filename);

		
		//sifting up after removing one element from the heap
		j = i;
		while(j < temp->heapsize && temp->occurs[j].reps != -1)
		{
			
			if(temp->occurs[2*j + 1].reps == -1 && temp->occurs[2*j + 2].reps == -1)
				break;
			else if(temp->occurs[2*j + 1].reps == -1 && temp->occurs[2*j + 2].reps != -1)
			{
				temp->occurs[j].filename = temp->occurs[2*j + 2].filename;
				temp->occurs[j].reps = temp->occurs[2*j + 2].reps;
				break;
			}
			else if(temp->occurs[2*j + 1].reps != -1 && temp->occurs[2*j + 2].reps == -1)
			{
				temp->occurs[j].filename = temp->occurs[2*j + 1].filename;
				temp->occurs[j].reps = temp->occurs[2*j + 1].reps;
				break;
			}	
			
			if(compareOccurs(temp->occurs[2*j + 1], temp->occurs[2*j + 2]) == 1)
			{
				//left child needs to become parent
				temp->occurs[j].filename = temp->occurs[2*j + 1].filename;
				temp->occurs[j].reps = temp->occurs[2*j + 1].reps;
				j = 2*j + 1;
			}
			else
			{
				//right child needs to become parent
				temp->occurs[j].filename = temp->occurs[2*j + 2].filename;
				temp->occurs[j].reps = temp->occurs[2*j + 2].reps;
				j = 2*j + 2;
			}		
		}
		temp->heapsize--;
	}

	if(temp->right != NULL)
	{
		printSorted(temp->right);
		temp->right = NULL;
	}

	free(temp->occurs);
	free(temp);
}

/*No capital letters
int findcap (char c, char c2)
{

	int i = 0;
	int upper;

	if (isupper (c) && !isupper (c2))
	{
		return 1;
	}
	else if (!isupper (c) && isupper (c2))
	{
		return -1;
	}                                               
	return 0;
}
*/


void swap(BSTNode* sevnode, int parent_index, int index)
{
	char* temp_filename;
	int temp_reps;

	temp_filename = sevnode->occurs[parent_index].filename;
	temp_reps = sevnode->occurs[parent_index].reps;

	sevnode->occurs[parent_index].filename = sevnode->occurs[index].filename;
	sevnode->occurs[parent_index].reps = sevnode->occurs[index].reps;

	sevnode->occurs[index].filename = temp_filename;
	sevnode->occurs[index].reps = temp_reps;
}

void heapify(BSTNode* sevnode, int index)
{
	int parent_index = -1;

	if((index - 1) % 2 == 0)
	{
		parent_index = (index - 1) % 2;
	}
	else
	{
		parent_index = (index - 2) % 2;
	}

	//if child has higher reps, swap with parent
	while(index > 0 && sevnode->occurs[index].reps > sevnode->occurs[parent_index].reps)
	{
		swap(sevnode, parent_index, index);

		index = parent_index;

		if((index - 1) % 2 == 0)
			parent_index = (index - 1) % 2;
		else
			parent_index = (index - 2) % 2;
	}

	//if both have the same reps, compare filenames and swap appropriately
	while(index > 0 && sevnode->occurs[index].reps == sevnode->occurs[parent_index].reps)
	{
		if(compareStrings(sevnode->occurs[parent_index].filename, sevnode->occurs[index].filename) == -1)
		{
			swap(sevnode, parent_index, index);

			index = parent_index;
			if((index - 1) % 2 == 0)
				parent_index = (index - 1) % 2;
			else
				parent_index = (index - 2) % 2;
		}
		else
			break;
	}
}

void expandHeap(BSTNode * sevnode, int old_size)
{
	int i = 0;

	Heap * new_heap;
	new_heap = (Heap*)calloc(2*old_size, sizeof(Heap));
	sevnode->heapsize = 2*old_size;

	for(i = 0; i < 2*old_size; i++)
	{
		if(i<old_size)
			new_heap[i].filename = sevnode->occurs[i].filename;
		else
			new_heap[i].reps = -1;
	}

	free(sevnode->occurs);
	sevnode->occurs = new_heap;
}

void addfile(BSTNode * sevnode, char * filename)
{
	int i;
	char* newfile;

	for(i = 0; i < sevnode->heapsize; ++i)
	{
		if((sevnode->occurs[i].reps) == -1)
			break;
		else if(strcmp(filename, sevnode->occurs[i].filename) == 0)
		{
			sevnode->occurs[i].reps++;
			heapify(sevnode, i);
			break;
		}
	}
	//reached the end of heap, have to expand heap
	if(i == sevnode->heapsize)
	{
		expandHeap(sevnode, i);
		sevnode->occurs[i].reps = 1;
		heapify(sevnode, i);
	}

	//this is the first unique filename with that word
	if((sevnode->occurs[i].reps) == -1)
	{
		newfile = (char*) malloc(strlen(filename)+1);
		strcpy(newfile, filename);
		sevnode->occurs[i].filename = newfile;
		sevnode->occurs[i].reps = 1;
		
		if(i != 0)
			heapify(sevnode, i);
	}
}

void createNode(char * word, char * filename)
{
	BSTNode * new_node;
	BSTNode * temp = root;
	char * new_word;
	int compare_value;
	int i  = 0;

	//if the word has numbers in the starting
	while(isdigit(word[i]))
	{
		++i;
	}
	word += i;
	if(strlen(word) == 0)
		return;

	if(root == NULL)
	{
		//creating new BSTNode and adding the new word to it
		new_node = (BSTNode*)malloc(sizeof(BSTNode));
		new_word = (char*) malloc((strlen(word)+1)*sizeof(char));
		strcpy(new_word, word);
		new_node->word = new_word;
		//adding the filename on to the newly created heap
		new_node->occurs = (Heap*)malloc(HEAPSIZE * sizeof(Heap));
		new_node->heapsize = HEAPSIZE;

			//to indicate unused blocks in the heap array
			for(i = 0; i < HEAPSIZE; ++i)
			{
				new_node->occurs[i].reps = -1;
			}
	
		addfile(new_node, filename);
		root = new_node;
		return;
	}
	compare_value = compareStrings(word, temp->word);

	while(1)
	{
		//the new string is the same as the string in the current node
		if(compare_value == 0)
		{
			addfile(temp, filename);
			return;
		}
		//the new string is less priority than the string in the current node, go right
		else if(compare_value == -1)
		{
			if(temp->right == NULL)
			{
				//creating a new node and setting its data
				new_node = (BSTNode*)malloc(sizeof(BSTNode));
				//the word that needs to be set in the BST
				new_word = (char*)malloc((strlen(word)+1)*sizeof(char));
				strcpy(new_word, word);
				new_node->word = new_word;
				//the filename that needs to be set in the BST-Node heap
				new_node->occurs = (Heap*)malloc(HEAPSIZE * sizeof(Heap));
				new_node->heapsize = HEAPSIZE;

					//to indicate unused blocks in the heap array
					for(i = 0; i < HEAPSIZE; ++i)
					{
						new_node->occurs[i].reps = -1;
					}

				addfile(new_node, filename);
				temp->right = new_node;
				return;
			}
			else
			{
				temp = temp->right;
				compare_value = compareStrings(word, temp->word);
				continue;
			}
		}
		else if(compare_value == 1)
		{
			if(temp->left == NULL)
			{
				//putting the new word in the bst
				new_node = (BSTNode*)malloc(sizeof(BSTNode));
				new_word = (char*)malloc((strlen(word)+1)*sizeof(char));
				strcpy(new_word, word);
				new_node->word = new_word;
				//the filename that needs to be set in the BSTNode heap
				new_node->occurs = (Heap*)malloc(HEAPSIZE * sizeof(Heap));
				new_node->heapsize = HEAPSIZE;
					
					//to indicate unused blocks in the heap array
					for(i = 0; i < HEAPSIZE; ++i)
					{
						new_node->occurs[i].reps = -1;
					}
				addfile(new_node, filename);
				temp->left = new_node;
				return;
			}
			else
			{
				temp = temp->left;
				compare_value = compareStrings(word, temp->word);
				continue;
			}
		}
	}
}

/*void createNode(char* token, char* filename)
 * {
 *         printf("%s %s\n", token, filename);
 *         }
 *         */

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
	//
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
void processDir(const char *name)
{
	int fd;
	DIR *dir;
	struct dirent *entry;
   //IF you have a file run process file  
	if (!(dir == opendir(name)))
		return;

	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_DIR)
		{
			char * Lastfile(char *name )
			{
				char *s =name;  
				char *last = strrchr(s, '/');   
				if (last != NULL)
				{
				    last=last+1;
				    return last;
			    	}
				if (last == NULL) {
					return s;   
				}

			}
			char source[3000];
		    
			/* recursively print directory contents, except for . and .. */
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
		    /* concatenate current path and subdir name */
			snprintf(source, sizeof(source), "%s/%s", name, entry->d_name);
		    //printf("%*s[%s]\n", indent, "", entry->d_name);
			processDir(source);
		}
		else
		{ 
			fd = open(entry->d_name, O_RDWR);
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

void endProgram()
{
	printSorted(root);
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
	
	endProgram();                                                                                                                          

	close(out_fd);                                                                                                                          
	close(index_fd);                                                                                                                        
}                                                                                                                                               

