#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quest_bank.h"
//char input_q[512];
//char [200][80]answers;	
//use double pointer
//char  questions[50][80] ;



int cnt_q = 0;
int cnt_a = 0;

struct quest_bank qbank[250];
int num_of_q_a = 0;
int num_of_lin = 0;
//int n_q = sizeof(questions);
//int n_a = sizeof(answers);

/*
allocate memory
char **line_array = malloc(e_line_number * sizeof(char*));
  int i;
  for(i = 0;i<e_line_number;i++) {
    line_array[i] = malloc(BUFLOWSIZE * sizeof(char));
  }

deallocate memory
int t;
for(t = 0;t<5;t++) {
  printf("%s\n",pointer[t]);
  free(pointer[t]);
}
*/

void close_file(){
	/*
	char input[250];


	FILE *my_file = fopen("questions.txt", "r");
	//* malloc (sizeof (int));
	//char input[512];
	if(my_file == NULL) 
    {

      	perror("Error opening file");
      	//return(-1);
    }
    int cnt = 0;
  	int line = 0;
	*/

}

void open_file(){

  	char input[250];


	FILE *my_file = fopen("questions.txt", "r");
	//* malloc (sizeof (int));
	//char input[512];
	if(my_file == NULL) 
    {
      	perror("Error opening file");
      	//return(-1);
    }
  	int cnt = 0;
  	int line = 0;
	//look to resize array
	//check for empty file
	while(fgets(input, 250, my_file)!= NULL){// && input != ""){
    int cnt_answ = 0;
    strcpy(qbank[cnt].questions, input);
    //printf("THis is question number %d: -> %s", cnt+1 , qbank[cnt].questions);
    //num_of_q_a++;
    line++;
	num_of_lin++;
    //read and copy 2nd line
    fgets(input, 250, my_file);
    strcpy(qbank[cnt].answers[cnt_answ], input);
    //printf("It is from qbank %d -> %s", line , qbank[cnt].answers[cnt_answ]);
    //num_of_q_a++;
    cnt_answ++;
    num_of_lin++;
	line++;
    //read and copy 3rd line
    fgets(input, 250, my_file);
    strcpy(qbank[cnt].answers[cnt_answ], input);
    //printf("%d -> %s", line , qbank[cnt].answers[cnt_answ]);
    //num_of==_q_a++;
    cnt_answ++;
    num_of_lin++;
	line++;
    //read and copy 4th line
    fgets(input, 250, my_file);
    strcpy(qbank[cnt].answers[cnt_answ], input);
    //printf("%d -> %s", line , qbank[cnt].answers[cnt_answ]);
    //num_of_q_a++;
    cnt_answ++;
    num_of_lin++;
	line++;
    //read and copy 5th line
    fgets(input, 250, my_file);
    strcpy(qbank[cnt].answers[cnt_answ], input);
    //printf("%d -> %s", line , qbank[cnt].answers[cnt_answ]);
    //num_of_q_a++;
    num_of_lin++;
	line++;
    //empty line
    fgets(input, 250, my_file);
     if (input == "")
     { 
      line++;
     }
    //printf("Line:%d -> %s", line , input);
    //printf("THis is question banks quest %s\n", qbank[cnt].questions);
    cnt++;
	}
	printf("\n\nEnd Of Program\n");
	fclose(my_file);
  num_of_q_a = cnt;

	//memcpy(qbank, input, strlen(input)+1);
  //printf("THis is qbank %s\n", qbank.questions[0]);
  
	//questions[511] = '\0';
	//not sure what is happening????????????????????????????//
    //questions = malloc(sizeof(char)*100);

}

void list_data(){
  //int s_q_bank = sizeof(qbank);
  int i = 0;
  
  printf("SIze of qbank is %d\n", num_of_q_a);
  
  while(i < num_of_q_a){
    int j = 0;
    printf(qbank[i].questions, "\n");
    printf(qbank[i].answers[j], "\n");
    j++;
    printf(qbank[i].answers[j], "\n");
    j++;
    printf(qbank[i].answers[j], "\n");
    j++;
    printf(qbank[i].answers[j], "\n");
    i++;
    
  }
  
  
}

