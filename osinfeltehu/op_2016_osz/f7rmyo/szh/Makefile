
all : client server

client   : simple_client.c ; gcc -Wall simple_client.c common.c -o client
server   : simple_server.c ; gcc -Wall simple_server.c common.c -o server

clean	 : ; rm client server
