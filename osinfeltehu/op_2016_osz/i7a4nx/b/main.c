//Operacios rendszerek beadando -- King of stands rendezveny nyilvantartas
//Keszitette Kakuk Norbert -- i7a4nx -- kano1337@gmail.com

//a programban 3 binaris fajlt kezelek: 
//- rendezvenyek                              (EVENTS struct -- eventsData filenev)    
//- vendegek adatait nyilvantartoakat         (GUESTS struct -- visitorsData filenev)        
//es e ketto kozott kapcsolatot letesito tabla szeruen a
//- regisztraciok adatait nyilvantartot       (REGISTRATIONS struct -- regData filenev)    


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h> 
#include <time.h>

//----------------------Felhasznaloi tipusok

typedef struct {
   char  	guestName[40];
   char  	email[50];
   int          guestID;
} GUESTS; 

typedef struct {
   char	eventName[40];
   int		eventID;
} EVENTS; 

typedef struct {
    int guestID;
    int eventID;
    int order;
    time_t registrationTime;
} REGISTRATIONS;


//----------------------Segedfuggvenyek

//mivel több adatfileom is van, ebben a fv ben a nevük által adott bemenő
//paraméter alapján kérdezem le a (struct adategységben mért) hosszukat
int fajlhossz(char* filenev){

	REGISTRATIONS reg;
        GUESTS guest;
        EVENTS event;
        
	FILE *f;
	int hossz=0;
	
	f=fopen( filenev ,"rb" );
	if (f==NULL) {
            perror("\n ! Hiba a fajl megnyitasanal ! \n");
            exit(1); 
        }
	fseek(f,0,SEEK_SET);

        if( strcmp(filenev, "regData") == 0 ) {
        	while (!feof(f)){
                    fread(&reg,sizeof(reg),1,f);
                    hossz++;
                }
	}else if ( strcmp(filenev, "eventsData") == 0 ) {
                while (!feof(f)){
                    fread(&event,sizeof(event),1,f);
                    hossz++;
                }          
	}else if ( strcmp(filenev, "visitorsData") == 0 ) {
                while (!feof(f)){
                    fread(&guest,sizeof(guest),1,f);
                    hossz++;
                }          
        }

return hossz;
}


//lekérdezi és visszaadja a rendszeridőt
time_t getTime()
{
    time_t current_time;
    char* c_time_string;

    current_time = time(NULL);
return current_time;
}

//char* -á konvetrálja és visszaadja a rendszeridőt
char* ctimeToStr(time_t time){
    char* c_time_string;

    c_time_string = ctime(&time);
return c_time_string;
}

//egy másik lehetőség a fájlhossz mérésére
int getFileSize(FILE* fptr){
    int filesize;
    
    fseek(fptr, 0L, SEEK_END);
    filesize = ftell(fptr);
return filesize;
}

//az újsor  és a carriage return karakterek eltávolítása az inputból
//(0 termináló karekterre cserélem)
void removeNewLine(char* inpstr){
    //char *pos;
    //if ((pos=strchr(inpstr, '\n')) != NULL)
    //*pos = '\0';
    
    //vagy:
    inpstr[strcspn( inpstr, "\r\n" )] = 0;
}


//a zéró termináló karakterekkel végigtöltött dest célkarakterláncba
//másolja az src forrás karakterláncot
void strncpyBuffSafe( char* dest, char* src ){
   memset(dest, '\0', sizeof(dest));
  
   strncpy( dest, src, sizeof(dest)-1 );
}


