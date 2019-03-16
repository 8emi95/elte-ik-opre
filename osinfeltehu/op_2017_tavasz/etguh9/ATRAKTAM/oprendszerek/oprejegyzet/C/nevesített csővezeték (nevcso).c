NEVESÍTETT CSŐVEZETÉK



-> LÉTREHOZÁS:

    int fid = mkfifo("fifo.ftc", S_IRUSR|S_IWUSR );
		vagy:
	int fid = mkfifo("namedpipe", 0600);
		//creating named pipe file
		//S_IWGRP, S_IROTH (other jog), file permission mode
		//the file name: fifo.ftc
		//the real fifo.ftc permission is: mode & ~umask 
    if (fid == -1)
		printf("HIBA\n");    

	

-> MEGNYITÁS + ÍRÁS/OLVASÁS:

	int fd;

	printf("Csonyitas eredmenye: %d!\n", fid);
	
	fd = open("fifo.ftc", O_WRONLY);
	write(fd, "Hajra Fradi!\n", 12);
	
	fd = open("fifo.ftc",O_RDONLY);
	read(fd, s, sizeof(s));
	
	close(fd);
	
	
	
-> TÖRLÉS:

	unlink("fifo.ftc");
		//remove fifo.ftc