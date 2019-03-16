       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <sys/msg.h> 
	   #include <signal.h>
	   #include <time.h>
	//
	// unnamed pipe example
	//
	
	
	typedef int bool;
const int true=1;
const int false=0;

key_t kulcs; 

struct string2
{
	char string[255];
	int length;
};
typedef struct string2 str;

void szokiir(str cout)
{
	int i;
	for(i=0; i<cout.length; ++i)
	{
		printf("%c",cout.string[i]);
	}
}

void szokiirnl(str cout)
{
	szokiir(cout);
	printf("\n");
}


str szobeker(str cout)
{
	szokiirnl(cout);
	str ret;
	int db=0;
	char seged;
	seged=getchar();
	while(seged!='\n')
	{
		ret.string[db]=seged;
		++db;
		seged=getchar();
	}
	ret.length=db;
	return ret;
}


str stringToStr(char input[])
{
	str retvalue;
	int i=0;
	while(input[i] != '\0')
	{
		retvalue.string[i]=input[i];
		++i;
	}
	retvalue.length=i;
	return retvalue;
}

bool StrCompare(str A, str B)
{
	if (A.length!=B.length)
	{
		return false;
	}
	else
	{
		int i=0;
		while(i<A.length && A.string[i]==B.string[i])
		{
			++i;
		}
		if(i==A.length)
			return true;
		else
			return false;
	}
}

struct date
{
	int year,month,day,h,min;
};
typedef struct date date;

bool CompareDate(date A, date B)
{
	int Aarr[5]={A.year,A.month,A.day,A.h,A.min};
	int Barr[5]={B.year,B.month,B.day,B.h,B.min};
	
	int i=0;
	while(i<5 && Aarr[i]==Barr[i])
	{
		++i;
	}
	return (i<5);
}

date StrToDate(str input)
{
	date retvalue;
	int i=0;
	//year
	retvalue.year=0;
	while(input.string[i]!='.')
	{
		retvalue.year*=10;
		retvalue.year+=input.string[i]-48;
		++i;
	}
	++i;
	//month
	retvalue.month=0;
	while(input.string[i]!='.')
	{
		retvalue.month*=10;
		retvalue.month+=input.string[i]-48;
		++i;
	}
	++i;
	//day
	retvalue.day=0;
	while(input.string[i]!='.')
	{
		retvalue.day*=10;
		retvalue.day+=input.string[i]-48;
		++i;
	}
	i+=2;
	//hour
	retvalue.h=0;
	while(input.string[i]!=':')
	{
		retvalue.h*=10;
		retvalue.h+=input.string[i]-48;
		++i;
	}
	++i;
	//min
	retvalue.min=0;
	int j=i+2;
	while(i<j)
	{
		retvalue.min*=10;
		retvalue.min+=input.string[i]-48;
		++i;
	}
	
	return retvalue;
}


str DateToStr(date input)
{
	str retvalue;
	date temp=input;
	int i=0;
	//year
	int del=temp.year;
	while(del!=0)
	{
		del=del/10;
		++i;
	}
	i-=1;
	retvalue.length=i+2;
	retvalue.string[i+1]='.';
	for(i; i>=0; --i)
	{
		retvalue.string[i]=(temp.year%10)+48;
		temp.year=temp.year/10;
	}
	//month
	retvalue.string[retvalue.length]=(temp.month/10)+48;
	retvalue.string[retvalue.length+1]=(temp.month%10)+48;
	retvalue.string[retvalue.length+2]='.';
	retvalue.length+=3;
	//day
	retvalue.string[retvalue.length]=(temp.day/10)+48;
	retvalue.string[retvalue.length+1]=(temp.day%10)+48;
	retvalue.string[retvalue.length+2]='.';
	retvalue.length+=3;
	//hour
	retvalue.string[retvalue.length]=' ';
	retvalue.string[retvalue.length+1]=(temp.h/10)+48;
	retvalue.string[retvalue.length+2]=(temp.h%10)+48;
	retvalue.string[retvalue.length+3]=':';
	retvalue.length+=4;
	//min
	retvalue.string[retvalue.length]=(temp.min/10)+48;
	retvalue.string[retvalue.length+1]=(temp.min%10)+48;
	retvalue.length+=2;
	
	return retvalue;
}

