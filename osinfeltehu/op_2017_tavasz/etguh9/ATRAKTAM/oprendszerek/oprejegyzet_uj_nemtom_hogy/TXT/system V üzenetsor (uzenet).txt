ÜZENETSOR
(nem posX-es)



-> STRUKTÚRA:

	struct uzenet
	{
		long mtype;
			//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
		char mtext[1024];
			//Ez lehet pl. egy int is... (??)
	};



-> ÜZENETSOR LÉTREHOZÁSA:

	int uzenetsor;
	key_t kulcs; 

	kulcs = ftok(argv[0],1); 

	uzenetsor = msgget(kulcs, 0600 | IPC_CREAT); 
	if ( uzenetsor < 0 )
		printf("HIBA\n");
		

	
-> ÜZENETSOR TÖRLÉSE:

Adott:
- int uzenetsor  -  üzenetsor azonosító

	int status;

	status = msgctl(uzenetsor, IPC_RMID, NULL);
	if ( status < 0 )
		printf("HIBA\n");



-> ÜZENET KÜLDÉSE:

Adott:
- int uzenetsor  -  üzenetsor azonosító

	const struct uzenet uz = { 5, "Hajra Fradi!" }; 
	int status; 
     
    status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
		// a 3. param ilyen is lehet: sizeof(uz.mtext)
		// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     	// a 4. paraméter: mit csináljon ha valami nem sikerült (????)
	if (status < 0)
		printf("HIBA\n");

		
     
-> ÜZENET FOGADÁSA:

Adott:
- int uzenetsor  -  üzenetsor azonosító

	struct uzenet uz; 
	int status;
	
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 );
		//az utolso parameter(0) az uzenet azonositoszama
		//ha az 0, akkor a sor elso uzenetet vesszuk ki
		//ha > 0 (5), akkor az 5-os uzenetekbol a kovetkezot vesszuk ki a sorbol
		//4. paraméter: várjon-e üzenetre (0), vagy menjen tovább, ha nincs üzenet a sorban
     
	 if (status < 0)
		 printf("HIBA\n");
     else
		 printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext );