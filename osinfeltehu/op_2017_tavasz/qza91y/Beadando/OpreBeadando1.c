#include "OpreBeadando1MainMenu.h"
#include "OpreBeadando1ModifyMenu.h"

int main(void){
	
	int m = 0;
	Question_list survey;
	survey.next = NULL;
	do{
		Menu();
		fseek(stdin,0,SEEK_END);
		if( scanf("%d[\n]", &m) == 0 )
		{
			//continue;
		}
	
		
		printf("%d", m);
		
		switch(m){
			case 1: {
				int m2 = 0;
				char c = 'Y';
				while(m2 < 1 || m2 > 4) {
					// choosing menupoint
					modifyingMenu();
					printf("\nChoose a menupoint(1-4): ");
					fseek(stdin,0,SEEK_END);
					scanf("%d", &m2);
					
					// menupoints
					switch(m2) {
						case 1:
							createQuestion(&survey);
							printf("Do you want to add further question(s)? (Y/N): ");
							scanf("%c", &c);
							while(c == 'Y' || c == 'y') {
								createQuestion(&survey);
								printf("Do you want to add further question(s)? (Y/N): ");
								scanf("%c", &c);
								printf("\n");
							}
							break;
						case 2:
							modify(&survey);
							printf("Do you want to modify (an)other question(s)? (Y/N): ");
							scanf("%c", &c);
							while(c == 'Y' || c == 'y') {
								modify(&survey);
								printf("Do you want to modify (an)other question(s)? (Y/N): ");
								scanf("%c", &c);
								printf("\n");
							}
							break;
						case 3:
							deleteQuestion(&survey);
							printf("Do you want to delete further question(s)? (Y/N): ");
							scanf("%c", &c);
							while(c == 'Y' || c == 'y') {
								deleteQuestion(&survey);
								printf("Do you want to delete further question(s)? (Y/N): ");
								scanf("%c", &c);
								printf("\n");
							}
							break;
						case 4:
							break;
						default:
							printf("\n Invalid input!");
							printf("\n Hint: It accepts just numbers between 1 and 4.\n");
							break;
					}
				}
				break;
			}
			case 2:
				listingQuestions(&survey);
				break;
			case 3: 
				writeIntoFile(&survey);
				break;
			case 4:
				printf("\n\n\tGOOD BYE!\n");
				break;
			default:
				printf("\n Invalid input!");
				printf("\n Hint: It accepts just numbers between 1 and 4.\n");
				break;
		}
	}while(m != 4);
	
	return 0;
}