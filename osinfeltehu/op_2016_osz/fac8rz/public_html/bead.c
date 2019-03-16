#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define DATA "kingofstands.data"
#define TEMP "kingofstands_temp.data"

struct my_type
{
	int guest_ID;
	int event_ID;	
	char guest_name[100];
	char e_mail[100];
	time_t deadline;
};

void new_check_in(struct my_type new_one);
void modify_check_in(struct my_type new_one, int guest_identification);
void delete(int guest_identification);
void delete_event(int event_identification);
void recreate();
void print();

int new_check_in_ID();
const char * time_format(time_t t);

int main(int argc, char** argv){
	
	char reply[1];
	
	do{ // menu
		printf("New Check In - 1 \n");
		printf("Modify Check In - 2 \n");
		printf("Delete Check In - 3 \n");
		printf("Show Guests - 4 \n");
		printf("Delete Event -  5 \n");
		printf("Empty File - 6 \n");
		printf("Fill File - 7 \n");
		printf("Exit - 8");
		
		scanf("%s", reply);
		
		if (strcmp(reply, "1") == 0)
		{ 
			struct my_type new_one;
			
			printf("jelentkezo_neve:\n");
			scanf("%s",&new_one.guest_name);
			
			printf("e_mail:\n");
			scanf("%s",&new_one.e_mail);
			
			printf("rendezveny_azonosito:\n");
			scanf("%d",&new_one.event_ID);
			
			new_one.deadline = time(NULL);
			
			new_check_in(new_one);
			
		}
		else if (strcmp(reply, "2") == 0)
		{
			int guest_identification;
			printf("Select guest to modify by ID - \n");
			scanf("%d",&guest_identification);
			
			struct my_type new_one;
			
			printf("Guest name - \n");
			scanf("%s",&new_one.guest_name);
			
			printf("Guest E-Mail - \n");
			scanf("%s",&new_one.e_mail);
					
			printf("Event ID - \n");
			scanf("%d",&new_one.event_ID);
			
			new_one.deadline = time(NULL);
			
			modify_check_in(new_one, guest_identification);			
		}
		else if (strcmp(reply, "3") == 0)
		{
			int guest_identification2;
			printf("Select guest to delete by ID - \n");
			scanf("%d",&guest_identification2);
			delete(guest_identification2);
		}
		else if (strcmp(reply, "4") == 0)
		{
			print();
		}
		else if (strcmp(reply, "5") == 0)
		{
			int event_identification;
			printf("Select event to delete by ID - \n");
			scanf("%d",&event_identification);
			delete_event(event_identification);
		}
		else if (strcmp(reply, "6") == 0)
		{
			recreate();
		}
		else if (strcmp(reply, "7") == 0) // standard file
		{ 
			struct my_type new_one;
			
			strcpy(new_one.guest_name,"Marci"); strcpy(new_one.e_mail,"gyurjanmarcell@gmail.com");
			new_one.event_ID = 63; new_one.deadline = time(NULL); new_check_in(new_one);

			strcpy(new_one.guest_name,"Bela"); strcpy(new_one.e_mail,"bela@freemail.com");
			new_one.event_ID = 99; new_one.deadline = time(NULL); new_check_in(new_one);
			
			strcpy(new_one.guest_name,"Szetyi"); strcpy(new_one.e_mail,"szetyigyerek@citromail.com");
			new_one.event_ID=412; new_one.deadline = time(NULL); new_check_in(new_one);
			
			strcpy(new_one.guest_name,"schiman"); strcpy(new_one.e_mail,"schiman@caesarelte.com");
			new_one.event_ID=412; new_one.deadline = time(NULL); new_check_in(new_one);
			
			strcpy(new_one.guest_name,"karcsika"); strcpy(new_one.e_mail,"karcsika@gmail.com");
			new_one.event_ID=75; new_one.deadline = time(NULL); new_check_in(new_one);
			
			strcpy(new_one.guest_name,"eni"); strcpy(new_one.e_mail,"eni@asd.com");
			new_one.event_ID=22; new_one.deadline = time(NULL); new_check_in(new_one);			
		}
	
	}
	while(strcmp(reply, "8") != 0);
	
	return 0;
}

void new_check_in(struct my_type new_one)
{	
	int id_azon = new_check_in_ID();
	new_one.guest_ID = id_azon;
	
	int fileIn = open(DATA, O_WRONLY|O_APPEND);
	
	if(fileIn < 0)
	{
		printf("File opening failed! \n");
	}
	else
	{	
		if (write(fileIn, &new_one, sizeof(struct my_type)) < 0)
		{
			printf("Error editing file! \n");
		}
		else
		{
			printf("Inserted Data - %s|%s|%d|%d|%s\n",&new_one.guest_name, &new_one.e_mail, new_one.event_ID, new_one.guest_ID, time_format(new_one.deadline));
		}
	}
	close(fileIn);
}

