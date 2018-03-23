#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>

#define BUFSIZE 500
struct fileNode{
char *fname;
int cnt;	
};
struct BSTNode{
	char * token;
	struct BSTNode * left;
	struct BSTNode * right;
	char filearray[3000];
	int reps;
};
char *Lastfile (char *name)
{
  char *s = name;
  char *last = strrchr (s, '/');
  if (last != NULL)
    {
      last = last + 1;
      return last;
    }
  if (last == NULL)
    {
      return s;
    }
}

void processDir(const char *name)
{
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
			//THiS IS JUST THE FILE
            //printf("%*s- %s\n", indent, "", entry->d_name);
        } 
    }
    closedir(dir);
} 

int file(const char *name){
errno=0;
DIR* directory = opendir(name);

if(directory!=NULL)
{
	
    closedir(directory);
    return 0; //if it is a directory then return 0 
}
if(errno==ENOTDIR)
{
    return 1; //if if errno is set not a directory return 1
}

    return 50;
}
int processF(const char*name)
{

//int dirFD = opendir(name);		
//struct dirent * currentEntry  = readdir(dirFD);
}

int main(int argc, char *argv[]){
 /*if (argc < 3) {
        printf("Provide two arguments\n");
        return 1;
    }*/   

//if file return 1 else return 0 for directoru
//You should be reading from argv[2] but for now argv[1]
/*const char *filename=argv[1];
const char *directory=argv[1];
if(file(filename)==1)
{
	
}
else if(file(filename)==0)
{
	//inside of ProcessDIr you need to check whether it's a file
 //  processDir(filename);
	
}*/
//printf("%s is it a file ?   %s ",filename,((file(filename)==1)? "yes":"no"));
//printf("%s is it a directory ?   %s ",directory,((file(directory)==0)? "yes":"no"));


     
return 0;
}


