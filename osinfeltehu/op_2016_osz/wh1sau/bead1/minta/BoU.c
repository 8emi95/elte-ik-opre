#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Passenger
{
	char name[20];
	char destination[20];
	char phone[20];
	char date[20];
};

struct Route
{
	char destination[20];
	char date[20];
};

void list_passenger(struct Passenger passenger)
{
	printf("%s\t%s\t%s\t%s\n",passenger.name, passenger.destination, passenger.phone, passenger.date);
}

void list_passenger2(struct Passenger passenger)
{
	printf("%s\t%s\n",passenger.name, passenger.phone);
}

void list_route(struct Route route)
{
	printf("%s\t%s\n", route.destination, route.date);
}

void add_passenger(struct Passenger* p)
{
	printf("New passenger:\n");
	printf("Name: ");
	scanf("%s",(*p).name);
	printf("Destination: ");
	scanf("%s",(*p).destination);
	printf("Phone number: ");
	scanf("%s",(*p).phone);
	
	time_t now = time(0);
	strftime((*p).date, 20, "%Y.%m.%d-%H:%M", localtime(&now));

}

void add_route(struct Route* p)
{
	printf("New route:\n");
	printf("Destination: ");
	scanf("%s", (*p).destination);
	printf("Date: ");
	scanf("%s", (*p).date);
}

int main(int argc,char** argv)
{
	int i;
	int swtch1;
	int swtch2;
	int run = 1;
	
	
	struct Passenger passengers[50];
	struct Route routes[10];
	
	int passenger_num;
	int route_num;
	
	struct Passenger* pass_p;
	struct Route* route_p;
	
	FILE* passengers_in = fopen("passengers.txt", "r");
	FILE* routes_in = fopen("routes.txt", "r");
	
	fscanf(passengers_in, "%d", &passenger_num);
	fscanf(routes_in, "%d", &route_num);
	
	for(i = 0; i < passenger_num; i++)
	{
		fscanf(passengers_in,"%s %s %s %s",passengers[i].name,passengers[i].destination,passengers[i].phone,passengers[i].date);
	}
	for(i = 0; i < route_num; i++)
	{
		fscanf(routes_in, "%s %s", routes[i].destination, routes[i].date);
	}
	
	while(run)
	{
		printf("------------------------------\n");
		printf("Wellcome to the Best of Uber!\n");
		printf("1: List passengers\t\t\t4: List routes\n");
		printf("2: Add passenger\t\t\t5: Add route\n");
		printf("3: Modify/Delete passenger\t\t6: Modify/Delete route\n");
		printf("7: List a route with passengers\n");
		printf("8: Exit\n");
		
		swtch1 = 0;
		printf("------------------------------\n>");
		scanf("%d",&swtch1);
		
		switch(swtch1)
		{
			case 1:
				for(i = 0; i < passenger_num; i++)
				{
					list_passenger(passengers[i]);
				}
			break;
				
			case 2:
				pass_p = &passengers[passenger_num];
				add_passenger(pass_p);
				passenger_num++;
			break;
				
			case 3:
				printf("Name for the search: ");
				char name[20];
				int index = -1;
				scanf("%s", name);
				pass_p = NULL;
				i = 0;
				while (i < passenger_num && index == -1)
				//for(i = 0; i < passenger_num; i++)
				{
					if(strcmp(name, passengers[i].name) == 0)
					{
						pass_p = &passengers[i];
						index = i;
					}
					i++;
				}
				if (index != -1)
				{
					printf("%s found\n",(*pass_p).name);
					
					swtch2 = 0;
					printf("1: Modtfy\t2: Delete\n");
					scanf("%d",&swtch2);
				
					switch(swtch2)
					{
						case 1:
							printf("New name: ");
							scanf("%s",(*pass_p).name);
							printf("New destination: ");
							scanf("%s",(*pass_p).destination);
							printf("New phone number: ");
							scanf("%s",(*pass_p).phone);
							
							time_t now = time(0);
							strftime((*pass_p).date, 20, "%Y.%m.%d-%H:%M", localtime(&now));
							
							printf("Passenger modified\n");
						break;
						
						case 2:
							for(i = index; i < passenger_num; i++)
							{
								passengers[i] = passengers[i + 1];
							}
							passenger_num--;
							printf("Passenger deleted\n");
						break;
						
						default: 
							printf("Non existed switch!\n");
						break;
					}
				}
				else
				{
					printf("No match!\n");
				}
			break;
			
			case 4:
				
				for(i = 0; i < route_num; i++)
				{
					list_route(routes[i]);
				}
			break;
			
			case 5:
				route_p = &routes[route_num];
				add_route(route_p);
				route_num++;
				break;
			break;
			
			case 6:
				printf("Destination for the search: ");
				char des[20];
				index = -1;
				scanf("%s", des);
				route_p = NULL;
				i = 0;
				while (i < route_num && index == -1)
				//for(i = 0; i < route_num; i++)
				{
					if(strcmp(des, routes[i].destination) == 0)
					{
						route_p = &routes[i];
						index = i;
					}
					i++;
				}
				if (index != -1)
				{
					printf("%s found\n",(*route_p).destination);
					
					swtch2 = 0;
					printf("1: Modtfy\t2: Delete\n");
					scanf("%d",&swtch2);
				
					switch(swtch2)
					{
						case 1:
							printf("New destination: ");
							scanf("%s",(*route_p).destination);
							printf("New date: ");
							scanf("%s",(*route_p).date);
							printf("Route modified\n");
						break;
						
						case 2:
							for(i = index; i < route_num; i++)
							{
								routes[i] = routes[i + 1];
							}
							route_num--;
							printf("Route deleted\n");
						break;
						
						default: 
							printf("Non existed switch!\n");
						break;
					}
				}
				else
				{
					printf("No match!\n");
				}
			break;
			
			case 7:
				printf("Destination for the search: ");
				index = -1;
				scanf("%s", des);
				for(i = 0; i < route_num; i++)
				{
					if(strcmp(des, routes[i].destination) == 0)
					{
						index = i;
					}
				}
				if(index != -1)
				{
					for(i = 0; i < passenger_num; i++)
					{
						if(strcmp(passengers[i].destination, des) == 0)
						{
							list_passenger2(passengers[i]);
						}
					}
				}
				else
				{
					printf("No match!\n");
				}
			break;
			
			case 8:
				run = 0;
				printf("Program ended!\n");
			break;
			
			default: 
				printf("Non existed switch!\n");
			break;
				
		}
		
	}	
	
	fclose(passengers_in);
	fclose(routes_in);
	
	FILE* passengers_out = fopen("passengers.txt","w");
	FILE* routes_out = fopen("routes.txt", "w");
	
	fprintf(passengers_out,"%d\n",passenger_num);
	fprintf(routes_out, "%d\n", route_num);
	
	for(i = 0; i < passenger_num; i++)
	{
		fprintf(passengers_out, "%s\t%s\t%s\t%s\n", passengers[i].name, passengers[i].destination, passengers[i].phone, passengers[i].date);
	}
	for(i = 0; i < route_num; i++)
	{
		fprintf(routes_out, "%s\t%s\n", routes[i].destination, routes[i].date);
	}
	
	fclose(passengers_out);
	fclose(routes_out);
	
	return 0;
}




