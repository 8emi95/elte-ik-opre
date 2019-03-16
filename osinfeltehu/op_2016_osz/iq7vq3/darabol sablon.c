/*void darabol(char** darabok,char* darabolnivalo);

void darabol(char** darabok,char* darabolnivalo){
	int i=0;
	int j=0;
	int k=0;
	for(i=0;i<strlen(darabolnivalo);i++){
		if(darabolnivalo[i] == ';'){
			//printf("%s, ",darabok[j]);
			j++;
			k=0;
		} else {
			darabok[j][k++]=darabolnivalo[i];
		}	
	}
}*/

/*char** darabok;
	darabok = (char **) malloc(50);
	int i = 0;
	for(i = 0; i< sizeof(darabok);i++)
		darabok[i] = (char*) malloc(50);
	//memset(x[1], 0, sizeof(x[1]));
	/*	sprintf(darabok[1],"Szia");
		sprintf(darabok[2],"Hello");
		printf("%s\n",darabok[2]);
		printf("%s\n",darabok[1]);
		memset(darabok[1], 0, sizeof(darabok[1]));
		memset(darabok[2], 0, sizeof(darabok[2]));
	char str[100];
	sprintf(str,"13;52;1999;2000;Szia Bela!!!");
	darabol(darabok,str);
	for(i=0;i<sizeof(darabok);i++)
		printf("%s\n",darabok[i]);*/