//email cim formai ellenorzese (egyszeru probalkozas)
int validateEmailFormat( char* input, int len ){
    int err = 0;
    
    int i;
    int last_dot_pos = 0;
    int ats_count = 0;
        
    int ats_pos[len];
    int dots_pos[len];
    int hyphen_pos[len];
    
    for(i = 0; i<len; i++){
        if( input[i] == '@' ) {
            ats_pos[i] = 1;
            ats_count++;
        }else{
            ats_pos[i] = 0;
        }
    }
    
    for(i = 0; i<len; i++){
        if( input[i] == '.' ) {
            dots_pos[i] = 1;
        }else{
            dots_pos[i] = 0;
        }
    }
    
    for(i = 0; i<len; i++){
        if( input[i] == '-' ) {
            hyphen_pos[i] = 1;
        }else{
            hyphen_pos[i] = 0;
        }
    }
    
    if ( ats_count != 1 ) { err = -1; }
    
    if ( ats_pos[0] == 1 ) { err = -1; }
    if ( dots_pos[0] == 1 ) { err = -1; }
    if ( hyphen_pos[0] == 1 ) { err = -1; }
    
    //nem allhat ket . vagy ket - egymas utan
    for(i = 1; i < len; i++){
        if ( dots_pos[i] == 1 && dots_pos[i-1] == 1 ) { err = -1; }
        if ( hyphen_pos[i] == 1 && hyphen_pos[i-1] == 1 ) { err = -1; }
        
        if ( dots_pos[i] == 1 ) { last_dot_pos = i; }
    }
    //@ elott es utan nem all pont
    for(i = 1; i<len-4; i++) {
        if ( ( ats_pos[i] == 1 && dots_pos[i-1] == 1 ) || ( ats_pos[i] == 1 && dots_pos[i+1] == 1 ) ) {
            err = -1;
        }
    }
    //utolso pont utan 2-3 karakter kell legyen meg
    if( last_dot_pos < len-4 || last_dot_pos > len-3 ) { err = -1; }
    
return err;
}


