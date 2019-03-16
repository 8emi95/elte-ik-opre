#include <stdio.h>
#include <pwd.h>
#include <shadow.h>

void main()
{
 struct spwd sh_entry;
 /* ez megjegyzes */
 // ez egy egysoros megjegyzes
 printf("Hajra Fradi!\n");
 struct passwd* iz;
 iz=getpwnam("illes");
 printf("Név: %s, Jelszó: %s\n",iz->pw_name, iz->pw_passwd);
 sh_entry=getspnam("illes");
 printf("/s \n",sh_entry.sp_pwdp);

}
