king_of_stands: linked_list.o visitor.o visitor_list.o event.o event_list.o king_of_stands.c
	gcc -W -Wall -o king_of_stands linked_list.o visitor.o visitor_list.o event.o event_list.o king_of_stands.c

visitor.o: visitor.c
	gcc -W -Wall -c -o visitor.o visitor.c

event.o: event.c
	gcc -W -Wall -c -o event.o event.c

visitor_list.o: visitor_list.c
	gcc -W -Wall -c -o visitor_list.o visitor_list.c

event_list.o: event_list.c
	gcc -W -Wall -c -o event_list.o event_list.c

linked_list.o: linked_list.c
	gcc -W -Wall -c -o linked_list.o linked_list.c

clean:
	rm *.o