//----------------------ID - nev konverzios fv ek

    
//a bemenő pareméter karekterlánc a vendég neve
//visszaadja a vendég ID jét (int)
//ha nem talalta a vendeget -1 et ad vissza hibakezeleshez
//csak addig olvassa a visitorsData fájl, amíg azt meg nem találja
    int getGuestID(char* guestName){
       int guestID;
       FILE* ptr_visitorsFile;
       GUESTS guest; 
       
       ptr_visitorsFile = fopen( "visitorsData" , "rb" );
       
       if (ptr_visitorsFile==NULL){
           perror("! visitorsFile nyitási hiba !\n"); 
           exit(1);
       } else {
           //printf("\nvisitorsData opened\n");
       } 
       
       while( fread( &guest, sizeof(guest), 1, ptr_visitorsFile ) ){           
           // printf("\n %s \n", guest.guestName);
            if ( strcmp(guest.guestName, guestName) == 0){
                break;
            }
       }
       if ( strcmp(guest.guestName, guestName) == 0){ guestID = guest.guestID; }
       else { 
           guestID = -1; 
           printf("\n HIBA ! Vendeg nem talalhato !\n\n"); 
       }
       
       fclose(ptr_visitorsFile);
       //printf("gID %d returned, VD closed", guestID);
    return guestID;
    }
    
    
  //a vendég ID je alapján a visitorsData fájlt olvasva 
 // vissszaadja az emailcímet hordozó karakterláncot     
    void getEmail(int guestID, char* storedEmail){
    
       FILE* ptr_visitorsFile;
       GUESTS guest;    
       ptr_visitorsFile = fopen( "visitorsData" , "rb" );
       
       if (ptr_visitorsFile==NULL){
           perror("! visitorsFile nyitási hiba !\n"); 
           exit(1);
       } else {
           //printf("\nvisitorsData opened, looking for EML of %d ID\n",guestID);
       } 
      
        while( fread( &guest, sizeof(guest), 1, ptr_visitorsFile ) ){           
            //printf("\n %d \n", guest.guestID );
            if ( guest.guestID == guestID ) {
                break;
            }
       }
       if ( guest.guestID == guestID ){ 
           //printf(" found StoredEML %s ", guest.email ); 
           //strncpy(email, guest.email, strlen(guest.email));
           //email[strlen(email)] = '\0';
           
           strncpy(storedEmail, guest.email, strlen(guest.email));
           storedEmail[strlen(storedEmail)] = '\0';           
           
       }
       else { 
           printf("\n ! HIBA Vendeg nem talalhato !\n\n"); 
         
       }
    
       fclose(ptr_visitorsFile);
       //printf("StoredEML %s returned, VD closed", email);
     //return email;
    }
        
  
   //a rendezvény ID je alapján az eventsData fájlt olvasva 
 // vissszaadja a rendezvény nevét hordozó karakterláncot     
   char* getEventName(int eventID){
      
       static char eventName[] = "";
       FILE* ptr_eventsFile;
       EVENTS event; 
       
       ptr_eventsFile = fopen( "eventsData" , "rb" );
       
       if (ptr_eventsFile==NULL){
           perror("! eventsFile nyitási hiba !\n"); 
           exit(1);
       } else {
         //  printf("\neventsData opened, looking for EVTNAME of %d ID\n",eventID);
       }   
   
       while( fread( &event, sizeof(event), 1, ptr_eventsFile ) ){           
            //printf("\n %d \n", event.eventID );
            if ( event.eventID == eventID ) {
                break;
            }
       }
       if ( event.eventID == eventID ){ 
            //printf(" found event %s ", event.eventName ); 
            strncpy( eventName, event.eventName, strlen(event.eventName) );
            eventName[strlen(event.eventName)] = '\0';
       }
       else { 
           printf("\n HIBA ! Rendezveny nem talalhato !\n\n"); 
          
       }
    
       fclose(ptr_eventsFile);
       //printf("\neventname %s returned, ED closed\n", eventName);
           
   return eventName;
   }
   
   
   //a rendezvény nevét hordozó karakterlánc alapján az eventsData fájlt olvasva 
   //vissszaadja a rendezvény ID t tartalmazó egészet     
   int getEventID(char* eventName){
       int eventID;
       FILE* ptr_eventsFile;
       EVENTS event; 
       
       ptr_eventsFile = fopen( "eventsData" , "rb" );
       
       if (ptr_eventsFile==NULL){
       perror("! eventsData nyitási hiba !\n"); 
           exit(1);
       }
       
       while ( fread( &event, sizeof(event), 1, ptr_eventsFile) ) {
          if ( strcmp(event.eventName, eventName) == 0){
                break;
            }
       }
       
       if ( strcmp(event.eventName, eventName) == 0){ eventID = event.eventID; }
       else { 
           eventID = -1; 
           printf("\n HIBA ! Rendezveny nem talalhato !\n\n"); 
       }
       fclose(ptr_eventsFile);
       
    return eventID;
    }
   
   
   //a rendezvény ID ken átfutva eldönti, van e ilyen renzevény
   //ha van 0 egészet, ha nincs -1 egészet ad vissza
   int eventExists(int eventID){
       int err = -1;
       FILE* ptr_eventsFile;
       EVENTS event; 
       
       ptr_eventsFile = fopen( "eventsData" , "rb" );
              
       if (ptr_eventsFile==NULL){
           perror("! eventsData nyitási hiba !\n"); 
           exit(1);
       } else {
           //printf("\neventsData opened, looking for %d ID event's existence\n",eventID);
       } 
       
        while( fread( &event, sizeof(event), 1, ptr_eventsFile ) ){           
            //printf("\n %d %s \n", event.eventID, event.eventName );
            if ( event.eventID == eventID ) {
                err = 0;
                break;
            }
       }
       if ( err == 0 ){ 
            //printf(" found EVENT %s ", event.eventName ); 
            
       } else { 
           printf("\n HIBA! Rendezveny nem talalhato !\n\n"); 
       }
       
       fclose(ptr_eventsFile); 
   return err;
   }

   //a vendég ID je alapján a VisitorsData fájlt olvasva 
 // vissszaadja a vendég nevét hordozó karakterláncot     
   char* getGuestName(int guestID){
       
       static char guestName[] = "";
       FILE* ptr_visitorsFile;
       GUESTS guest; 
                
       ptr_visitorsFile = fopen( "visitorsData" , "rb" );
              
       if (ptr_visitorsFile==NULL){
           perror("! visitorsData nyitási hiba !\n"); 
           exit(1);
       } else {
          //printf("\nguestsData opened, looking for GUESTNAME of %d ID\n",guestID);
       }   
   
       while( fread( &guest, sizeof(guest), 1, ptr_visitorsFile ) ){           
            //printf("\n %d %s \n", guest.guestID , guest.guestName );
            if ( guest.guestID == guestID ) {
                break;
            }
       }
       if ( guest.guestID == guestID ){ 
            //printf(" found GUEST %s ", guest.guestName ); 
            strncpy( guestName, guest.guestName, strlen(guest.guestName) );
            guestName[strlen(guest.guestName)] = '\0';
       }
       else { 
           printf("\n HIBA ! Vendeg nem talalhato !\n\n"); 
         
       }
    
       fclose(ptr_visitorsFile);
       //printf("guestName %s returned, GD closed", guestName);
       
    return guestName;
    } 
   
   
   
