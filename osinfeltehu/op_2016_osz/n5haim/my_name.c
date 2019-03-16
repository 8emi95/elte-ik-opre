#include<stdio.h>
#include<stdlib.h>
#include<pwd.h>

int main()
{
    uid_t user_id;
    struct passwd pwd;

    user_id = geteuid();
    pwd = getpwuid(user_id);

    struct FILE* out_file = fopen("my_name.txt","w+");
    fputs(out_file,pwd.pw_name);
    fclose(out_file);

    return 0;
}