bool DateValid(date d)
{
	int monthtop[12]={31,29,31,30,31,30,31,31,30,31,30,31};
	if(
	d.month>0 && d.month<13 &&
	d.day>0 && d.day<=monthtop[d.month+1] &&
	d.h>=0 && d.h<24 && 
	d.min>=0 && d.min<60
	)
	{
		return true;
	}
	else
	{
		return false;
	}
}

struct jarat
{
	str celallomas;
	date indulas;
};
typedef struct jarat jarat;

void MenuKiir()
{
	printf("Kerem valasszon a menupontok kozul\n");
	printf("1 - Celallomasok litazasa\n");
	printf("2 - Uj celallomas felvetele\n");
	printf("3 - Celallomas torlese\n");
	printf("4 - Uj jelentkezo\n");
	printf("5 - Jelentkezo adatainak modositasa\n");
	printf("6 - Jelentkezo torlese\n");
	printf("7 - Jaratok infromacioi\n");
	printf("8 - Jarat inditasa\n");
	printf("0 - Kilepes\n");
}

jarat sorfelbont(char line[160])
{
	jarat retvalue;
	int i=0;
	while(line[i] != '\t')
	{
		retvalue.celallomas.string[i] = line[i];
		++i;
	}
	retvalue.celallomas.length=i;
	++i;
	str datumstr;
	int j=0;
	while(line[i] != '\n')
	{
		datumstr.string[j] = line[i];
		++i;
		++j;
	}
	datumstr.length=j;
	retvalue.indulas = StrToDate(datumstr);
	return retvalue;
}

date earliest(date A,date B)
{
	int Aarr[5]={A.year,A.month,A.day,A.h,A.min};
	int Barr[5]={B.year,B.month,B.day,B.h,B.min};
	
	int i=0;
	for(i; i<5; ++i)
	{
		if(Aarr[i]>Barr[i])
			return B;
		else if (Aarr[i]<Barr[i])
			return A;
	}
	return A;
}

struct jaratok
{
	jarat j[100];
	int jdb;
};
typedef struct jaratok jaratok;

jaratok GetJaratok()
{
	FILE * f;
	jaratok volt;
	volt.jdb = 0;
	 f=fopen("indulasok.txt","r");
	 if (f==NULL){perror("File opening error\n"); exit(1);}
	 while (!feof(f)){
		 char line[160];
	   fgets(line,sizeof(line),f);
	   jarat jl = sorfelbont(line);
	   
	   int i=0;
		bool voltmar=false;
		
	   while(i<volt.jdb && voltmar==false)
	   {
		   voltmar = StrCompare(volt.j[i].celallomas,jl.celallomas);
		   ++i;
	   }
	   
	   if (voltmar!=false)
	   {
		   --i;
		   volt.j[i].indulas = earliest(volt.j[i].indulas,jl.indulas);
	   }	   
	   else
	   {
		   volt.j[volt.jdb]=jl;
		   ++volt.jdb;
		   
	   } 
	 }
	 
	 return volt;

	 fclose(f);
}


void jaratlista()
{
	jaratok j=GetJaratok();
	
	int i;
	for(i=0; i<j.jdb; ++i)
	{
		szokiir(j.j[i].celallomas);
		printf("\t");
		szokiirnl(DateToStr(j.j[i].indulas));
	}
}


void ujjarat()
{
	jarat j;
	j.celallomas = szobeker( stringToStr("Kerem adja meg a jarat celallomasat: ") );
	j.indulas = StrToDate( szobeker( stringToStr("Kerem adja meg az indulás idejét(year.mm.dd. hh:mm): ") ) );
	
	if( DateValid(j.indulas) != false)
	{
		int i;
		char celallomas[j.celallomas.length];
		for(i=0; i<j.celallomas.length; ++i) {celallomas[i]=j.celallomas.string[i];}
		str indulasstr = DateToStr(j.indulas);
		char indulas[indulasstr.length];
		for(i=0; i<indulasstr.length; ++i) {indulas[i]=indulasstr.string[i];}
		
		FILE * f = fopen("indulasok.txt","a");
		
		fprintf(f,"%s\t%s\n",celallomas,indulas);
		
		fclose(f);
	}
	else
	{
		printf("Nem ertelmezheto datumot adott meg!\n");
	}
}

