#include <stdio.h>

struct dogFavs{

  char *food;
  char *friend;
  
};

typedef struct dog{
  const char *name;
  const char *breed;
  int avgHeightCm;
  int avgWeightLbs;
 
  struct dogFavs favThings; //struct inside a struct

} dog; // typedef allows you to shorten struct "name" to just "name"


/* outputting the struct info + getting the memory location
void getDogInfo(struct dog theDog){

  printf("Name: %s\n\n", theDog.name);
  printf("breed: %s\n\n", theDog.breed);
  printf("Avg Height: %d cms\n\n", theDog.avgHeightCm);
  printf("Avg Weight: %d lbs\n\n", theDog.avgWeightLbs);
    
}

void getMemoryLocation(struct dog theDog){
  printf("Name Location: %d\n\n", theDog.name);
  printf("Breed Location: %d\n\n", theDog.breed);
  printf("Height Location: %d\n\n", &theDog.avgHeightCm);
  printf("Weight Location: %d\n\n", &theDog.avgWeightLbs);

}
*/

void getDogsFavs(dog theDog){
  printf("\n");
  
  printf("%s loves %s and his friend is %s.\n\n", theDog.name,
          theDog.favThings.food, theDog.favThings.friend);
 
}

void setDogWeightPtr(dog *theDog, int newWeight){
  (*theDog).avgWeightLbs = newWeight;
  // we can use another way that is: theDog->avgWeight  as a way
  // to point to the data we want  
  printf("The weight was changed to %d \n\n", (*theDog).avgWeightLbs);
}

void main(){
/*
  struct dog rex = {"Rex", "Saint Bernard", 90, 264};
  getDogInfo(rex);
  
  struct dog rex2 = rex;
  
  getMemoryLocation(rex);
  getMemoryLocation(rex2);
*/
      //a struct inside a struct
  dog benji = {"Benji", "Silky Terrier", 25, 9, {"Meat", "Joe Camp"}};

  //getDogsFavs(Benji);
  
  setDogWeightPtr(&benji, 11);
  
  printf("The weight in Main() is %d \n\n", benji.avgWeightLbs);
  

}