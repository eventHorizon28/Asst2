#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

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

void freeBST(BSTNode * temp)
{
	free(temp->occurs->filename);
	free(temp->occurs);
	free(temp);
}

void printSorted(BSTNode* temp)
{
	int i;
	if(temp == NULL)
		return;

	if(temp->left != NULL)
	{
		printSorted(temp->left);
		temp->left = NULL;
	}
	
	printf("word: %s file: %s reps: %d\n", temp->word, temp->occurs->filename, temp->occurs->reps);

	if(temp->right != NULL)
	{
		printSorted(temp->right);
		temp->right = NULL;
	}

	freeBST(temp);
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

	for(i = 0; i < old_size; i++)
	{
		new_heap[i].filename = sevnode->occurs[i].filename;
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
		if(strcmp(filename, sevnode->occurs[i].filename) == 0)
		{
			sevnode->occurs[i].reps++;
			heapify(sevnode, i);
		}
		else if(*(&(sevnode->occurs[i])) == 0)
			break;
	}
	//reached the end of heap, have to expand heap
	if(i == sevnode->heapsize)
	{
		expandHeap(sevnode, i);
		sevnode->occurs[i].reps = 1;
		heapify(sevnode, i);
	}

	//this is the first unique filename with that word
	if(*(&(sevnode->occurs[i])) == 0)
	{
		newfile = (char*) malloc(strlen(filename)+1);
		strcpy(newfile, filename);
		sevnode->occurs[i].filename = newfile;
		sevnode->occurs[i].reps = 1;
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
		new_node->occurs = (Heap*)calloc(HEAPSIZE, sizeof(Heap));
		new_node->heapsize = HEAPSIZE;
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
				new_node->occurs = (Heap*)calloc(HEAPSIZE, sizeof(Heap));
				new_node->heapsize = HEAPSIZE;
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
				new_node->occurs = (Heap*)calloc(HEAPSIZE, sizeof(Heap));
				new_node->heapsize = HEAPSIZE;
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
