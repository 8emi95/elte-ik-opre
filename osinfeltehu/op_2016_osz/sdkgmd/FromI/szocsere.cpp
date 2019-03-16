/*  A  */                       /*  Ez JO  */
/* 1994. 10. 19. Csutortok                                                 */
/* Egy adott szovegben egy szonak a csereje */

#include<iostream.h>;
#include<conio.h>;

  char ch[]="Alma a fa alatt.";
  int s;  /* Karakter szamlalas  */
  int i;
  char c[]="a";

/***************************************************************************/
/*  Szoveg hosszusag kereses. */
int hossz(char *s)
    { int i = 0;
      while(s[i]) i++;
      return i;
    }

/*************************************************************************/
/* Karakter elofordulas egy adoptt szovegben */

int hanyszor(char *miben, char* mi)
 { int i,j,k=hossz(miben),l=hossz(mi),db=0;
  for (i=0;i<k;i++)
  {
   for (j=0; (miben[i]==mi[j]) && (j<l); j++) i++;
   if (j==l) db++;
   if (j) i--;
   }
  return db;
 }

/***************************************************************************/
 char* mintacsere(char *miben, char* mit, char*mire)
 { int i,j,k=hossz(miben),l=hossz(mit),uji=0;
   int n=hanyszor(miben,mit);
   int h=k-n*l+n*hossz(mire)+1;
   char* uj=new char[h];
  for (i=0;i<k;i++)
  {
   for (j=0; (miben[i]==mit[j]) && (j<l); j++) i++;
   if (j==l)
     {
       for(int m=0;m<hossz(mire);m++)
	 {
	   uj[uji]=mire[m]; uji++;
	 }
       i--;
     }
   else
     {
       i-=j; uj[uji]=miben[i]; uji++;
     }
  }
  uj[uji]=0;
  return uj;
 }


 main()
{
  clrscr();
    cout<< "Egy szoveg kiirasa\n";
    cout<<"\n";
    cout<<"A szoveg: ";
/*    cin>>ch; */
      cout<<ch;

    cout<<"\n";
    cout<<"Az uj szoveg: ";
    cout << mintacsere(ch,"fa","gyoker");
  getch();
/*  clrscr();*/
}
