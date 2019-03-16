#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
void main()
{
    FILE *outp,*inpt;
    char another,choice;
    struct emp
    {
        int emp_no,age;
        char name[40];
        float bs;
    };
    struct emp e;
    char empname[40];
    long int recsize;
    outp=fopen("emp.dat","r+");
    if(outp=='\0')
    {
        outp=fopen("emp.dat","w+");
        if(outp=='\0')
        {
            puts("cannot open file\n");
            exit(1);
        }
    }
    recsize=sizeof(e);
    while(1)
    {
        printf("1.Add records\n");
        printf("2.List records\n");
        printf("3.Modify records\n");
        printf("4.Delete records\n");
        printf("0. exit\n");
        printf("Your choice\n");
        fflush(stdin);
        choice=getche();
        switch(choice)
        {
            case '1':                           //code to add data
            .

            case '2':                        //code to display data

            case '3':                       //code to modify data
            another='Y';
            while(another=='Y')
            {
                printf("\nEnter name of employee to modify");
                scanf("%s",empname);
                rewind(outp);
                while(fread(&e,recsize,1,outp)==1)
                {
                    if(strcmp(e.name,empname)==0)
                    {
                        printf("\nenter new name,age & gs");
                        scanf("%d %s %d %f",&e.emp_no,&e.name,&e.age,&e.bs);
                        fseek(outp,-recsize,SEEK_CUR);
                        fwrite(&e,recsize,1,outp);
                        break;
                    }
                }
                printf("\nModify another record(Y/N)");
                fflush(stdin);
                another=getche();
            }
                        printf("\n\n");
            break;
            case '4':                         //code to delete data

            case '0':
            fclose(outp);
            printf("\n\n");
            exit(1);
        }
    }
}