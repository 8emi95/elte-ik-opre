#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include  <signal.h>


struct data{
    char question[100];
    char a[50];
    char b[50];
    char c[50];
    char d[50];
    int answer_count;
    time_t created;
};

struct data data_array[50];
int count = 0;

void save(){
    int file=open("data.dat",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    write(file,data_array,sizeof(data_array));
    close(file);
}

void show_question(int i){
    printf("%d. %s - %s--%s\n--%s\n",i+1,data_array[i].question,ctime(&data_array[i].created),data_array[i].a,data_array[i].b);
        if(data_array[i].answer_count>2)
            printf("--%s\n",data_array[i].c);
        if(data_array[i].answer_count>3)
            printf("--%s\n",data_array[i].d);
}

void add_question(){
    int file=open("data.dat",O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
    int c=0;
    int more = 1;
    char user_input[100];
    struct data d;
    printf("Kerdes: ");
    scanf("%s", user_input);
    strcpy(d.question, user_input);
    while(c<4 && more){
        printf("%d. valasz: ",c+1);
        scanf("%s", user_input);
        switch(c){
            case 0:
                strcpy(d.a, user_input);
                break;
            case 1:
                strcpy(d.b, user_input);
                printf("Tov�bbi v�lasz f�lv�tele - i\n");
                scanf("%s", user_input);
                more = !strcmp(user_input,"i");
                break;
            case 2:
                strcpy(d.c, user_input);
                printf("Tov�bbi v�lasz f�lv�tele - i\n");
                scanf("%s", user_input);
                more = !strcmp(user_input,"i");
                break;
            case 3:
                strcpy(d.d, user_input);
                break;
        }
        c++;
    }
    d.answer_count=c;
    d.created=time(NULL);
    write(file,&d,sizeof(d));
    close(file);
    data_array[count]=d;
    count++;
}

void modify_question(){
    printf("\nKerdes modositasa\n----------------------\nKerdes sorszama:\n");
    char user_input[100];
    scanf("%s", user_input);
    int id = atoi(user_input)-1;
    if(id<0||id>=count)
        printf("Nem l�tezik\n");
    else{
        show_question(id);
        struct data d = data_array[id];
        printf("1.Kerdes modositasa\n2.Elso valasz modositasa\n3.Masodik valasz modositasa\n");
        if(d.answer_count>2)
            printf("4.Harmadik valasz modositasa\n");
        if(d.answer_count>3)
            printf("5.Negyedik valasz modositasa\n");
        scanf("%s", user_input);
        char* p;
        if(!strcmp(user_input,"1")){
            p=d.question;
        } else if(!strcmp(user_input,"2")){
            p=d.a;
        } else if(!strcmp(user_input,"3")){
            p=d.b;
        } else if(!strcmp(user_input,"4")&&d.answer_count>2){
            p=d.c;
        } else if(!strcmp(user_input,"5")&&d.answer_count>3){
            p=d.d;
        } else{
            printf("Hibas input\n");
            return;
        }
        scanf("%s", user_input);
        strcpy(p,user_input);
        data_array[id]=d;
        save();
    }

}

void delete_question(){
    printf("\nKerdes torlese\n----------------------\nKerdes sorszama:\n");
    char user_input[100];
    scanf("%s", user_input);
    int id = atoi(user_input)-1;
    if(id<0||id>=count){
        printf("Nem l�tezik\n");
        return;
    }
    show_question(id);
    if(count!=id+1)
        memcpy((void*)&data_array[id],(void*)&data_array[id+1],(count-id+1)*sizeof data_array[0]);
    count--;
    save();
}

void list(){
    printf("\nKerdesek listazasa\n----------------------\n");
    int i = 0;
    for(i;i<count;++i){
        show_question(i);
    }
    printf("\n----------------------\n");
}

void finalize(char** argv){
    printf("\nKerdesek veglegesitese\n----------------------\n");
    FILE * f;
    f=fopen("final.txt","w");
    char user_input[2]={0,0};
    int c=1;
    int i=0;
    struct data final_array[50];
    for(i;i<count;++i){
        show_question(i);
        printf("Elfogadas - i\n");
        scanf("%s", user_input);
        if(!strcmp(user_input,"i")){
            fprintf(f,"%d. %s - %s--%s\n--%s\n",c,data_array[i].question,ctime(&data_array[i].created),data_array[i].a,data_array[i].b);
            if(data_array[i].answer_count>2)
                fprintf(f,"--%s\n",data_array[i].c);
            if(data_array[i].answer_count>3)
                fprintf(f,"--%s\n",data_array[i].d);
                final_array[c-1]=data_array[i];
            c++;
        }
    }
    fclose(f);

    srand(time(NULL));
    int pipe1[2];
    int pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1 ){
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }
    int pid = fork();
    if (pid == -1){
        perror("Fork hiba");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){		    	// child process
        close(pipe1[1]);
        close(pipe2[0]);
        struct data random_datas[3];
        //sleep(5);
        //printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
        read(pipe1[0],random_datas,sizeof random_datas);
    	/*printf("Gyerek kapott:");
    	for(i=0;i<3;++i){
        	printf("%d. %s - %s--%s\n--%s\n",i+1,random_datas[i].question,ctime(&random_datas[i].created),random_datas[i].a,random_datas[i].b);
        	if(random_datas[i].answer_count>2)
            		printf("--%s\n",random_datas[i].c);
        	if(random_datas[i].answer_count>3)
            		printf("--%s\n",random_datas[i].d);
    	}*/

        int consultants=rand() % 11 + 10;
        int answers[3][4];
        int k=0;

        for(i=0;i<3;++i){
            for(k;k<4;++k){
                answers[i][k]=0;
            } 
        }

        for(i=0;i<consultants;++i){
            for(k=0;k<3;++k){
                int answer = rand() % random_datas[k].answer_count;
                answers[k][answer]++;
            } 
        }

        /*printf("Gyerek elkezdi irni a csobe az adatokat!\n");
        printf("Vart eredmeny:\n");
        for(i=0;i<3;i++){
            printf("%d. %s - %s--%s - %d\n--%s - %d\n",i+1,data_array[i].question,ctime(&data_array[i].created),data_array[i].a,answers[i][0],data_array[i].b,answers[i][1]);
            if(data_array[i].answer_count>2)
                printf("--%s - %d\n",data_array[i].c,answers[i][2]);
            if(data_array[i].answer_count>3)
                printf("--%s - %d\n",data_array[i].d,answers[i][3]);
            printf("\n");
        }*/
        write(pipe2[1],answers, sizeof answers);
        
        fflush(NULL);

        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    } else{    // szulo process 
        printf("\n");
        close(pipe1[0]);
        close(pipe2[1]);

        struct data random_datas[3];
        for(i=0;i<3;++i){
            int r = rand() % (c-1);
            random_datas[i] = final_array[r];
        }
        /*printf("Szulo kuldi:");
    	for(i=0;i<3;++i){
        	printf("%d. %s - %s--%s\n--%s\n",i+1,random_datas[i].question,ctime(&random_datas[i].created),random_datas[i].a,random_datas[i].b);
        	if(random_datas[i].answer_count>2)
            		printf("--%s\n",random_datas[i].c);
        	if(random_datas[i].answer_count>3)
            		printf("--%s\n",random_datas[i].d);
    	}*/
        write(pipe1[1], random_datas,sizeof random_datas);
        fflush(NULL);

        //printf("Szulo beirta az adatokat a csobe!\n");

        int answers[3][4];
        
        //sleep(5);
        //printf("Szulo elkezdi olvasni a csobol az adatokat!\n");
        read(pipe2[0],answers,sizeof answers);
        wait();

        printf("Eredmeny:\n");
        for(i=0;i<3;i++){
            printf("%d. %s - %s--%s - %d\n--%s - %d\n",i+1,data_array[i].question,ctime(&data_array[i].created),data_array[i].a,answers[i][0],data_array[i].b,answers[i][1]);
            if(data_array[i].answer_count>2)
                printf("--%s - %d\n",data_array[i].c,answers[i][2]);
            if(data_array[i].answer_count>3)
                printf("--%s - %d\n",data_array[i].d,answers[i][3]);
            printf("\n");
        }
        close(pipe1[1]);
        close(pipe2[0]);


	    //printf("Szulo befejezte!");	
	}
	exit(EXIT_SUCCESS);
}

int main(int argc, char** argv){
    char user_input[2]={0,0};
    int file=open("data.dat",O_RDONLY|O_CREAT,S_IRUSR|S_IWUSR);
    count = read(file,data_array,sizeof data_array)/sizeof data_array[0];
    close(file);
    while (strcmp(user_input,"6")){
        printf("Menu\n----------------------\n1.Kerdes felvetele\n2.Kerdes modositasa\n3.Kerdes torlese\n4.Kerdesek listazasa\n5.Veglegesites\n6.Kilepes\n");
        scanf("%s", user_input);
        if(!strcmp(user_input,"1")){
            add_question();
        } else if(!strcmp(user_input,"2")){
            modify_question();
        } else if(!strcmp(user_input,"3")){
            delete_question();
        } else if(!strcmp(user_input,"4")){
            list();
        } else if(!strcmp(user_input,"5")){
            finalize(argv);
        }
    }
    return 0;
}