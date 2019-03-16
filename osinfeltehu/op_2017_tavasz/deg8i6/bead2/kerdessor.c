#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#include "common.h"
#include "kerdessor.h"

void create_kerdessor(){
    struct Kerdes kerdes[MAX_KERDES_COUNT];
    char name[MAX_KERDES_NAME_LENGTH], date[MAX_DATETIME_LENGTH],
         question[MAX_KERDES_LENGTH], answer[MAX_VALASZ_LENGTH],
         filename[30], kimenet[101];
    int question_count, file;
    
    DIR *mappa = opendir("kerdoivek");
    if( mappa ){
        closedir(mappa);
    }else if( ENOENT == errno ){
        system("mkdir kerdoivek");
    }else{
        perror("ERROR: opendir() has some problem");
    }
    
    do{
        printf("Add meg a kerdessor nevet: ");
        readLine(name);
        strcpy(filename,"kerdoivek/");
        strcat(filename,name);
    }while( checkFileExistence(filename) == true && printf("Mar van ilyen kerdessor!\n"));
    
    file = open(filename,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    sprintf(kimenet,"%s\n",getTime(date));
    write(file,kimenet,strlen(kimenet));
    
    do{
        printf("Mennyi kerdes fog szerepelni a kerdoivben? ");
        scanf("%d",&question_count);
    }while( (question_count <= 0 || question_count > MAX_KERDES_COUNT) && 
            printf("Minimum 1 es maximum 20 kerdes szerepelhet a kerdoivben!\n"));
    
    int i;
    for(i=0;i<question_count;i++){
        printf("%d. kerdes:\n",i+1);
        readLine(kerdes[i].kerdes);
        sprintf(kimenet,"%s\n",kerdes[i].kerdes);
        write(file,kimenet,strlen(kimenet));
        
        int response_count;
        do{
            printf("Mennyi valasz fog szerepelni a kerdoivben? ");
            scanf("%d",&response_count);
        }while( (response_count < MIN_VALASZ_COUNT || response_count > MAX_VALASZ_COUNT) && 
                printf("Minimum 2 es maximum 4 valasz szerepelhet!\n") );
    
        int j;
        for(j=0;j<response_count;j++){
            printf("Add meg a %d. valaszt: ",j+1);
            readLine(kerdes[i].valasz[j]);
            sprintf(kimenet,"%s\n",kerdes[i].valasz[j]);
            write(file,kimenet,strlen(kimenet));
        }
        sprintf(kimenet,"\n");
        write(file,kimenet,strlen(kimenet));
        
    }
    close(file);
}

//////////////////////////////////////////////////

int edit_kerdes(char *kerdes, char valasz[][MAX_VALASZ_LENGTH]){
    printf("Add meg a kerdest:\n");
    readLine(kerdes);
    strcat(kerdes,"\n");

    int response_count;
    do{
        printf("Mennyi valasz fog szerepelni a kerdoivben? ");
        scanf("%d",&response_count);
    }while( (response_count < MIN_VALASZ_COUNT || response_count > MAX_VALASZ_COUNT) && 
            printf("Minimum 2 es maximum 4 valasz szerepelhet!\n") );

    int i;
    for(i=0;i<response_count;i++){
        printf("Add meg a %d. valaszt: ",i+1);
        readLine(valasz[i]);
        strcat(valasz[i],"\n");
    }
    
    return response_count;
}

void edit_kerdessor(){
    DIR *mappa = opendir("kerdoivek");
    if( mappa ){
        closedir(mappa);
    }else if( ENOENT == errno ){
        printf("Nem talalhatoak kerdoivek!\n");
        return;
    }else{
        perror("ERROR: opendir() has some problem");
    }
    
    char sor[100],kimenet[101],filename[30],buf[30],
            kerdes[MAX_KERDES_LENGTH],valasz[MAX_VALASZ_COUNT][MAX_VALASZ_LENGTH];
    
    printf("Add meg a modositando kerdoiv nevet: ");
    scanf("%s",buf);
    sprintf(filename,"kerdoivek/%s",buf);
    
    if( checkFileExistence(filename) == false ){
        printf("Nem letezik ilyen kerdoiv!\n");
        return;
    }
    
    FILE *file = fopen(filename,"r");
    int file2 = open("temp",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    
    if( file == NULL ){ perror("ERROR: Could not open file for read"); return; }
    
    fgets(sor,sizeof(sor),file);
    sprintf(kimenet,"%s",sor);
    write(file2,kimenet,strlen(kimenet));
    
    while( fgets(sor,sizeof(sor),file) ){
        if( sor[0] == '\n' ) continue;
        
        strcpy(kerdes,sor);
        printf("%s",sor);
        int responses = 0;
        while( fgets(sor,sizeof(sor),file) ){
            if( sor[0] == '\n' ) break;
            printf("%d: ",responses+1);
            strcpy(valasz[responses++],sor);
            printf("%s",sor);
        }
        
        printf("\nTorlesre keruljon-e?(y/n): ");
        scanf("%1s",buf);
        if( buf[0] == 'y' ) continue;
        printf("Modositasra keruljon-e?(y/n): ");
        scanf("%1s",buf);
        
        if( buf[0] == 'y' ) responses = edit_kerdes(kerdes,valasz);
        
        sprintf(kimenet,"%s",kerdes);
        write(file2,kimenet,strlen(kimenet));
        
        int i;
        for(i=0;i<responses;i++){
            sprintf(kimenet,"%s",valasz[i]);
            write(file2,kimenet,strlen(kimenet));
        }
        write(file2,"\n",1);
    }
    
    fclose(file);
    close(file2);
    
    sprintf(kimenet,"mv temp %s",filename);
    system(kimenet);
}
////////////////////////////////////////////////////////////////////////////////

void listaz_kerdessor(){
    char filename[30], buf[100],
         kerdes[MAX_KERDES_LENGTH], valasz[MAX_VALASZ_COUNT][MAX_VALASZ_LENGTH];
    
    printf("Add meg a listazando kerdoiv nevet: ");
    scanf("%s",buf);
    sprintf(filename,"kerdoivek/%s",buf);
    
    if( checkFileExistence(filename) == false ){
        printf("Nem letezik ilyen kerdoiv!\n");
        return;
    }
    
    FILE *file = fopen(filename,"r");
    
    if( file == NULL ){ perror("ERROR: Could not open file for read"); return; }
    
    fgets(buf,sizeof(buf),file);
    printf("%s",buf);
    
    while( fgets(buf,sizeof(buf),file) ){
        if( buf[0] == '\n' ) continue;
        
        printf("%s",buf);
        int responses = 0;
        while( fgets(buf,sizeof(buf),file) ){
            if( buf[0] == '\n' ) break;
            strcpy(valasz[responses++],buf);
        }
        
        printf("Valaszok megjelenitese?(y/n): ");
        scanf("%1s",buf);
        if( buf[0] != 'y' ) continue;
        
        int i;
        for(i=0;i<responses;i++){
            printf("%d: %s",i+1,valasz[i]);
        }
        printf("\n");
    }
    
    fclose(file);
}
////////////////////////////////////////////////////////////////////////////////

void veglegesit_kerdessor(){
    DIR *mappa = opendir("veglegesitett");
    if( mappa ){
        closedir(mappa);
    }else if( ENOENT == errno ){
        system("mkdir veglegesitett");
    }else{
        perror("ERROR: opendir() has some problem");
    }
    
    char buf[30], kimenet[100];
    printf("Add meg a veglegesiteni kivant kerdessor nevet: ");
    scanf("%s",buf);
    sprintf(kimenet,"kerdoivek/%s",buf);
    
    if( checkFileExistence(kimenet) == false ){
        printf("Nem letezik ilyen kerdoiv!\n");
        return;
    }
    
    sprintf(kimenet,"mv kerdoivek/%s veglegesitett/%s",buf,buf);
    system(kimenet);
}