//----------------------A feladatot megvalosito fuggvenyek
   
 //"nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát
 /// jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció!":
 
   //már felvitt vendég esetén tudunk rendezvényre jelentkezni:
   // visszaadja, hányadik vendégként jelentkeztünk az adott rendezványre (order int)
  int participateEventID(char* guestName, char* email, int eventID, char* storedEmail){
      
      int order=1;
      int guestID;
      int emailErr = 0;
      int eventNotExistsErr = 0;
      
      REGISTRATIONS registration; 
      FILE* ptr_regFile;
            
      guestID = getGuestID(guestName);
           
      getEmail(guestID, storedEmail);
          
      printf("\n GID%d  STE%s", guestID, storedEmail);
      printf("\n EML%s",email);
                  
      if( (strcmp ( storedEmail, email) != 0 ) && ( guestID != -1 ) ){
          printf("\n ! HIBA: a regisztraciokor megadott email nem egyezik a most megadottal !\n\n");
          emailErr = -1;
      }
      
      if ( eventExists( eventID ) == -1 ) {
          eventNotExistsErr = -1;
      }
      //ha minden rendben, regisztralunk:
      if ( guestID!= -1 && emailErr!= -1 && eventNotExistsErr!= -1 ) {
      
          ptr_regFile = fopen( "regData" , "a+b" );
          if (ptr_regFile==NULL){
              perror("! regData nyitási hiba !\n"); 
              exit(1);
          }

          while ( fread( &registration, sizeof(registration), 1, ptr_regFile ) ){       
               if ( registration.eventID==eventID ) { ++order; }        
          }

          registration.guestID = guestID;
          registration.eventID = eventID;
          registration.order = order;
          registration.registrationTime = getTime();

          fwrite( &registration, sizeof(registration), 1, ptr_regFile );           

          fclose(ptr_regFile);
      }
      else { order = -1; }
   return order;
   }
  

//a teljes adathalmaz áttekinthető listázására teszt, ellenőrzési céllal        
void list(){
    
    FILE* ptr_visitorsFile;
    FILE* ptr_regFile;
    FILE* ptr_eventsFile;
    
    GUESTS vis;
    REGISTRATIONS reg;
    EVENTS event;
    
    ptr_visitorsFile = fopen("visitorsData", "a+b");
    if (ptr_visitorsFile == NULL) {
        perror("! visitorsData nyitási hiba !\n"); 
    }
    ptr_regFile = fopen("regData", "a+b");
    if (ptr_regFile == NULL) {
        perror("! regData nyitási hiba !\n"); 
    }
    ptr_eventsFile = fopen("eventsData", "a+b");
    if (ptr_eventsFile == NULL) {
        perror("! eventsData nyitási hiba !\n"); 
    }
    
    
    printf("\n    ID \t\t\t     Nev \t\t\t\t  Email");
    printf("\n    -- \t\t\t     --- \t\t\t\t  ----- \n");   
    while ( fread( &vis, sizeof(vis), 1, ptr_visitorsFile) ) {
        printf("\n %5d \t %25s \t %30s", vis.guestID, vis.guestName, vis.email);        
    }
    printf("\n\n");   

    printf("\n rend.ID \t\t    RendezvenyNev");
    printf("\n ------- \t\t    ------------- \t \n");   
    while ( fread( &event, sizeof(event), 1, ptr_eventsFile) ) {
        printf("\n %5d \t %30s", event.eventID, event.eventName);        
    }
    printf("\n\n");   
    
    printf("\n rendID  vendID  reg.Sorrend \t time");
    printf("\n ------  ------  ----------- \t ----\n");   
    while ( fread( &reg, sizeof(reg), 1, ptr_regFile) ) {
        printf("\n %5d \t %5d \t  %5d    %30s", reg.eventID, reg.guestID, reg.order, ctimeToStr(reg.registrationTime));        
    }
    printf("\n");   
       
    fclose(ptr_visitorsFile);
    fclose(ptr_regFile);
    fclose(ptr_eventsFile);
    
}