void jaratTorlNevvel(str torl)
{
	FILE * f = fopen("indulasok.txt","r");
	FILE * temp = fopen("temp.txt","a");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
	bool megvan = false;
	char del[160];
	while (!feof(f) ){
		 char line[160];
	   fgets(line,sizeof(line),f);
	   jarat jl = sorfelbont(line);
	   if(StrCompare(jl.celallomas,torl) != false )
	   {
		   if(megvan==false)
		   {
			   megvan = true;
			   strcpy(del,line);
		   }
		   else
		   {
				date deldate = sorfelbont(del).indulas;
			   if( CompareDate(deldate,jl.indulas) )
			   {
				   fprintf(temp,"%s",del);
			   }
			   else
			   {
				   fprintf(temp,"%s",line);
			   }
		   }
	   }	   
	   else
	   {
			if(!feof(f)) {fprintf(temp,"%s",line);}
	   }
	}
	//printf("%s\n",del);
		
	fclose(f);
	fclose(temp);
	
	remove("indulasok.txt");
	rename("temp.txt","indulasok.txt");
}

void jarattorl()
{
	str torl = szobeker( stringToStr("Kerem adja meg a torlendo jarat uticeljat: ") );
	jaratTorlNevvel(torl);
}


struct jelentkezo
{
	str nev;
	date idopont;
	str celallomas;
	str telefonszam;
};
typedef struct jelentkezo jelentkezo;

void jelentToFile(FILE * stream,jelentkezo j)
{
	int i;
	char nev[j.nev.length];
	for(i=0; i<j.nev.length; ++i) {nev[i]=j.nev.string[i];}
	nev[j.nev.length]=0;
	
	str idopontstr = DateToStr(j.idopont);
	char idopont[idopontstr.length];
	for(i=0; i<idopontstr.length; ++i) {idopont[i]=idopontstr.string[i];}
	idopont[idopontstr.length]=0;
	
	char celallomas[j.celallomas.length];
	for(i=0; i<j.celallomas.length; ++i) {celallomas[i]=j.celallomas.string[i];}
	celallomas[j.celallomas.length]=0;
	
	char telefonszam[j.telefonszam.length];
	for(i=0; i<j.telefonszam.length; ++i) {telefonszam[i]=j.telefonszam.string[i];}
	telefonszam[j.telefonszam.length]=0;
	
	fprintf(stream,"%s\t%s\t%s\t%s\n",nev,idopont,celallomas,telefonszam);
}

void jelentki(jelentkezo j)
{
	szokiir(j.nev);
	printf("\t");
	szokiir( DateToStr(j.idopont) );
	printf("\t");
	szokiir(j.celallomas);
	printf("\t");
	szokiir(j.telefonszam);
	printf("\n");
}

void ujjelentkezo()
{
	jelentkezo j;
	j.nev = szobeker( stringToStr("Kerem adja meg a nevet: ") );
	j.celallomas = szobeker( stringToStr("Kerem adja meg az uticeljat: ") );
	j.telefonszam = szobeker( stringToStr("Kerem adja meg a telefonszamat: ") );
	
	time_t aktido;
	time(&aktido);
	struct tm helyi = * localtime(&aktido);
	j.idopont.year = helyi.tm_year + 1900;
	j.idopont.month = helyi.tm_mon + 1;
	j.idopont.day = helyi.tm_mday;
	j.idopont.h = helyi.tm_hour;
	j.idopont.min = helyi.tm_min;
	
	FILE * f = fopen("jelentkezok.txt","a");
	
	jelentToFile(f,j);
	
	fclose(f);
	
}


