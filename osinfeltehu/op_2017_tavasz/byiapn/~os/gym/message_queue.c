parancssori parancsok:
ipcs: mqueue list�z�sa
ipcrm -q[msqid]: msg t�rl�se
ipcmk -Q: msg l�trehoz�sa

include <mqueue.h>

mq_open(n�v, m�d) -> mqd_t t�pus� ha j�, -1 ha nem ( ell: mqd_t x = mq_open...; if(x == (mqd_t)-1)... )
mq_open(n�v, m�d, enged�lyek, &attrib)
	n�v: mindig /-el kezd�dik
	m�dok: O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_EXCL, O_NONBLOCK
	attrib�tumok: mq_attr (struct) { mq_maxmsg (max �zenetsz�m), mq_msgsize (�zenethossz byteokban) } vagy NULL: rsz. alap�rt. 
mq_send(�zenetle�r�(mqd_t), &�zenet, m�rete, &priorit�s) -> 0, ha siker, -1, ha nem
	priorit�s: unsigned int, min�l nagyobb a sz�m ann�l hamarabb olvassa ki a sorb�l
mq_receive(�zenetle�r�, &�zenet, maxm�ret, &priorit�s[beleolvas, lehet NULL]) -> olvasott byto-ok sz�ma, vagy -1
mq_close(�zenetle�r�)
mq_unlink(n�v)
mq_notify(�zenetle�r�, &signalevent(sigevent) )
			SI_MESGQ
p�ld�k:
mq_uzenet.c, mq_uzenet1.c

