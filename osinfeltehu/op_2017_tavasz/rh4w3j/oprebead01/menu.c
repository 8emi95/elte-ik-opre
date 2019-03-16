#include "menu.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

void run(struct menu m, struct data infos[] )
{
    m.tombszamolo=0;
    while(m.chose!=0)
    {
        printf("File megnyitasa es adatok betoltese (1)!\n");
        printf("Eddigi kerdesek lekerdezese (2)!\n");
        printf("Uj kerdes & valasz(ok) hozzaadasa (3)!\n");
        printf("Kerdes modositas eseten (4)!\n");
        printf("Kerdes/Valasz torlese (5)\n");
        printf("Vegleges kerdessor osszeszerkesztese (6)\n");
        printf("Menubol valo kilepes (0)!\n");
        printf("Eddigi kerdesek szama: %i\n\n", m.tombszamolo);
        scanf("%d", &m.chose);



    switch ( m.chose ) {
        case 0:
  // Code
  break;
        case 1:
    m.tombszamolo=dataloading(infos, m.tombszamolo);
  break;
        case 2:
    buffervivion(infos, m.tombszamolo);
  break;
        case 3:
    m.tombszamolo=m.tombszamolo+1;
    new_kerdes(infos,m.tombszamolo);
  break;
        case 4:
        kerdes_modosit(infos, m.tombszamolo);
  break;
        case 5:
        m.tombszamolo=torol(infos,m.tombszamolo);

  break;
          case 6:
       veglegesites(infos,m.tombszamolo);
  break;
        default:
             printf("Nincs ilyen valasztasi heletoseg \n");
        break;

    }

    }

    be_file=fopen("questlist.txt","w+");
    int i;
    for(i=1; i<=m.tombszamolo;i++)
    {
       fwrite(&infos[i].question, strlen(infos[i].question), 1, be_file);
        fwrite(&infos[i].ans1, strlen(infos[i].ans1), 1, be_file);
          fwrite(&infos[i].ans2, strlen(infos[i].ans2), 1, be_file);
            fwrite(&infos[i].ans3, strlen(infos[i].ans3), 1, be_file);
              fwrite(&infos[i].ans4, strlen(infos[i].ans4), 1, be_file);
              fwrite(&infos[i].ido, strlen(infos[i].ido), 1, be_file);
    }
    fclose(be_file);
}

void veglegesites(struct data infos[],int teto)
{
    buffervivion(infos,teto);
    printf("Adja meg melyik kerdeseket szeretne a vegos kerdoivben. (minden kivalszott kerdes utan 'enter'-t kell nyomni \n");
    printf("0-val ki tud lepni es befejezte a vegleges kerdoivet!\n");
    fseek(stdin,0,SEEK_END);
    int i;
    ki_file=fopen("vegso.txt","w+");
    scanf("%i",&i);
    while(i!=0)
    {
        if(i>0 && i<teto+1)
        {
        fwrite(&infos[i].question, strlen(infos[i].question), 1, be_file);
        fwrite(&infos[i].ans1, strlen(infos[i].ans1), 1, be_file);
          fwrite(&infos[i].ans2, strlen(infos[i].ans2), 1, be_file);
            fwrite(&infos[i].ans3, strlen(infos[i].ans3), 1, be_file);
              fwrite(&infos[i].ans4, strlen(infos[i].ans4), 1, be_file);
              fwrite(&infos[i].ido, strlen(infos[i].ido),1,be_file);

            fseek(stdin,0,SEEK_END);
            printf("Adja meg a kovetkezo szamo: \n");
            scanf("%i",&i);
        }
        else
        {
            printf("Nincs ilyen kerdes! \n");
            fseek(stdin,0,SEEK_END);
            scanf("%i",&i);
        }
    }
    fclose(ki_file);

}