// a jelentkezettek listázásához
// a bemenő paraméterek a listázandó rendezvény IDje , és a regisztrációkat tartalmazó fájl hossza
// visszatérési értékében tud hibát jelezni, ha nem találta az adott rendezvényt
int listGuests(int eventID, int filehossza ) {

    int err=-1;
    int darab=0;
    int i, order;

    REGISTRATIONS egyezik[filehossza];
    REGISTRATIONS reg;

    FILE* ptr_regFile;

    char* guestName;
    char* eventName;
    char* timestr;
    time_t time;

    ptr_regFile = fopen("regData", "rb");
    if (ptr_regFile == NULL) {
        perror("! regData nyitási hiba !\n"); 
        exit(1);
    }

    eventName = getEventName(eventID);
    printf("\n %s rendezveny vendeglistaja:\n\n", eventName );
   
    while ( fread(&reg, sizeof(reg), 1, ptr_regFile ) ) {
        if ( reg.eventID == eventID ) {
            err=0;
            egyezik[darab] = reg;
            ++darab;         
        }        
    }
    
    printf(" \t\t  Nev       Sorrend \t\tJelentk.ido ");
    for(i=0; i<darab; ++i){
        
        guestName = getGuestName(egyezik[i].guestID);
        order = egyezik[i].order;
        time = egyezik[i].registrationTime;
        timestr = ctimeToStr(time);
        
        printf("\n %22s    %4d     %28s", guestName, order, timestr);
        
    }
    printf("\n");
        
    if (err==-1) { printf("\n ! HIBA: Nem talalhato az adott rendezveny !\n\n"); }

    fclose(ptr_regFile);
    darab=0;

return err;
}
	


//egy vendég adatainak módosításához, bemenó par: a vendég ID je
//ha volt ilyen vendég 0-t, ha nem 1et ad vissza
 int modifyGuestData(int guestID){
    
    int err=-1;
    FILE* ptr_visitorsFile;
    GUESTS guest;
    char newName[40];
    char newEmail[50];
    fpos_t pos;
        
    printf("A vendeg nevet erre modositjuk: ");
    fgets( newName, 40, stdin );
    removeNewLine(newName);
    
    printf("A vendeg emailcimet erre modositjuk: ");
    fgets( newEmail, 50, stdin );
    removeNewLine(newEmail);
    
        ptr_visitorsFile = fopen( "visitorsData" , "r+b" );
        if (ptr_visitorsFile==NULL){
            perror("! visitorsData nyitási hiba !\n"); 
            exit(1);
        }
        
            while ( !feof(ptr_visitorsFile) && err == -1 ){
          
            fgetpos(ptr_visitorsFile, &pos);
            fread( &guest, sizeof(guest), 1, ptr_visitorsFile );     
                                    
            if ( guest.guestID==guestID ) { 
                    err=0;

                    strncpy( guest.guestName , newName , strlen(newName) );
                    guest.guestName[strlen(newName)]='\0';
                   
                    strncpy( guest.email , newEmail , strlen(newEmail) );
                    guest.email[strlen(newEmail)]='\0';

                    fsetpos(ptr_visitorsFile, &pos);
                    fwrite( &guest, sizeof(guest), 1, ptr_visitorsFile );  
                    printf("\n Sikeres modositas. \n\n"); 
                }
            }       
        if (err==-1) {
            printf("\n ! HIBA: Nem talalhato az adott szemely !\n\n");
        }
        
    fclose(ptr_visitorsFile);
    return err;
}



