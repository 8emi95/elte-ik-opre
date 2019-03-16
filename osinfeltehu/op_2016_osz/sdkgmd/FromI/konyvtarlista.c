#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void print_dir(const char* dir,int indent);
char* copy_cat(const char* s1,const char* s2);

int main()
{
    print_dir(".",0);
    return 0;
}

void print_dir(const char* dir,int indent)
{
    int i;
    DIR* dp;
    struct dirent* dirp;
    if((dp=opendir(dir)))
    {
	while((dirp=readdir(dp)))
	{
	    for(i=0;i<indent;++i) printf("\t");
	    printf("%s\n",dirp->d_name);
	    if(dirp->d_type&DT_DIR && strcmp(".",dirp->d_name)!=0 && strcmp("..",dirp->d_name)!=0)
	    {
		char* d2 = copy_cat(dir,dirp->d_name);
		print_dir(d2,indent+1);
		free(d2);
	    }
	}
	closedir(dp);
    }
}

char* copy_cat(const char* s1,const char* s2)
{
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    char* s = (char*)malloc(len1+len2+2);
    memcpy(s,s1,len1);
    *(s+len1) = '/';
    memcpy(s+len1+1,s2,len2+1);
    return s;
}