/*
char *line = NULL;
+size_t size;
+if (getline(&line, &size, stdin) == -1) {
-if (getline(&line, 0, stdin) == -1) {
    printf("No line\n");
} else {
*/
void add_quest_answ(){
	int cnt_answ = 0;
	//char * inp_quest;

	size_t BUFFERSIZE = 50;
	char buffer[BUFFERSIZE];
	//char *text = calloc(1,1), buffer[BUFFERSIZE];
	//while( fgets(buffer, BUFFERSIZE , stdin) ) /* break with ^D or ^Z */
	//{
		//n++;
  		//text = realloc( text, strlen(text)+1+strlen(buffer) );
  		//if( !text ) 
  		//{
  	//		perror("Error happened\n");
  //		}/* error handling */
  		//strcat( text, buffer ); /* note a '\n' is appended here everytime */
  		//printf("%s\n", buffer);
	//}
/*
	char *buffer;
    size_t bufsize = 32;
    size_t characters;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    printf("Type something: ");
    characters = getline(&buffer,&bufsize,stdin);
    printf("%zu characters were read.\n",characters);
    printf("You typed: '%s'\n",buffer);
*///inp_quest = 
	//printf(getlines());
	//printf("This is input %s\n", inp_quest);
	
	//read line question from terminal
	//??? fgetc();
	
	strcat(buffer, "5.");
	do{
		printf("Enter a question\n");
		fgets(buffer, BUFFERSIZE , stdin);
		printf("input buffer\n%s",buffer);
		
	}while(buffer[0] == '\n');
	
	strcpy(qbank[num_of_q_a].questions, buffer);
	printf("input qbank\n%s",qbank[num_of_q_a].questions);
	num_of_lin++;
	printf("THe number of lines are %d\n", num_of_lin);
	
	do{
		printf("Enter first answer\n");
		fgets(buffer, BUFFERSIZE , stdin);
		printf("input buffer\n%s",buffer);
	}while(buffer[0] == '\n');
	strcpy(qbank[num_of_q_a].answers[cnt_answ], buffer);
	printf("input qbank\n%s",qbank[num_of_q_a].answers[cnt_answ]);
	cnt_answ++;
	num_of_lin++;
	printf("THe number of lines are %d\n", num_of_lin);
	printf("Question number is %d\n", cnt_answ);

	do{
		printf("Enter second answer\n");
		fgets(buffer, BUFFERSIZE , stdin);
		printf("input buffer\n%s",buffer);
	}while(buffer[0] == '\n');
	strcpy(qbank[num_of_q_a].answers[cnt_answ], buffer);
	printf("input qbank\n%s",qbank[num_of_q_a].answers[cnt_answ]);
	cnt_answ++;
	num_of_lin++;
	printf("THe number of lines are %d\n", num_of_lin);
	printf("Question number is %d\n", cnt_answ);	

	do{	
		printf("Enter third answer\n");
		fgets(buffer, BUFFERSIZE , stdin);
		printf("input buffer\n%s",buffer);
	}while(buffer[0] == '\n');
	strcpy(qbank[num_of_q_a].answers[cnt_answ], buffer);
	printf("input qbank\n%s",qbank[num_of_q_a].answers[cnt_answ]);
	cnt_answ++;
	num_of_lin++;
	printf("THe number of lines are %d\n", num_of_lin);
	printf("Question number is %d\n", cnt_answ);	

	do{
		printf("Enter forth answer\n");
		fgets(buffer, BUFFERSIZE , stdin);
		printf("input buffer\n%s",buffer);
	}while(buffer[0] == '\n');
	strcpy(qbank[num_of_q_a].answers[cnt_answ], buffer);
	printf("input qbank\n%s",qbank[num_of_q_a].answers[cnt_answ]);
	cnt_answ++;
	num_of_lin++;
	printf("THe number of lines are %d\n", num_of_lin);
	printf("Question number is %d\n", cnt_answ);	

	num_of_q_a++;

	printf("NUmber of question answer is %d\n", num_of_q_a);
	
}