//vendég törlése    vendég ID alapján
// további bemenő paraméterek: a vendégeket és regisztrációkat tartelmezó fájl hossza 
//tovabbgyuruzo torles a rendezvenyek regisztracioit nyilvantarto fajlban
//
//
//bemenő paraméterek: vendég ID, a regisztrációs fájl hossza 
//                      és a rendezvényeket tartalmazó fájl hoszza
//tömbbe olvassa az adatokat, majd a törlendők nélkül mindent kiír
//
//ennek a megoldásnak sajnos nagy a lépésszáma és memóriaigénye
//lehetséges javítás lenne az adatokat tartalmazó structok bővítése
//egy töröltséget jelző mezővel, és bizonyos időközönként "karbantartás"
//azaz a töröltnek jelöltek valódi törlése 
int deleteGuest(int guestID, int gu_filelen, int reg_filelen){
    int err=-1;
    int i;
    
    GUESTS guest;
    REGISTRATIONS reg;
    
    GUESTS garr[gu_filelen];
    REGISTRATIONS regarr[reg_filelen];
    
    FILE* ptr_regFile;
    FILE* ptr_guestsFile;
    
            
        ptr_guestsFile = fopen( "visitorsData" , "rb" );
        if (ptr_guestsFile==NULL) { 
            perror("! visitorsData nyitási hiba !\n"); 
            exit(1); }
        ptr_regFile = fopen( "regData", "rb" );
        if (ptr_regFile == NULL) {
            perror("! regData nyitási hiba !\n"); 
            exit(1); 
        }
        
        for(i = 0; i < gu_filelen-1; i++){
            fread(&guest, sizeof(guest), 1, ptr_guestsFile);
            garr[i] = guest;
            if(guest.guestID == guestID) { err = 0; }
        }
        for(i = 0; i < reg_filelen-1; i++){
            fread(&reg, sizeof(reg), 1, ptr_regFile);
            regarr[i] = reg;
            if(reg.guestID == guestID) { err = 0; }
        }
                
        fclose(ptr_guestsFile);
        fclose(ptr_regFile);
        
         if (err==-1) { 
        printf("\n! HIBA: Nem talalhato az adott vendeg !\n\n");      
    }
    
    ptr_guestsFile = fopen( "visitorsData", "wb" );
    if (ptr_guestsFile == NULL) { 
        perror("! visitorsData nyitási hiba !\n"); 
        exit(1); 
    }
    
    ptr_regFile = fopen( "regData", "wb" );
    if (ptr_regFile == NULL) { 
        perror("! regData nyitási hiba !\n"); 
        exit(1); 
    }
       
    for(i=0; i<gu_filelen-1; i++){
        if( garr[i].guestID == guestID ) { continue; }
        
        fwrite(&garr[i], sizeof(garr[i]), 1, ptr_guestsFile);
    }
    for(i=0; i<reg_filelen-1; i++){
        if( regarr[i].guestID == guestID ) { continue; }
        
        fwrite(&regarr[i], sizeof(regarr[i]), 1, ptr_regFile);
    }
    
    fclose(ptr_guestsFile);
    fclose(ptr_regFile);
    if ( err == 0 ) { printf("\n Sikeres torles. \n\n"); }       
return err;    
}

//új rendezvény indítás
void createEvent(){
    
    int maxEventID = 0;
    char eventName[40];
    FILE* ptr_eventsFile;
    
    EVENTS event; 
    
    printf("\nAz uj rendezveny neve: ");
    fgets(eventName, 40, stdin);
    removeNewLine(eventName);
    
    ptr_eventsFile = fopen( "eventsData" , "a+b" );
    if (ptr_eventsFile==NULL){
        perror("! eventsData nyitási hiba !\n"); 
        exit(1);
    }
    
    while( fread(&event, sizeof(event), 1, ptr_eventsFile) ){
        if (event.eventID > maxEventID) { maxEventID = event.eventID; }
    }
    
    strncpy( event.eventName , eventName , strlen(eventName) );
    event.eventName[strlen(eventName)]='\0';
    
    event.eventID = maxEventID + 1;
    
    fwrite( &event, sizeof(event), 1, ptr_eventsFile );
      
    fclose(ptr_eventsFile);
    printf("\n Uj rendezveny sikeresen rogzitve. \n\n");
}

