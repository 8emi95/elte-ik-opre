�ZENETSOR
(nem posX-es)



-> STRUKT�RA:

	struct uzenet
	{
		long mtype;
			//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
		char mtext[1024];
			//Ez lehet pl. egy int is... (??)
	};



-> �ZENETSOR L�TREHOZ�SA:

	int uzenetsor;
	key_t kulcs; 

	kulcs = ftok(argv[0],1); 

	uzenetsor = msgget(kulcs, 0600 | IPC_CREAT); 
	if ( uzenetsor < 0 )
		printf("HIBA\n");
		

	
-> �ZENETSOR T�RL�SE:

Adott:
- int uzenetsor  -  �zenetsor azonos�t�

	int status;

	status = msgctl(uzenetsor, IPC_RMID, NULL);
	if ( status < 0 )
		printf("HIBA\n");



-> �ZENET K�LD�SE:

Adott:
- int uzenetsor  -  �zenetsor azonos�t�

	const struct uzenet uz = { 5, "Hajra Fradi!" }; 
	int status; 
     
    status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
		// a 3. param ilyen is lehet: sizeof(uz.mtext)
		// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     	// a 4. param�ter: mit csin�ljon ha valami nem siker�lt (????)
	if (status < 0)
		printf("HIBA\n");

		
     
-> �ZENET FOGAD�SA:

Adott:
- int uzenetsor  -  �zenetsor azonos�t�

	struct uzenet uz; 
	int status;
	
	status = msgrcv(uzenetsor, &uz, 1024, 5, 0 );
		//az utolso parameter(0) az uzenet azonositoszama
		//ha az 0, akkor a sor elso uzenetet vesszuk ki
		//ha > 0 (5), akkor az 5-os uzenetekbol a kovetkezot vesszuk ki a sorbol
		//4. param�ter: v�rjon-e �zenetre (0), vagy menjen tov�bb, ha nincs �zenet a sorban
     
	 if (status < 0)
		 printf("HIBA\n");
     else
		 printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext );