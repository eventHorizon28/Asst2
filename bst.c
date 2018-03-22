#include<stdio.h>
#include<stdlib.h>

#define HEAPSIZE 50

typedef struct _Heap{
	char* filename;
	int reps;
} Heap;

typedef struct _BSTnode{
	char* word;
	struct BSTNode * left;
	struct BSTNode * right;
	Heap * occurs;
} BSTnode;

BSTNode * root;

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

int compareStrings(char *str1, char *str2)
{
	int capflag = 0;
	int i = 0;
	while ((str1[i] != '\0') || (str2[i] != '\0'))
	{

		if (str1[i] < str2[i])
		{
			capflag = findcap (str1[i], str2[i]);
			if (capflag == 1)
			{
				return 1;
			}
			else if (capflag == -1)
			{
				return -1;
			}
			else if (capflag == 0)
			{
				return 1;
			}
		}

		if (str1[i] > str2[i])
		{
			capflag = findcap (str1[i], str2[i]);
			if (capflag == 1)
			{
				return 1;
			}
			else if (capflag == -1)
			{
				return -1;
			}
			else if (capflag == 0)
			{
				return -1;
			}
		}

		i++;
	}

	if(strlen(str1) == strlen(str2))
		return 0;
	else if(strlen(str1) > strlen(str2))
		return -1;
	else
		return 1;
	
}


void heapify(int index)
{

}


void printSorted(struct BSTNode* temp)
{
	int i;
	if(temp == NULL)
		return;

	if(temp->left != NULL)
	{
		printSorted(temp->left);
		temp->left = NULL;
	}
	
	for(i = 0; i< temp->reps; i++)
	{
		printf("%s\n", temp->str_value);
	}

	if(temp->right != NULL)
	{
		printSorted(temp->right);
		temp->right = NULL;
	}

	free(temp);
}

void addfile(Heap * heap_ptr, char * filename)
{
	int i;
	char* newfile;

	for(i = 0; i < HEAPSIZE; ++i)
	{
		if(strcmp(filename, heap_ptr[i]->filename) == 0)
		{
			heap_ptr[i]->reps++;
			heapify(heap_ptr, i);
		}
		else if(heap_ptr[i] == 0)
			break;
	}

	//this is the first unique filename with that word
	if(heap_ptr[i] == 0)
	{
		newfile = (char*) malloc(strlen(filename)+1);
		strcpy(newfile, filename);
		heap_ptr[i]->filename = newfile;
		heap_ptr[i]->reps = 1;
	}
}

void createNode(char * word, char * filename)
{
	struct BSTNode * new_node;
	struct BSTNode * temp = root;
	char * new_word;
	int compare_value;
	

	//if the word has numbers in the starting
	//
	//

	if(root == NULL)
	{
		new_node = (struct BSTNode*)malloc(sizeof(struct BSTNode));
		new_word = (char*)malloc((strlen(word)+1)*sizeof(char));
		strcpy(new_word, word);
		new_node->str_value = new_word;
		new_node->occurs = (Heap*)calloc(HEAPSIZE, sizeof(Heap));
		addfile(new_node->occurs, filename);
		root = new_node;
		return;
	}
	compare_value = compareStrings(word, temp->str_value);

	while(1)
	{
		//the new string is the same as the string in the current node
		if(compare_value == 0)
		{
			addfile(temp->occurs, filename);
			return;
		}
		//the new string is less priority than the string in the current node, go right
		else if(compare_value == -1)
		{
			if(temp->right == NULL)
			{
				new_node = (struct BSTNode*)malloc(sizeof(struct BSTNode));
				new_word = (char*)malloc((strlen(word)+1)*sizeof(char));
				strcpy(new_word, word);
				new_node->str_value = new_word;
				new_node->occurs = (Heap*)calloc(HEAPSIZE, sizeof(Heap));
				addfile(new_node->occurs, filename);
				temp->right = new_node;
				return;
			}
			else
			{
				temp = temp->right;
				compare_value = compareStrings(word, temp->str_value);
				continue;
			}
		}
		else if(compare_value == 1)
		{
			if(temp->left == NULL)
			{
				new_node = (struct BSTNode*)malloc(sizeof(struct BSTNode));
				new_word = (char*)malloc((strlen(word)+1)*sizeof(char));
				strcpy(new_word, word);
				new_node->str_value = new_word;
				new_node->occurs = (Heap*)calloc(HEAPSIZE, sizeof(Heap));
				addfile(new_node->occurs, filename);
				temp->left = new_node;
				return;
			}
			else
			{
				temp = temp->left;
				compare_value = compareStrings(word, temp->str_value);
				continue;
			}
		}
	}
}
