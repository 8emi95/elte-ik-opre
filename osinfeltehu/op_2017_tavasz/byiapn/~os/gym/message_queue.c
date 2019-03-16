parancssori parancsok:
ipcs: mqueue listázása
ipcrm -q[msqid]: msg törlése
ipcmk -Q: msg létrehozása

include <mqueue.h>

mq_open(név, mód) -> mqd_t típusú ha jó, -1 ha nem ( ell: mqd_t x = mq_open...; if(x == (mqd_t)-1)... )
mq_open(név, mód, engedélyek, &attrib)
	név: mindig /-el kezdõdik
	módok: O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_EXCL, O_NONBLOCK
	attribótumok: mq_attr (struct) { mq_maxmsg (max üzenetszám), mq_msgsize (üzenethossz byteokban) } vagy NULL: rsz. alapért. 
mq_send(üzenetleíró(mqd_t), &üzenet, mérete, &prioritás) -> 0, ha siker, -1, ha nem
	prioritás: unsigned int, minél nagyobb a szám annál hamarabb olvassa ki a sorból
mq_receive(üzenetleíró, &üzenet, maxméret, &prioritás[beleolvas, lehet NULL]) -> olvasott byto-ok száma, vagy -1
mq_close(üzenetleíró)
mq_unlink(név)
mq_notify(üzenetleíró, &signalevent(sigevent) )
			SI_MESGQ
példák:
mq_uzenet.c, mq_uzenet1.c

