/* Usage: pw_check username */
#define _XOPEN_SOURCE
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <shadow.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
	char pw[100]; int i=0;
	struct spwd *shadow_entry;
	char *p, *correct, *salt;
	if (argc < 2)
	{
		printf("Program usage: pw_check username\n");
		return 2;
	}
	/* Read the password from stdin */
	for(i=0;i<100;i++) pw[i]=0;
	scanf("%s",pw);
	printf("Jelszó: %s\n",pw);
	//p = fgets(password, sizeof(password), stdin);
	//if (p == NULL) return 2;
	//*p = 0;
	/* Read the correct hash from the shadow entry */
	shadow_entry = getspnam(argv[1]);
	if (shadow_entry == NULL) 
		{ printf("getspnam error: %s\n",argv[1]);
		return 1;
		}
	correct = (*shadow_entry).sp_pwdp;
	printf("Shadow value: %s\n",correct);
	/* Extract the salt */
	salt = strdup(correct);
	// 
	printf("Salt copy: %s\n",salt);	
	if (salt == NULL) return 2;
	p = strchr(salt + 1, '$');
	printf("Salt copy from second $: %s\n",p);	
	if (salt == NULL) return 2;
	p = strchr(p + 1, '$');
	// salt++; // salt points after the $
	printf("Salt copy from third $ : %s\n",p);	
	if (p == NULL) return 2;
	p[0] = 0;
	printf("The real salt: %s\n",salt);
	/*Encrypt the supplied password with the salt and compare the results*/
	char* titkos="almafa";
	titkos = (char*)crypt("katalin92", "$2z$05");
	printf("Crypted value: %s\n",titkos);
	if (titkos == NULL) return 2;
	i= strcmp(titkos, correct);
	if (!i) printf("Nem azonos!\n");
	return i;
}