int torol(struct data dat[], int teto)
{
    printf("Ezekbol torolhet : \n");
    buffervivion(dat,teto);
    printf("Melyikbol szeretne torolni?\n");
          int chose;
          fseek(stdin,0, SEEK_END);
          scanf("%i",&chose);
     printf("Kerdest vagy valaszt szeretne torolni ?\n");
     printf("Kerdes eseten a valaszok is torlodnek! \n");
    printf("Ha kerdes nyomjon egy 'q'-betut kulonben valaszt torolhet!\n");
          fseek(stdin,0,SEEK_END);
          char c;
          c=getchar();

          if(c=='q')
          {
                memset(dat[chose].question,0,145);
                 memset(dat[chose].ans1,0,144);
                  memset(dat[chose].ans2,0,144);
                   memset(dat[chose].ans3,0,144);
                    memset(dat[chose].ans4,0,144);
                    memset(dat[chose].ido,0,21);
                strcpy(dat[chose].question, dat[teto].question);
                strcpy(dat[chose].ans1, dat[teto].ans1);
                strcpy(dat[chose].ans2, dat[teto].ans2);
                strcpy(dat[chose].ans3, dat[teto].ans3);
                strcpy(dat[chose].ans4, dat[teto].ans4);
                strcpy(dat[chose].ido, dat[teto].ido);
                memset(dat[teto].question,0,145);
                 memset(dat[teto].ans1,0,144);
                  memset(dat[teto].ans2,0,144);
                   memset(dat[teto].ans3,0,144);
                    memset(dat[teto].ans4,0,144);
                    memset(dat[teto].ido,0,21);
                    teto=teto-1;
          }
         else
        {
            printf("Melyik valaszt szeretne torolni? (1,2,3,4) \n");
             fseek(stdin,0,SEEK_END);
             int szam;
             scanf("%d", &szam);

             while(1)
             {
                 if(dat[chose].ans2==0)
                 {
                     printf("Innen nem torolhet, mert nincsen 2 valasz ! \n");
                     break;
                 }else
                 if(szam==1)
                 {
                     memset(dat[chose].ans1, 0,144);
                     strcpy(dat[chose].ans1, dat[chose].ans2);
                     strcpy(dat[chose].ans2, dat[chose].ans3);
                     strcpy(dat[chose].ans3, dat[chose].ans4);
                     memset(dat[chose].ans4,0,144);


                 }else if
                 (szam==2)
                 {
                     memset(dat[chose].ans2, 0,144);
                     strcpy(dat[chose].ans2, dat[chose].ans3);
                     strcpy(dat[chose].ans3, dat[chose].ans4);
                     memset(dat[chose].ans4,0,144);
                 }
                 else if (szam==3)
                 {
                     memset(dat[chose].ans3, 0,144);
                     strcpy(dat[chose].ans3, dat[chose].ans4);
                     memset(dat[chose].ans4,0,144);
                 }
                else if (szam==4)
                 {
                     memset(dat[chose].ans4,0,144);
                 }
             }

        }
    return teto;
}



void kerdes_modosit(struct data dat[], int fullos)
{
          fseek(stdin,0,SEEK_END);
          printf("Eddigi kerdesek es valaszok : \n");
          buffervivion(dat, fullos);
          printf("Melyiket szeretne valtoztatni?\n");
          int chose;
          scanf("%i",&chose);
          printf("Kerdest vagy valaszt szeretne valtoztatni ?\n");
          printf("Ha kerdes nyomjon egy 'q'-betut kulonben valaszt valtoztat!\n");
          fseek(stdin,0,SEEK_END);
          char c;
          c=getchar();
          if(c=='q')
          {
             printf("Ez a volt a kerdes: \n%s\n", dat[chose].question);
             fseek(stdin,0,SEEK_END);
             char uj[145];
             printf("Mire akarja megvaltoztatni ? : \n");
             gets(uj);
             strcat(uj,"\n");
             while(kerdes_e(uj)==0)
             {
                 printf("Kerek egy uj kerdest!\n");
                memset(uj, 0, 145);
                gets(uj);
                strcat(uj, "\n");
             }
             memset(dat[chose].question,0,145);
             strcpy(dat[chose].question, uj);
             memset(dat[chose].ido,0,21);
            time_t ido=time(NULL);
            char buffer[21];
            strftime(buffer,21, "%Y-%m-%d %H:%M:%S\n", localtime(&ido));
             strcpy(dat[chose].ido, buffer);
             printf("A kerdes modositottuk\n!");

          }
          else
          {
               fseek(stdin,0,SEEK_END);

              printf("Csak teljes valaszsort lehet modositani! \n");
              printf("Ezek voltak a valaszok: \n%s \n%s \n%s \n%s \n",dat[chose].ans1,dat[chose].ans2,dat[chose].ans3,dat[chose].ans4);
             char valasz[144];
    gets(valasz);
    strcat(valasz, "\n");

          memset(dat[chose].ans1,0,144);
          memset(dat[chose].ans2,0,144);
          memset(dat[chose].ans3,0,144);
          memset(dat[chose].ans4,0,144);

        while (strlen(dat[chose].ans4)==0)
                {
            if(kerdes_1(valasz, dat[chose])==1)
            {
                strcpy(dat[chose].ans1,valasz);
            }
            else
            if(kerdes_1(valasz, dat[chose])==2)
            {
                strcpy(dat[chose].ans2,valasz);
            }
            else
            if(kerdes_1(valasz, dat[chose])==3)
            {
                strcpy(dat[chose].ans3,valasz);
            }
            else
            if(kerdes_1(valasz, dat[chose])==4)
            {
                strcpy(dat[chose].ans4,valasz);
                break;
            }
            printf("Kerem a kovetkezo valaszt! \n");
            memset(valasz,0,144);
            gets(valasz);
            strcat(valasz, "\n");
            }
          }


}







