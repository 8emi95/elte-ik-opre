#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "types.h"


int main() {
    vector v;
    vector_init(&v);
/*    
    int a;
    a=1;
    int b;
    b=2;
    int c;
    c=3;

    vector_add(v,&a);
    vector_add(v,&b);
    vector_add(v,&c);

    printf("%d\n", v->count);

    for (int i = 0; i<v->count; i++)
        printf("%d\n", *(int*)vector_get(v,i));
    
    vector_delete(v,1);

    printf("New count: %d\n", v->count);

    vector_add(v,&a);
    vector_add(v,&b);
    vector_add(v,&c);

    c=8;

    for (int i = 0; i<v->count; i++)
        printf("%d\n", *(int*)vector_get(v,i));

    printf("Vector capacity: %d", v->capacity);

//    char*  name = "Kaki klubb"; 
 //   Event e = *event_init(name);*/
    


    return 0;

}