jelentkezo jelentsorfelbont(char line[160])
{
	jelentkezo retvalue;
	int i=0;
	while(line[i] != '\t')
	{
		retvalue.nev.string[i] = line[i];
		++i;
	}
	retvalue.nev.length=i;
	++i;
	str datumstr;
	int j=0;
	while(line[i] != '\t')
	{
		datumstr.string[j] = line[i];
		++i;
		++j;
	}
	datumstr.length=j;
	retvalue.idopont = StrToDate(datumstr);
	++i;
	j=0;
	while(line[i] != '\t')
	{
		retvalue.celallomas.string[j] = line[i];
		++i;
		++j;
	}
	retvalue.celallomas.length=j;
	++i;
	j=0;
	while(line[i] != '\n')
	{
		retvalue.telefonszam.string[j] = line[i];
		++i;
		++j;
	}
	retvalue.telefonszam.length=j;

	return retvalue;
}

void JelentkezoModosit()
{
	FILE * f = fopen("jelentkezok.txt","r");
	remove("temp.txt");
	FILE * temp = fopen("temp.txt","a");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
	str onev;
	str new;
	int modositando;
	
	bool jovalasz = false;
	while(jovalasz == false)
	{
		printf("%s\n\t%s\n\t%s\n\t%s\n\t%s"
		,"Kerem adja meg mely adatat kivanja modosítani a jelentkezonek: "
		,"1 - Nev"
		,"2 - Indulasi idopont"
		,"3 - Uticel"
		,"4 - Telefonszam"
		);
		
		jovalasz = true;
		switch (szobeker(stringToStr("")).string[0]-48)
		{
			case 1: printf("1\n"); 
			{
				onev = szobeker( stringToStr("Kerem adja meg a regi nevet!") );
				new = szobeker( stringToStr("Kerem adja meg az uj nevet!") );
				modositando = 1;
			}
			break;
			case 2: printf("2\n");
			{
				onev = szobeker( stringToStr("Kerem adja meg a nevet!") );
				new = szobeker( stringToStr("Kerem adja meg az uj dindulasi idopontot(year.mm.dd. hh:mm)!") );
				modositando = 2;
			}
			break;
			case 3: printf("3\n");
			{
				onev = szobeker( stringToStr("Kerem adja meg a nevet!") );
				new = szobeker( stringToStr("Kerem adja meg az uj celallomast!") );
				modositando = 3;
			}
			break;
			case 4: printf("4\n"); 
			{
				onev = szobeker( stringToStr("Kerem adja meg a nevet!") );
				new = szobeker( stringToStr("Kerem adja meg az uj telefonszamot!") );
				modositando = 4;
			}
			break;
			default : jovalasz = false;
		}
	}
	
	bool megvan = false;
	while(!feof(f))
	{
		char line[160];
		fgets(line,sizeof(line),f);
		jelentkezo jl = jelentsorfelbont(line);
		
		if(!feof(f))
		{
			if(StrCompare(jl.nev,onev) != false && megvan == false)
			{
				switch (modositando)
				{
					case 1: 
					{
						jl.nev=new;
						jelentToFile(temp,jl);
					}
					break;
					case 2:
					{
						if( DateValid(StrToDate(new)) != false)
						{
							jl.idopont=StrToDate(new);
							jelentToFile(temp,jl);
						}
						else
						{
							printf("Hibas datumot adott meg!");
							fprintf(temp,"%s",line);
						}
					}
					break;
					case 3: 
					{
						jl.celallomas=new;
						jelentToFile(temp,jl);
					}
					break;
					case 4:
					{
						jl.telefonszam=new;
						jelentToFile(temp,jl);
					}
					break;
				}
				
				megvan = true;
			}
			else
			{
				fprintf(temp,"%s",line);
			}
		}
		
	}
	
	fclose(f);
	fclose(temp);	
	
	remove("jelentkezok.txt");
	rename("temp.txt","jelentkezok.txt");
}

