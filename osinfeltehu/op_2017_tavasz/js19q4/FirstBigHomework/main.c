#include <stdio.h>
#include <stdlib.h>
//#include <string>

int main(int argc, char ** argv)
{
 char * questions[] = {};
 char * answers[] = {};
 char str ;
 int cnt_q = 0;
 int cnt_a = 0;
 int n_q = sizeof(questions);
 int n_a = sizeof(answers);
 printf("If you want list the data please press L\n");
 printf("If you want to add question please press AQ button\n");
 printf("If you want to add answer please press AA button\n");
 printf("If you want to modify question please press MQ button\n"); 
 printf("If you want to modify answer please press MA button\n");
 printf("If you want to delete question please press DQ button\n");
 printf("If you want to delete answer please press DA button\n");
 printf("Select operation!");
 scanf("%s", str);
  delete_quest(2);
  
 
 switch(str){
  case 'L' :
  // list_quest();
   break;
  default:
   printf("Enter right operation!!\n");
 }
 
 
 void list_quest(){
 
  int i;  
  for(i = 0; i < n_q; i++){
    printf(questions[i], "\n");
  }
  
 } 
 void add_quest(char inp_q){
  
  questions[cnt_q] = inp_q;
  cnt_q++; 
 } 
 void add_answer(char inp_a ){
  answers[cnt_a] = inp_a;
  cnt_a ++; 
 }
 
 void modify_quest(int m_q){
  printf("This is old question = ", questions[m_q]);
  questions[m_q] = "";
  printf("Write new one");
  scanf("%i", questions[m_q]);
  //look again
 }
 
 void modify_answer(int m_a){
  printf("This is old answer = ", answers[m_a]);
  answers[m_a] = "";
  printf("Write new one");
  scanf("%i", answers[m_a]);
  //look again
 }
 
 void delete_answer(int d_a){
  char * answer;
  int pos_a; 
  printf("This is the answer you want to delete = ", answers[d_a]);
  printf("Are you sure to delete?\n Yes(y), No(n)");
  scanf("%s", answer);
  if(answer == "y"){
   if(d_a >= n_a + 1){
    printf("Deletion is not possible\n");
   }
   else{
    for (pos_a = d_a - 1; pos_a < n_a - 1; pos_a ++ ){
     answers[pos_a] = answers[pos_a + 1];
    }
    printf("Resultant array is\n");
    
    for(pos_a = 0; pos_a < n_a - 1; pos_a++){
     printf("%d\n", answers[pos_a]);
    }
   }
  }
 } 

  
 return 0;
}

 
 void delete_quest(int d_q){
  char * answer;
  int pos_q; 
  printf("This is the question you want to delete = ", questions[d_q]);
  printf("Are you sure to delete?\n Yes(y), No(n)");
  scanf("%s", answer);
  if(answer == "y"){
   if(d_q >= n_q + 1){
    printf("Deletion is not possible\n");
   }
   else{
    for (pos_q = d_q - 1; pos_q < n_q - 1; pos_q ++ ){
     questions[pos_q] = questions[pos_q + 1];
    }
    printf("Resultant array is\n");
    
    for(pos_q = 0; pos_q < n_q - 1; pos_q++){
     printf("%d\n", questions[pos_q]);
    }
   }
  }
 }
 


