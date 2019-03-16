#include <stdio.h>
#include <stdlib.h>

int main(){



char str[] ="Ez a probax";

char str3[]="szoVeg";
char str4[]="MasszoVeg";
char str5[]="szoVeg";


char *ptr_str; 
char *ptr_str1;


ptr_str = &str[0];
ptr_str1 = &str[10];


printf("Hello %s", str);


printf("ez a mem cim: %d", ptr_str  );


printf("ez a mem cim: %d", ptr_str1  );

printf("a kulonbseg: %d", (ptr_str1-ptr_str)  );

printf("utso karakter: %c", *ptr_str1  );

printf("\n");

printf(" \n ez a karakter sorozat  hossza:  %d ", length(str) );

printf(" \n ket szoveg nem egyenloa:  %d ", equal(str3, str4) );
printf(" \n ket szoveg  egyenloa:  %d ", equal(str3, str5) );


printf("Malloc");

Memofoglalas();


printf("\n Malloc - vege");


}
////////////

void Memofoglalas( ){

char *String;
String = malloc(20);




free(String);

}



/////////////////////
int equal(char str1[], char str2[]){

char *ptr_String1 = &str1[0];
char *ptr_String2 = &str2[0];
int i =0;
        do {

//              printf("ez a szam %d /n ", i );
                if  (*ptr_String1 ==*ptr_String2){
		i =i +1;
		ptr_String1= &str1[i];
		ptr_String2= &str2[i];
		}else{
			 i =0;
			 break;
		}

         

        }while(*ptr_String1!=NULL);



return i;
}

///////////////////////////////////////////
int length(char str[]){

int i =0;

char *ptr_String = &str[0];



	do {
		
//		printf("ez a szam %d /n ", i );
//		 printf("ez a szam %c /n ", *ptr_String);
	
	   i = i+1;	
	   ptr_String= &str[i];
	  
	}while(*ptr_String!=NULL);

return i;

}



