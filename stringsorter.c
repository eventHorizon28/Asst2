#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

struct BSTNode{
	char * str_value;
	struct BSTNode * left;
	struct BSTNode * right;
	int reps;
};
struct BSTNode * root;

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

//check if the free works in this method!
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
			temp->reps++;
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

int main(int argc, char** argv)
{
	//char tmp;
	int i = 0;
	char* new_word;
	char* tmp_str;

	if(argc != 2)
	{
		printf("ERROR: program expects 1 input, given %d\n", argc-1);
		return 0;
	}
	root = NULL;
	new_word = (char*)malloc(1024*sizeof(char));
	strcpy(new_word, "");
	tmp_str = (char*)malloc(1024*sizeof(char));
	//reading strings
	//tmp = *argv[1]+i;

	while(argv[1][i] != '\0')
	{
		//tmp = *argv[1]+i;
		if(isalpha(argv[1][i]))
		{
			strcpy(tmp_str, new_word);
			snprintf(new_word, 1024, "%s%c", tmp_str, argv[1][i]);
		}

		else
		{
			if(strcmp(new_word, "") == 0)
			{
				i++;
				continue;
			}
			createNode(new_word);
			strcpy(new_word, "");
			strcpy(tmp_str, "");
		}
		i++;
	}
	if(strcmp(new_word, "") != 0)
		createNode(new_word);
	if(root != NULL)
		printSorted(root);
	
	free(tmp_str);
	free(new_word);
}