//új vendég felvitele
void createGuest(){
    
    int maxGuestID = 0;
    int email_format_err = 0;
    int email_length;
    char guestName[40];
    char email[50];
    
    FILE* ptr_visitorsFile;
    GUESTS guest; 
    
    printf("\nAz uj vendeg neve: ");
    fgets( guestName, 40, stdin );
    removeNewLine( guestName );
    
    printf("\nAz uj vendeg emailcime: ");
    fgets( email, 50, stdin );
    removeNewLine( email );
    
    email_length = strlen(email);
    email_format_err = validateEmailFormat( email, email_length );   

    if ( email_format_err == 0 ) {
            
        ptr_visitorsFile = fopen( "visitorsData" , "a+b" );
        if ( ptr_visitorsFile==NULL ){
            perror("! visitorsData nyitási hiba !\n"); 
            exit(1);
        }

        while( fread( &guest, sizeof(guest), 1, ptr_visitorsFile) ){
            if( guest.guestID > maxGuestID ) { maxGuestID = guest.guestID; }
        }

        strncpy( guest.guestName , guestName , strlen(guestName) );
        guest.guestName[strlen(guestName)]='\0';

        strncpy( guest.email , email , strlen(email));
        guest.email[strlen(email)]='\0';

        guest.guestID = maxGuestID + 1;

        fwrite( &guest, sizeof(guest), 1, ptr_visitorsFile );

        fclose(ptr_visitorsFile);
        printf("\n Uj vendeg sikeresen rogzitve. \n\n");
    } else { 
        printf("\n ! Hibas formatumu emailcim ! \n\n");
    }
    
    
}

//egy rendezvény összes adatának törlése
//tovabbgyuruzo torles a rendezvenyek regisztracioit nyilvantarto fajlban
//bemenő paraméterek: rendezvényID, a regisztrációs fájl hossza 
//                      és a rendezvényeket tartalmazó fájl hoszza
//tömbbe olvassa az adatokat, majd a törlendők nélkül mindent kiír
//
//ennek a megoldásnak sajnos nagy a lépésszáma és memóriaigénye
//lehetséges javítás lenne az adatokat tartalmazó structok bővítése
//egy töröltséget jelző mezővel, és bizonyos időközönként "karbantartás"
//azaz a töröltnek jelöltek valódi törlése
int deleteEventData(int eventID, int ev_filelen, int reg_filelen){
    int err=-1;
    int i;
    
    REGISTRATIONS reg;
    EVENTS event;
    
    EVENTS evarr[ev_filelen];
    REGISTRATIONS regarr[reg_filelen];
    
    FILE* ptr_eventsFile;
    FILE* ptr_regFile;
    
    ptr_eventsFile = fopen( "eventsData", "rb" );
    if (ptr_eventsFile == NULL) { 
        perror("! eventsData nyitási hiba !\n"); 
        exit(1); 
    }
    
    ptr_regFile = fopen( "regData", "rb" );
    if (ptr_regFile == NULL) { 
        perror("! regData nyitási hiba !\n"); 
        exit(1); 
    }
    
    for(i = 0; i < ev_filelen-1; i++){
        fread(&event, sizeof(event), 1, ptr_eventsFile);
        evarr[i] = event;
        if(event.eventID == eventID) { err = 0; }
    }
    for(i = 0; i < reg_filelen-1; i++){
        fread(&reg, sizeof(reg), 1, ptr_regFile);
        regarr[i] = reg;
        if(reg.eventID == eventID) { err = 0; }
    }
    
    fclose(ptr_eventsFile);
    fclose(ptr_regFile);
    
    if (err==-1) { 
        printf("\n! HIBA: Nem talalhato az adott rendezveny !\n\n");      
    }
    
    ptr_eventsFile = fopen( "eventsData", "wb" );
    if (ptr_eventsFile == NULL) { 
        perror("! eventsData nyitási hiba !\n"); 
        exit(1); 
    }
    
    ptr_regFile = fopen( "regData", "wb" );
    if (ptr_regFile == NULL) { 
        perror("! regData nyitási hiba !\n"); 
        exit(1); 
    }
       
    for(i=0; i<ev_filelen-1; i++){
        if( evarr[i].eventID == eventID ) { continue; }
        
        fwrite(&evarr[i], sizeof(evarr[i]), 1, ptr_eventsFile);
    }
    for(i=0; i<reg_filelen-1; i++){
        if( regarr[i].eventID == eventID ) { continue; }
        
        fwrite(&regarr[i], sizeof(regarr[i]), 1, ptr_regFile);
    }
    
    fclose(ptr_eventsFile);
    fclose(ptr_regFile);
    
    if ( err == 0 ) { printf("\n Sikeres torles. \n\n"); }
return err;
}




