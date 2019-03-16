POSIX ÜZENETSOR



//mq_open, mq_send,mq_receive, mq_unlink

//mq_send, mq_receive uses char array



-> LÉTREHOZ:
	
	char* mqname = "/baba1";   //mqname must start with / !!!!!
	
	struct mq_attr attr;
	mqd_t mq1;
	
	attr.mq_maxmsg = 5;   //MAXMSGS
	attr.mq_msgsize = 50;   //MSGSIZE
	
	mq_unlink(mqname);   // delete if exist !!!!
	mq1 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
	if (mq1 < 0)
		printf("HIBA\n");
	
	 

-> ÜZENETKÜLDÉS:

Adott:
- mqd_t mq1  -  MQ azonosítója(?)
- "char array" uzenet   (Pl.: char uzenet[30] = "Hajrá Fradi!";)
	
	int db = mq_send(mq1, uzenet, 17, 5 );
	//sleep(1); // ha ez nincs a fogad be is olvassa!!!!
	mq_send(mq1, "Hajra Debrecen!", 20, 4);
	//mq_receive(mq1, buffer, strlen(buffer), 0); // get the first message
	printf("Szulo: uzenet elkuldve!%d\n", db);
	   

	  
-> ÜZENET FOGADÁS:
	  
Adott:
- mqd_t mq1  -  MQ azonosítója(?)
- "char array" uzenet   (Pl.: char uzenet[30] = "Hajrá Fradi!";)

	int db = mq_receive(mq1, buffer, 54, NULL);
		// 5 is the message priority

	/*mqd_t mq2;
	mq2 = mq_open(mqname, O_RDWR);*/
	
	printf("mq_receive : %s, olvasott bájtok hossza: %d errno: %s\n", buffer, db, strerror(errno));
	db = mq_receive(mq1, buffer, 50, 0);
	
	
	//mq_send(mq1,"Gyerek befejezte!", 20,6);
	//int uzenetsor;
	//msgsnd(uzenetsor,&valasz,sizeof(struct uzenet),0);
	


-> BEZÁRÁS, TÖRLÉS (??):

Adott:
- char* mqname  -  MQ neve
- mqd_t mq1  -  MQ azonosítója(?)

	mq_close(mq1);
	mq_unlink(mqname);   (// delete if exist)