void jelentkezoTorlNevvel(str torl)
{
	FILE * f = fopen("jelentkezok.txt","r");
	FILE * temp = fopen("temp.txt","a");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
	bool megvan = false;
	while(!feof(f))
	{
		char line[160];
		fgets(line,sizeof(line),f);
		jelentkezo jl = jelentsorfelbont(line);
		
		if( !feof(f) && (StrCompare(jl.nev,torl) == false || megvan !=false ))
		{
			fprintf(temp,"%s",line);
		}
		else
		{
			megvan=true;
		}
		
	}
	
	fclose(f);
	fclose(temp);
	
	remove("jelentkezok.txt");
	rename("temp.txt","jelentkezok.txt");
}

void JelentkezoTorl()
{
	str torl = szobeker( stringToStr("Kerem adja meg a torlendo jelentkezo nevet: ") );
	jelentkezoTorlNevvel(torl);
}


void Adatok()
{
	FILE * f = fopen("jelentkezok.txt","r");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
	jelentkezo jelentkezok[100];
	int jelentkdb = 0;
	while(!feof(f))
	{
		char line[160];
		fgets(line,sizeof(line),f);
		if(!feof(f))
		{
			jelentkezok[jelentkdb] = jelentsorfelbont(line);
			++jelentkdb;
		}
	}
	fclose(f);
	
	jaratok jaratokarr=GetJaratok();
	int i;
	for(i=0; i<jaratokarr.jdb; ++i)
	{
		szokiir(jaratokarr.j[i].celallomas);
		printf("\t");
		szokiirnl(DateToStr(jaratokarr.j[i].indulas));
		
		int j;
		for(j=0; j<jelentkdb; ++j)
		{
			if( StrCompare(jelentkezok[j].celallomas,jaratokarr.j[i].celallomas) != false )
			{
				printf("\t");
				jelentki(jelentkezok[j]);
			}
		}
	}
}


struct jeletkezokstruct
{
	jelentkezo jelentkezok[100];
	int jelentkdb;
};
typedef struct jeletkezokstruct jelentkezokstruct;


jelentkezokstruct JaratInditseged()
{
	jaratlista();
	str juticel = szobeker( stringToStr("Kerem adja meg az inditando jarat uticeljat: ") );
	
	jaratok j = GetJaratok();
	
	bool bennevan = false;
	int k = 0;
	while (k < j.jdb && !bennevan)
	{
		bennevan = StrCompare(j.j[k].celallomas,juticel);
		++k;
	}
	
	if(!bennevan)
	{
		jelentkezokstruct jstruct;
		jstruct.jelentkdb=0;
		return jstruct;
	}
	
	FILE * f = fopen("jelentkezok.txt","r");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	jelentkezokstruct jstruct;
	jstruct.jelentkdb=0;
	
	while(!feof(f))
	{
		char line[160];
		fgets(line,sizeof(line),f);
		if(!feof(f))
		{
			jelentkezo temp = jelentsorfelbont(line);
			if( StrCompare(temp.celallomas,juticel))
			{
				jstruct.jelentkezok[jstruct.jelentkdb] = temp; 
				++jstruct.jelentkdb;
			}
		}
	}
	fclose(f);
	
	jaratTorlNevvel(juticel);
	int i;
	for(i=0; i<jstruct.jelentkdb; ++i)
	{
		jelentkezoTorlNevvel(jstruct.jelentkezok[i].nev);
	}
	
	return jstruct;
}


	
	int befejezheti = 0;
	
	void handler1(){
		befejezheti = 1;
	}
	
	/*
	void handler2(){
		befejezheti = 1;
	}
	*/
	
	struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     int szam; 
	}; 
	typedef struct uzenet uzenet;
	
int jaratindit();
	
int main(int argc, char *argv[])
 {
	 kulcs = ftok(argv[0],1); 
	 
	char valasz;
	do
	{
		MenuKiir();
		valasz=getchar();
		char c;
		do 
		{
			c = getchar();
		} while (c != '\n');
		switch(valasz)
		{
			case '1': jaratlista(); break;
			case '2': ujjarat(); break;
			case '3': jarattorl(); break;
			case '4': ujjelentkezo(); break;
			case '5': JelentkezoModosit(); break;
			case '6': JelentkezoTorl(); break;
			case '7': Adatok(); break;
			case '8': jaratindit(); break;
		}
		
	}while(valasz!='0');
	return 0;
}      
		
		
		