void modify_check_in(struct my_type new_one, int guest_identification)
{	
	int temporaryFile = open(TEMP, O_WRONLY|O_CREAT, S_IRWXU);
	int fileIn = open(DATA, O_RDONLY);	
	int fail = 0;
	
	if(temporaryFile < 0 || fileIn < 0)
	{
		printf("Error opening file! \n");
	}
	else
	{
		struct my_type input_data;
	
		while (read(fileIn, &input_data, sizeof(struct my_type)) > 0)
		{				
			if (guest_identification != input_data.guest_ID)
			{
				if(write(temporaryFile, &input_data, sizeof(struct my_type)) < 0)
				{
					printf("Error displaying %i. datarow! \n",guest_identification);
					fail = 1;
				}
			}
			else
			{
				new_one.deadline = input_data.deadline;
				
				if (write(temporaryFile, &new_one, sizeof(struct my_type)) < 0)
				{
					printf("Error displaying %i. datarow! (*) \n",guest_identification);
					fail = 1;
				}
				else
				{
					printf("Successfully saved %i. ! \n",guest_identification);
				}
			}
			
		}
		
		printf("Success! \n");
		
	}
	close(temporaryFile);
	close(fileIn);
		
	if (fail == 0)
	{
		if (remove(DATA) != 0)
		{
			printf("Error saving file! (Errorcode - 1)");
		}
		else
		{
			if (rename(TEMP, DATA) != 0)
			{
				printf("Error saving file! (Errorcode - 2)");
			}
			else
			{
				printf("File saved! \n");
			}
		}
	}
}

void delete(int guest_identification)
{	
	int temporaryFile = open(TEMP, O_WRONLY|O_CREAT, S_IRWXU);
	int fileIn = open(DATA, O_RDONLY);
	int fail = 0;
	
	if(temporaryFile < 0 || fileIn < 0)
	{
		printf("Error opening file! \n");
	}
	else
	{	
		struct my_type input_data;
		
		while (read(fileIn, &input_data, sizeof(struct my_type)) > 0)
		{			
			if (input_data.guest_ID == guest_identification)
			{				
				printf("Record deleted! \n");
			}
			else
			{
				if (write(temporaryFile, &input_data, sizeof(struct my_type)) < 0)
				{
					printf("Error displaying %i. datarow! \n", input_data.guest_ID);
					fail = 1;
				}
			}
			
		}
		
		printf("Success! \n");
		
	}
	close(temporaryFile);
	close(fileIn);
		
	if (fail == 0)
	{
		if (remove(DATA)!=0)
		{
			printf("Error saving file (Errorcode - 1)");			
		}
		else
		{
			if (rename(TEMP, DATA)!=0)
			{
				printf("Error saving file (Errorcode - 2)");
			}
			else
			{
				printf("File saved successfully! \n");
			}
		}
	}
}

void delete_event(int event_identification)
{
	int temporaryFile = open(TEMP, O_WRONLY|O_CREAT, S_IRWXU);
	int fileIn = open(DATA, O_RDONLY);	
	int fail = 0;
	
	if(temporaryFile < 0 || fileIn < 0)
	{
		printf("Error opening file! \n");
	}
	else
	{		
		struct my_type input_data;
		
		while (read(fileIn, &input_data, sizeof(struct my_type)) > 0)
		{			
			if (input_data.event_ID == event_identification)
			{				
				printf("Record deleted successfully! \n");
			}
			else
			{
				if (write(temporaryFile, &input_data, sizeof(struct my_type)) < 0)
				{
					printf("Error displaying %i. datarow \n", input_data.event_ID);
					fail = 1;
				}
			}			
		}
		
		printf("Success! \n");		
	}
	close(temporaryFile);
	close(fileIn);
		
	if (fail == 0)
	{
		if (remove(DATA) != 0)
		{
			printf("Error saving file (Errorcode - 1)");
		}
		else
		{
			if (rename(TEMP, DATA) !=0 )
			{
				printf("Error saving file (Errorcode - 2)");
			}
			else
			{
				printf("File saved successfully! \n");
			}
		}
	}
}

void print()
{
	int fileIn = open(DATA, O_RDONLY);
	
	if(fileIn < 0)
	{
		printf("Error opening file! \n");
	}
	else
	{	
		struct my_type input_data;
		int row = 0;
	
		while (read(fileIn, &input_data, sizeof(struct my_type)) > 0)
		{	
			row++;
			printf("Data entered - %s|%s|%d|%d|%s\n", &input_data.guest_name, &input_data.e_mail, input_data.event_ID, input_data.guest_ID, time_format(input_data.deadline));
		}
	}
	close(fileIn);
}

void recreate()
{
	if (remove(DATA) == 0)
		printf("File successfully deleted! \n");
	else 
		printf("Could not find file to delete! \n");
	
	int fileIn = open(DATA, O_WRONLY|O_CREAT, S_IRWXU);
	if (fileIn < 0)
	{
		printf("Error opening file! \n");
	}
	else
	{
		printf("File successfully created! \n");
	}
	close(fileIn);
}

int new_check_in_ID(){
	int fileIn = open(DATA, O_RDONLY);
	int ID = 0;
	
	if (fileIn < 0)
	{
		printf("Error opening file! \n");
	}
	else
	{		
		struct my_type input_data;
		while (read(fileIn, &input_data, sizeof(struct my_type)) > 0)
		{	
			ID = input_data.guest_ID;
		}
	}
	close(fileIn);
	return ID+1;
}

const char * time_format(time_t t)
{
	static char asd[20];
	strftime(asd, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return asd;
}