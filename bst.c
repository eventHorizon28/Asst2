#include<stdio.h>
#include<stdlib.h>

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

void createNode(char * name)
{
	struct BSTNode * new_node;
	struct BSTNode * temp = root;
	char * new_name;
	int compare_value;
	
	if(root == NULL)
	{
		new_node = (struct BSTNode*)malloc(sizeof(struct BSTNode));
		new_name = (char*)malloc((strlen(name)+1)*sizeof(char));
		strcpy(new_name, name);
		new_node->str_value = new_name;
		new_node->reps = 1;
		root = new_node;
		return;
	}
	compare_value = compareStrings(name, temp->str_value);

	while(1)
	{
		if(compare_value == 0)
		{
			strcmp(
			temp->occurs[]->reps++;
			return;
		}
		else if(compare_value == -1)
		{
			if(temp->right == NULL)
			{
				new_node = (struct BSTNode*)malloc(sizeof(struct BSTNode));
				new_name = (char*)malloc((strlen(name)+1)*sizeof(char));
				strcpy(new_name, name);
				new_node->str_value = new_name;
				new_node->reps = 1;
				temp->right = new_node;
				return;
			}
			else
			{
				temp = temp->right;
				compare_value = compareStrings(name, temp->str_value);
				continue;
			}
		}
		else if(compare_value == 1)
		{
			if(temp->left == NULL)
			{
				new_node = (struct BSTNode*)malloc(sizeof(struct BSTNode));
				new_name = (char*)malloc((strlen(name)+1)*sizeof(char));
				strcpy(new_name, name);
				new_node->str_value = new_name;
				new_node->reps = 1;
				temp->left = new_node;
				return;
			}
			else
			{
				temp = temp->left;
				compare_value = compareStrings(name, temp->str_value);
				continue;
			}
		}
	}
}
