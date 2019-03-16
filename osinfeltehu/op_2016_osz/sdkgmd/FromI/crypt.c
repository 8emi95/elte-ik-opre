#include <stdio.h>
#include <string.h>
#define _XOPEN_SOURCE
#include <unistd.h>
#include <pwd.h>

int main()
{
 /* ez megjegyzes */
 // ez egy egysoros megjegyzes
 struct passwd *pw;
 printf("Az almafa1 szoveget crypteljuk!\n");
 char titkos[100];
 char *t=(char*)crypt("almafa1","$2a$05$Epv5CvAKOwz9u.qemagF4e");
 strcpy(titkos,t);
 char* titkos1=(char*)crypt("almafa1","barack");
 printf("Sima crypt eredmeny: %s\n",titkos);
 printf("Sajat so hasznalat : %s\n",titkos1);
 pw=getpwnam("illes");
 printf("User neve: %s\n",pw->pw_name);
 // ez a shadow hasznalatnal csak x-et ad
 printf("Illes jelszava: %s\n",pw->pw_passwd);
 return 1;
}