//----------------------Foprogram

int main(int argc,char** argv){
 
int     newrun = 1;
int     choice, order;
int     regFileLen, eventsFileLen, guestsFileLen;
char    vendegID[10], rendezvenyID[10], guestName[40], email[50];
char    input[10]; 
char    storedEmail[50];


//menü megjelenítése és elágaztatás a választott opció szerint
    while(newrun==1){

        choice=0;
	printf("\n");
	printf(" 1 - Jelentkezettek listazasa \n");
	printf(" 2 - Nyilvantartasba jelentkezes    (uj vendeg)\n");
	printf(" 3 - Rendezvenyre jelentkezes       (mar letezo vendeggel)\n\n");
        
	printf(" 4 - Vendeg adatainak modositasa \n");
	printf(" 5 - Vendeg torlese \n\n");
        
        printf(" 6 - Uj rendezveny inditasa \n");
        printf(" 7 - Rendezveny osszes adatanak torlese \n\n");
        
	printf(" 8 - Teljes adathalmaz megjelenitese \n\n");
        
        printf(" 9 - Kilepes \n");
	
	printf("\nValasztasom: ");
	
	fgets(input, 10, stdin);
	choice = atoi(input);
	
	switch(choice){
	
	case 1:
            regFileLen = fajlhossz("regData");            
            printf("\nA listazando rendezveny ID-je: ");
            fgets( rendezvenyID, 10, stdin );
            listGuests( atoi(rendezvenyID), regFileLen );
            break;
	
	case 2:
            createGuest();
            break;
	
	case 3:
            memset(storedEmail,0,50);
            memset(email,0,50);
            printf("\nA jelentkezo neve: ");
            fgets( guestName, 40, stdin );
            removeNewLine(guestName);
            
            printf("\nA jelentkezo emailcime: ");
            fgets( email, 50, stdin );
            removeNewLine(email);
            
            printf("\nA rendezveny ID-je: ");
            fgets( rendezvenyID, 10, stdin );
                                    
            order = participateEventID( guestName, email, atoi(rendezvenyID), storedEmail );
            if ( order > 0 ) {  
                printf("\nA rendezvenyre jelentkezes a \t %d .sorszamu vendegkent sikeres volt! \n", order);
            }
            //memset(storedEmail,0,strlen(storedEmail));
            memset(storedEmail,0,50);
            memset(email,0,50);
            break;
	
	case 4:
            printf("\nA modositando vendeg ID-je: ");
            fgets( vendegID, 10, stdin );
            modifyGuestData( atoi(vendegID) );
            break;

	case 5:
            regFileLen = fajlhossz("regData");
            guestsFileLen = fajlhossz("visitorsData");
            
            printf("\nA torlendo vendeg ID-je: ");
            fgets( vendegID, 10, stdin );
                        
            deleteGuest( atoi(vendegID), guestsFileLen, regFileLen );
            break;
                
        case 6:
            createEvent();
            break;
        
        case 7:
            regFileLen = fajlhossz("regData");
            eventsFileLen = fajlhossz("eventsData");
            
            printf("\nA torlendo rendezveny ID-je: ");
            fgets( rendezvenyID, 10, stdin );
                        
            deleteEventData( atoi(rendezvenyID), eventsFileLen, regFileLen );
            break;        
                 
	case 8:
            list();
            break;
            
        case 9:
            newrun = 0;
            break;
		
	default:
            printf("\n ! Adjon meg ervenyes menupontot !\n");
            break;
    }
}

printf("\n A program futasa veget ert! \n\n");
   
return 0;
}