#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <time.h>

static int event_ids[100];

char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void refresh_file_list(int event_id){
    FILE *f = fopen("0.txt", "a+");
    fprintf(f, "%d\n", event_id);
    fclose(f);
}

void set_event_ids_array(){
    FILE *file = fopen("0.txt", "r+");
    int num;
    int i = 0;
    while(fscanf(file, "%d", &num) > 0) {
        event_ids[i] = num;
        i++;
    }
    fclose(file);
}

char* generate_file_name(int event_id){
    char event_id_str[256];
    sprintf(event_id_str, "%d", event_id);
    char *file_name = concat(event_id_str,".txt");
    return file_name;
}

void create_file(int event_id){
    FILE *fptr;
    fptr = fopen(generate_file_name(event_id), "rb+");
    if(fptr == NULL){
        fptr = fopen(generate_file_name(event_id), "wb");
    }
}
//1,2
bool check_event_id(int event_id){
    int i = 0;
    bool is_not_valid_event_id = true;
    while(is_not_valid_event_id && (i < count_lines("0.txt"))){
        if(event_id == event_ids[i]){
            is_not_valid_event_id = false;
        }
        i++;
    }
    return is_not_valid_event_id;
}
//1
void add_new_line_to_file(const char *s1, const char *s2, const char* file_name){
    FILE *f = fopen(file_name, "a+");
    fprintf(f, "%s", s1);
    fprintf(f, "%s", s2);
    time_t mytime;
    mytime = time(NULL);
    fprintf(f,"%s", ctime(&mytime));
    fclose(f);
}

int count_lines(const char *file_name){

    FILE* f = fopen(file_name, "r");
    int ch, number_of_lines = 0;

    do{
        ch = fgetc(f);
        if(ch == '\n'){
           number_of_lines++;
        }
    } while (ch != EOF);
    fclose(f);
    return number_of_lines;

}

void add_new_guest(){

    char name[256];
    printf("Name: \n");
    scanf ("%s",&name);

    char email[256];
    printf("E-mail: \n");
    scanf ("%s",&email);

    int event_id;
    printf("Event ID: \n");
    scanf ("%d",&event_id);

    if(check_event_id(event_id)){
        printf("There is no event with the given ID.\n");
    }
    else{
        add_new_line_to_file(concat(name," - "), concat(email,": "), generate_file_name(event_id));
        printf("Guest created.\n");
        printf("Your ID: %d\n", count_lines(generate_file_name(event_id)));
    }
}
//2
void add_new_event(){

    char name[256];
    printf("Event name: \n");
    scanf ("%s",&name);

    int event_id;
    printf("Event ID: \n");
    scanf ("%d",&event_id);

    if(!check_event_id(event_id)){
        printf("There is already an event with the given ID: %d\n", event_id);
    }
    else{
        event_ids[count_lines("0.txt") - 1] = event_id;
        refresh_file_list(event_id);
        create_file(event_id);
        printf("Event created.\n");
    }
}
//3
void print_lines_of_file(const int file_number){
    FILE *file = fopen(generate_file_name(file_number), "r");
    int c;
    c = fgetc(file);
    while (c!=EOF){
        printf("%c",c);
        c = fgetc(file);
    }

}

void list_guests_of_an_event(){
    int event_id;
    printf("Event ID: \n");
    scanf ("%d",&event_id);
    print_lines_of_file(event_id);

}
//4
void  delete_guest_by_id(){
    int event_id;
    printf("Event ID: \n");
    scanf ("%d",&event_id);

    int guest_id;
    printf("Guest ID: \n");
    scanf ("%d",&guest_id);

    FILE* inFile = fopen(generate_file_name(event_id), "r");
    FILE* outFile = fopen("temp.txt", "w+");
    char line [1024];
    int i = 1;
    while( fgets(line, sizeof(line), inFile) != NULL ){
        if( ( i == guest_id ) == 0 ){
            fprintf(outFile, "%s", line);
        }
        ++i;
    }
    fclose(inFile);
    fclose(outFile);

    FILE* inFile2 = fopen("temp.txt", "r");
    FILE* outFile2 = fopen(generate_file_name(event_id), "w");

    char line2 [1024]; // maybe you have to user better value here

    while( fgets(line2, sizeof(line2), inFile) != NULL ){
            fprintf(outFile, "%s", line2);
            //printf("%s", line2);
    }
    //printf("%d",count_lines(generate_file_name(event_id)));
    fclose(inFile2);
    fclose(outFile2);
    printf("Guest deleted.\n");
    remove("temp.txt");

}
//5
void modify_guest_by_id(){
    delete_guest_by_id();
    add_new_guest();
}
//6
void delete_event_data(){
    int event_id;
    printf("Event ID: \n");
    scanf ("%d",&event_id);
    fclose(fopen(generate_file_name(event_id), "w"));
    printf("All guest deleted.\n");
}

int main() {
    create_file(0);
    bool exit = false;
    while(!exit){
        set_event_ids_array();
        int n;
        printf("\n1: Add new guest.\n2: Add new event.\n3: List guests of an event.\n4: Delete a guest.\n5: Modify guest.\n6: Delete event data.\n7: Exit.\n");
        scanf ("%d",&n);
        switch(n){
            case 1: add_new_guest(); break;
            case 2: add_new_event(); break;
            case 3: list_guests_of_an_event(); break;
            case 4: delete_guest_by_id(); break;
            case 5: modify_guest_by_id();break;
            case 6: delete_event_data();break;
            case 7: exit = true; break;
        }
    }
   return (0);
}