void new_kerdes(struct data dat[], int index)
{
    printf("\n");
    printf("A Kerdes maximum 144 karakter lehet & vegen kerdojellel zarul!\n");
    char question[145];
    fseek(stdin,0,SEEK_END);
    gets(question);
    strcat(question, "\n");
    while(kerdes_e(question)==0)
    {


        if(strcmp(question, "")!=0)
        {
             printf("Ez nem egy kerdes vagy nem felel meg a kriteriumoknak\n");
            memset(question, 0, 145);
        }
                gets(question);
                strcat(question, "\n");
    }
    strcpy(dat[index].question,question);
    time_t ido=time(NULL);
    char buffer[21];
    strftime(buffer,21, "%Y-%m-%d %H:%M:%S\n", localtime(&ido));
    strcpy(dat[index].ido,buffer);

    printf("Most a valaszadasok kovetkeznek!\n Legalabb 2, de maximum 4 lehet!\n Maximum 143 karakterbol allhat!\n");
    fseek(stdin,0,SEEK_END);
    printf("Valasz megadasa utan felteve, hogy 2-ot megadott ures valasszal ki lehet lepni!\n");
    char valasz[144];
    gets(valasz);
    strcat(valasz, "\n");       //ha megtelt 0                                /ha megtelt 0
        //(  (strlen(dat[index].ans2)==0  &&    ((strlen(dat[index].ans4)==0)  || !(strcmp(valasz,"\n")))   ))
    while (strlen(dat[index].ans4)==0)
    {
            if(kerdes_1(valasz, dat[index])==1)
            {
                strcpy(dat[index].ans1,valasz);
            }
            else
            if(kerdes_1(valasz, dat[index])==2)
            {
                strcpy(dat[index].ans2,valasz);
            }
            else
            if(kerdes_1(valasz, dat[index])==3)
            {
                strcpy(dat[index].ans3,valasz);
            }
            else
            if(kerdes_1(valasz, dat[index])==4)
            {
                strcpy(dat[index].ans4,valasz);
                break;
            }
            printf("Kerem a kovetkezo valaszt! \n");
            memset(valasz,0,144);
            gets(valasz);
            strcat(valasz, "\n");

    }

}



int valasz_e(char valasz[])
{
    if (strlen(valasz)<144)
    return 1;
    return 0;
}


int dataloading(struct data dat[], int tombszamolo)
{

    be_file=fopen("questlist.txt","a+");
    char line[145];
     while (fgets(line, sizeof(line), be_file)) {
            if(kerdes_e(line))
            {
                tombszamolo=tombszamolo+1;
               strcpy(dat[tombszamolo].question,line);
            }
            else
            {
                if(kerdes_1(line,dat[tombszamolo])==1)
                {
                    strcpy(dat[tombszamolo].ans1,line);
                }
                else if(kerdes_1(line,dat[tombszamolo])==2)
                {
                    strcpy(dat[tombszamolo].ans2,line);
                }
                else if(kerdes_1(line,dat[tombszamolo])==3)
                {
                    strcpy(dat[tombszamolo].ans3,line);
                }
                else if(kerdes_1(line,dat[tombszamolo])==4)
                {
                    strcpy(dat[tombszamolo].ans4,line);
                }else
                {
                        time_t ido=time(NULL);
                        char buffer[21];
                        strftime(buffer,21, "%Y-%m-%d %H:%M:%S\n", localtime(&ido));
                        strcpy(dat[tombszamolo].ido,buffer);
                }

            }
     printf("%s", line);
    memset(line, 0, 145);

    }
     printf("Fin!  %i \n", tombszamolo);
     fclose(be_file);
     return tombszamolo;


}



bool kerdes_e(char kerdes[])
{
        int i;
       for( i=0; i<145; i++)
       {
            if(kerdes[i]=='\n')
            {
                if(kerdes[i-1]=='?')
                {
                       return 1;
                }

            }

       }
        return 0;
}


int kerdes_1(char kerdes[], struct data akt)
{
    int ans;
        if(strlen(akt.ans1)==0)
        {
            ans=1; return ans;
        }
        else
        if(strlen(akt.ans2)==0)
        {
            ans=2; return ans;
        }
        else
        if(strlen(akt.ans3)==0)
        {
            ans=3; return ans;
        }
        else
        if(strlen(akt.ans4)==0)
        {
            ans=4; return ans;
        }
        return -1;

}


void buffervivion(struct data dat2[], int tombszamolo)
{
    int i;
    printf("Az Eddigi kerdesbazis: \n\n");
    for(i=1; i<=tombszamolo; i++)
    {
        printf("------------------------------------------\n");
        printf("Ez a(z) %i-edik kerdes: \n", i);
        printf("%s",dat2[i].question);
        printf("%s",dat2[i].ans1);
        printf("%s",dat2[i].ans2);
        printf("%s",dat2[i].ans3);
        printf("%s",dat2[i].ans4);
        printf("%s",dat2[i].ido);
        printf("\n");
        printf("------------------------------------------\n");
    }
     printf("\nAz Eddigi kerdesbazis vege! \n");
      printf("\n");
}