int jaratindit()
{
	{
		   
		   
           int pipefd[2]; // unnamed pipe file descriptor array
           pid_t pid;
           char sz[100];  // char array for reading from pipe

           if (pipe(pipefd) == -1) 
			{
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
           pid = fork();	// creating parent-child processes
           if (pid == -1) 
			{
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }
		   
		   signal(SIGUSR1, handler1);
		   
		   struct ptomb
		   {
			   int size;
			   int arr[100];
		   };
		   typedef struct ptomb ptomb;
		   
		   pid_t child; 
			 int uzenetsor, status; 
			 key_t kulcs; 
			 
			 // msgget needs a key, amelyet az ftok generál 
			 //. 
			 //kulcs = ftok(argv[0],1); 
			 //printf ("A kulcs: %d\n",kulcs);
			 uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
			 if ( uzenetsor < 0 ) { 
				  perror("msgget"); 
				  return 1; 
			 } 

           if (pid == 0) 
			{		    	// child process
			   
					close(pipefd[1]);  //Usually we close the unused write end
					jelentkezokstruct jstruct;
			   
					read(pipefd[0],&jstruct,sizeof(jstruct)); // reading max 100 chars
					if(jstruct.jelentkdb==0) {printf("Hiba\n"); exit(EXIT_SUCCESS);}
					printf("Az idegenvezeto megkapta az utasokat: \n");
				   
				   
					   int i;
					   for(i=0; i<jstruct.jelentkdb; ++i)
					   {
						   szokiir(jstruct.jelentkezok[i].nev);
						   printf("\t");
						   szokiirnl(jstruct.jelentkezok[i].telefonszam);
					   }
					   close(pipefd[0]);  // finally we close the used read end
						
						srand ( time(NULL) );
						int ertekeles = ((rand() % 5) + 1);
					   
					   int status; 
					   uzenet uz={5,ertekeles};
		 
						status = msgsnd( uzenetsor, &uz, sizeof(uz) , 0 ); 
						// a 3. param ilyen is lehet: sizeof(uz.mtext)
						// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
						if ( status < 0 ) {perror("msgsnd"); }
						printf("Idegenvezeto elkuldte az ertekelest!\n");
					   
					   
					   
					   while (befejezheti == 0)
					   {
							;
					   }
					   sleep(1);
					   printf("Idegenvezeto befejezte az utat!\n");
				   exit(EXIT_SUCCESS);
				   
				   
			} 
			else 
			{    
						//parent
			
							jelentkezokstruct jstruct;
							jstruct = JaratInditseged();
							if(jstruct.jelentkdb == 0) {printf("Nincs ilyen uticel az adatbazisban\n");}
								/////////////// 
								close(pipefd[0]); //Usually we close unused read end
								write(pipefd[1], &jstruct,sizeof(jstruct));
								close(pipefd[1]); // Closing write descriptor 
								
							if(jstruct.jelentkdb != 0)
							{
								printf("A tarsasag atadta az utaslistat az idegenvezetonek!\n");
								fflush(NULL); 	// flushes all write buffers (not necessary)
								close(pipefd[1]);
								
								
								uzenet uz; 
								int status; 
								// az utolso parameter(0) az uzenet azonositoszama
								// ha az 0, akkor a sor elso uzenetet vesszuk ki
								// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
								// vesszuk ki a sorbol 
								status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
								 
								if ( status < 0 ) 
									perror("msgsnd"); 
								else
									printf("Ertekeles: %d\n",uz.szam);
								
								sleep(2);
								printf("Tarsasag befejezo jelet kuldott a gyereknek!\n");
								kill(0, SIGUSR1);
					
								wait();
								printf("Tarsasag befejezte az utat befejezte!\n");
							}

		   }
		   //exit(EXIT_SUCCESS);	// force exit, not necessary
		}
}

 