void add_data(){
	
    //add data to the array
	printf("The number of lines total are %d\n", num_of_lin);
	printf("NUmber of quest and answer are %d\n", num_of_q_a);
	add_quest_answ();
	printf("The number of lines after addition total are %d\n", num_of_lin + 1);
	
	
	//printf("Type of arraqy", typeof(questions));
	//questions[cnt_q] = inp_q;
	//cnt_q++; 
} 

void delete_quest_answ(int index)
{
	int c;
	if ( index >= num_of_q_a + 1 ){
      		printf("Deletion not possible.\n");
	}
  	else
   	{
      		for ( c = index - 1 ; c < num_of_q_a - 1 ; c++ ){
        	 	//array[c] = array[c+1];
			qbank[c] = qbank[c+1];
 		}
      		printf("Resultant array is\n");
 
      	for( c = 0 ; c < num_of_q_a  - 1 ; c++ )
         	printf("%d\n", qbank[c]);
   	}
	num_of_q_a --;
	num_of_lin -= 5;
	


}


void delete_data(int index)
{
	printf("The number of lines before deletion are %d\n", num_of_lin);
	printf("Number of quest and answer before deletion are  %d\n", num_of_q_a);
	delete_quest_answ(index);
	printf("The number of lines after deletion total are %d\n", num_of_lin + 1);
	printf("Number of quest and answer after deletion are  %d\n", num_of_q_a);
	printf("Question is deleted!!\n");



}





int main()
{ 
	
	//strcpy(questions, questionsP);	
	//questions = malloc(sizeof(char)*100);
	//answers = malloc(sizeof(char)*100);
	//????????????????????????????????????????free varaibles?????????????????
	char str;
	//void list_quest();
  	open_file();
	 
	printf("If you want list the data please press L\n");
	printf("If you want to add question and answer please press A button\n");
	printf("If you want to modify question, answer please press M button\n"); 
	printf("If you want to delete question, answer please press D button\n");
	printf("If you want to exit  please press E button\n");
	
	int exit = 0;
	int index = -1;
	while(!exit){ 
		printf("Select operation!\n");
		scanf(" %c%*c", &str);
		printf("Operation is = %c\n", str);
	
		switch(str){

		    case 'L' :
		    list_data();
		    break;
	    	case 'A' :
	    	add_data();
	    	break;
		case 'M' :
	    	//add_data();
	    	break;
		case 'D' :
		printf("Enter the question number\n");
	    	scanf(" %d", &index);
		printf("This is index %d\n", index);
		delete_data(index);
	    	break;
	    	case 'E' :
	    	exit = 1;
	    	break;
			default:
		  	printf("Enter right operation!!");
		  	break;
		}
	 
	} 
	 
	 
	//close_file();
	  
	return 0;
}

/*
 void add_answer(char *inp_a ){
  //answers[cnt_a] = inp_a;
  //cnt_a ++; 
 }
*/
 void modify_quest(int m_q){
  //printf("This is old question = %s", questions[m_q]);
  //questions[m_q] = "";
  printf("Write new one");
  //scanf("%s", questions[m_q]);
  //look again
 }
 
 void modify_answer(int m_a){
  //printf("This is old answer = %d", answers[m_a]);
  //answers[m_a] = "";
  printf("Write new one");
  //scanf("%d", answers[m_a]);
  //look again
 }
 

 
 void delete_answer(int d_a){
  /*
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
  */
 } 

 void delete_quest(int d_q){
  /*
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
  */
 }
 

