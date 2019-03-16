#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

long copyContent( int src, long posIt, long posEnd, int dest ) {
	lseek(src,posIt,SEEK_SET);
	lseek(dest, 0, SEEK_END);
	
	const unsigned int bufferSize = 128;
	char buffer[bufferSize];
	while( posIt < posEnd ) {
		long size = read(src,&buffer,sizeof( MIN(bufferSize, posEnd - posIt ) ));
		if( size == 0 ) return;
		write(dest, &buffer, size);
		posIt += size;
	}
	
	return lseek(src,0,SEEK_CUR);
}


#define V_NEV_HOSSZ 200
#define V_MAIL_HOSSZ 200

struct Vendeg {
	char nev[V_NEV_HOSSZ];
	char mail[V_MAIL_HOSSZ];
	time_t datum;
};


#define R_NEV_HOSSZ 200

struct Rendezveny {
	char nev[R_NEV_HOSSZ];
	unsigned int vendegekSzama;
};

struct RendezvenyDesc {
	int fileDesc;
	char nev[120];
	unsigned int rendezvenyekSzama; 
};

long kos_seekRendezveny( struct RendezvenyDesc* rd, unsigned int rIdx ) {
	struct Rendezveny r;
	int pos = lseek(rd->fileDesc, sizeof(int), SEEK_SET);;
	int i;
	for ( i = 0; i < rIdx; ++i ) {
		read(rd->fileDesc,&(r),sizeof(r));
		pos = lseek(rd->fileDesc,sizeof( struct Vendeg) * r.vendegekSzama,SEEK_CUR);
	}
	return pos;
}

int kos_createDB( const char * const dbName ) {
	int fileDesc = open(dbName, O_RDWR | O_CREAT | O_EXCL, S_IREAD | S_IWRITE);
	if( fileDesc < 0 ) {
		return fileDesc;
	}
    
	int rendezvenyekSzama = 0;
	write(fileDesc, &rendezvenyekSzama, sizeof(rendezvenyekSzama));
	close(fileDesc);
	return 0;
}

struct RendezvenyDesc kos_openDB( const char * const dbName  ) {
	struct RendezvenyDesc ret;
	ret.fileDesc = open(dbName, O_RDWR );
	if( ret.fileDesc < 0 ) {
		ret.fileDesc = kos_createDB(dbName);
		if( ret.fileDesc < 0 ) return ret;		
		ret.fileDesc = open(dbName, O_RDWR );
	}
	
	lseek(ret.fileDesc,0,SEEK_SET);
	read(ret.fileDesc,&(ret.rendezvenyekSzama),sizeof(ret.rendezvenyekSzama));
	
	strncpy(ret.nev, dbName, 120);
	return ret;
}

void kos_rendezvenyListazas( struct RendezvenyDesc const * rd ) {
	lseek(rd->fileDesc,0,SEEK_SET);
	lseek(rd->fileDesc,sizeof(int),SEEK_CUR);
	
	int i = 0;
	struct Rendezveny r;	
	for ( i = 0; i < rd->rendezvenyekSzama; ++i ) {
		read(rd->fileDesc,&(r),sizeof(r));
		printf("%d. %s\n", i+1, r.nev);
		lseek(rd->fileDesc,sizeof( struct Vendeg) * r.vendegekSzama,SEEK_CUR);
	}
}
	
void kos_addRendezveny( struct RendezvenyDesc * rd, const char * nev ) {
	rd->rendezvenyekSzama++;
	lseek(rd->fileDesc,0,SEEK_SET);
	write(rd->fileDesc, &(rd->rendezvenyekSzama), sizeof(rd->rendezvenyekSzama));
	
	struct Rendezveny r;
	strncpy(r.nev, nev, R_NEV_HOSSZ);
	r.vendegekSzama = 0;
	
	lseek(rd->fileDesc,0,SEEK_END);
	write(rd->fileDesc, &r, sizeof(r));
}

int kos_removeRendezveny( struct RendezvenyDesc * rd, unsigned int rIdx ) {
	int tempFd = open("tempDB", O_RDWR | O_CREAT | O_EXCL, S_IREAD | S_IWRITE );
	if ( tempFd < 0 ) return tempFd;
	
	int ujRendezvenyekSzama = rd->rendezvenyekSzama - 1;
	write(tempFd, &ujRendezvenyekSzama, sizeof(ujRendezvenyekSzama));
	copyContent(rd->fileDesc, sizeof(ujRendezvenyekSzama), kos_seekRendezveny(rd, rIdx), tempFd );
	
	long end = lseek(rd->fileDesc,0,SEEK_END);
	copyContent(rd->fileDesc, kos_seekRendezveny(rd, rIdx+1), end, tempFd);
	
	close(rd->fileDesc);
	rename("tempDB", rd->nev);
	rd->fileDesc = tempFd;
	rd->rendezvenyekSzama = ujRendezvenyekSzama;
	return 0;
	
	
}

void kos_vendegListazas( struct RendezvenyDesc const * rd, unsigned int rIdx ) {
	lseek(rd->fileDesc,0,SEEK_SET);
	lseek(rd->fileDesc,sizeof(int),SEEK_CUR);
	
	kos_seekRendezveny(rd, rIdx);
	
	struct Rendezveny r;
	read(rd->fileDesc,&(r),sizeof(r));
	struct Vendeg v;
	int i;
	char dateBuff[30];
	for ( i = 0; i < r.vendegekSzama; ++i ) {
		read(rd->fileDesc,&(v),sizeof(v));
		//strftime(dateBuff, 25, "%Y-%m-%d %H:%M:%S", localtime(v.datum));
		printf("%d.%s\t%s\t%s\n", i+1, ctime(&(v.datum)), v.nev, v.mail);
	}
}

int kos_addVendeg( struct RendezvenyDesc * rd, unsigned int rIdx, const char * nev, const char * mail ) {
	int tempFd = open("tempDB", O_RDWR | O_CREAT | O_EXCL, S_IREAD | S_IWRITE );
	if ( tempFd < 0 ) return tempFd;
	
	int pos;
	pos = kos_seekRendezveny(rd, rIdx);
	copyContent(rd->fileDesc, 0, pos, tempFd);
	
	struct Rendezveny r;
	read(rd->fileDesc,&(r),sizeof(r));
	r.vendegekSzama++;
	write(tempFd, &(r), sizeof(r));
	
	struct Vendeg v;
	int i;
	for( i=0; i < r.vendegekSzama - 1; ++i ) {	
		read(rd->fileDesc,&(v),sizeof(v));
		write(tempFd, &(v), sizeof(v));
	}
		
	strncpy(v.nev, nev, V_NEV_HOSSZ);
	strncpy(v.mail, mail, V_MAIL_HOSSZ);
	time( &(v.datum) );
	write(tempFd, &(v), sizeof(v));
	
	pos = lseek(rd->fileDesc,0,SEEK_CUR);
	long end = lseek(rd->fileDesc,0,SEEK_END);
	copyContent(rd->fileDesc, pos, end, tempFd);
	
	close(rd->fileDesc);
	rename("tempDB", rd->nev);
	rd->fileDesc = tempFd;
	return r.vendegekSzama;
}

int kos_modVendeg( struct RendezvenyDesc * rd, unsigned int rIdx, unsigned int vIdx, char * nev, char * mail ) {
	
	kos_seekRendezveny(rd, rIdx);
	struct Rendezveny r;
	read(rd->fileDesc,&(r),sizeof(r));
	if( r.vendegekSzama <= vIdx ) return -1;
	int i;
	struct Vendeg v;
	for( i=0; i <= vIdx; ++i ) {
		read(rd->fileDesc,&(v),sizeof(v));
	}
	
	strncpy(v.nev, nev, V_NEV_HOSSZ);
	strncpy(v.mail, mail, V_MAIL_HOSSZ);
	lseek(rd->fileDesc, sizeof(v) * -1, SEEK_CUR);
	write(rd->fileDesc, &(v), sizeof(v));
	return 0;
}
	
int kos_removeVendeg( struct RendezvenyDesc * rd, unsigned int rIdx, unsigned int vIdx ) {
	int tempFd = open("tempDB", O_RDWR | O_CREAT | O_EXCL, S_IREAD | S_IWRITE );
	if ( tempFd < 0 ) return tempFd;
	
	int pos;
	pos = kos_seekRendezveny(rd, rIdx);
	copyContent(rd->fileDesc, 0, pos, tempFd);
	
	struct Rendezveny r;
	read(rd->fileDesc,&(r),sizeof(r));
	if( vIdx >= r.vendegekSzama ) {
		close(tempFd);
		remove("tempDB");
	}
	r.vendegekSzama--;
	write(tempFd, &(r), sizeof(r));
	
	struct Vendeg v;
	int i;
	for( i=0; i < r.vendegekSzama + 1; ++i ) {
		read(rd->fileDesc,&(v),sizeof(v));
		if( i != vIdx ) {
			write(tempFd, &(v), sizeof(v));
		}
	}
	
	pos = lseek(rd->fileDesc,0,SEEK_CUR);
	long end = lseek(rd->fileDesc,0,SEEK_END);
	copyContent(rd->fileDesc, pos, end, tempFd);
	
	close(rd->fileDesc);
	rename("tempDB", rd->nev);
	rd->fileDesc = tempFd;
	return 0;
}

void help() {
	printf("King of Stands, elektronikus látogatói nyilvántartás\n");
	printf("Argumentumok:\n");
	printf("	h\n\t   Kiirja ez a tajekoztatot.\n");
	printf("	rl\n\t   Kilistazza a rendezvenyek nevet es sorszamat.\n");
	printf("	ru [rendezveny_neve]\n\t   uj rendezvenyt vehetunk fel.\n");
	printf("	rt [rendezveny_sorszama]\n\t   torolhetjuk az adott sorszamu rendezvenyt, es az osszes hozza tartozo informaciot.\n");
	printf("	vl [rendezveny_sorszama]\n\t   kilistazhatjuk az adott sorszamu rendezvenyre jelentkezett vendegek adatait.\n");
	printf("	vu [rendezveny_sorszama] [nev] [email]\n\t   uj vendeget adhatunk hozza az adott sorszamu rendezvenyhez.\n");
	printf("	vm [rendezveny_sorszama] [vendeg_sorszama] [nev] [mail]\n\t   modosithatjuk az adott sorszamu rendezveny adott sorszamu vendegenek adatait.\n");
	printf("	vt [rendezveny_sorszama] [vendeg_sorszama]\n\t   torolhetjuk az adott sorszamu rendezveny adott sorszamu vendeget.\n");
}

int rendezvenyek_main(int argc, char** argv, struct RendezvenyDesc* rd) {
	kos_rendezvenyListazas(rd);
	return 0;
}

int ujrendezveny_main(int argc, char** argv, struct RendezvenyDesc* rd) {
	if( argc < 3 ) {
		printf("Rendezveny neve hianyzik..\n");
		return 1;
	}
	kos_addRendezveny(rd, argv[2]);
	return 0;
}

int rendezveny_torles_main(int argc, char** argv, struct RendezvenyDesc* rd) {
	if( argc < 3 ) {
		printf("Rendezveny sorszama hianyzik..\n");
		return 1;
	}
	unsigned int rIdx = atoi(argv[2]) - 1;
	if( rIdx >= rd->rendezvenyekSzama ) {
		printf("Helytelen sorszam..\n");
		return 1;
	}
	
	int err = kos_removeRendezveny(rd, rIdx);
	if( err != 0 ) {
		printf("Muvelet sikertelen..");
	}
	return err;
}

int vendegek_main(int argc, char** argv, struct RendezvenyDesc* rd) {
	if( argc < 3 ) {
		printf("Rendezveny sorszama hianyzik..\n");
		return 1;
	}
	unsigned int rIdx = atoi(argv[2]) - 1;
	if( rIdx >= rd->rendezvenyekSzama ) {
		printf("Helytelen sorszam..\n");
		return 1;
	}
	kos_vendegListazas(rd, rIdx);
	return 0;
}

int ujvendeg_main(int argc, char** argv, struct RendezvenyDesc* rd) {
	if( argc < 5 ) {
		printf("Parameterek hianyoznak..\n");
		return 1;
	}
	unsigned int rIdx = atoi(argv[2]) - 1;
	if( rIdx >= rd->rendezvenyekSzama ) {
		printf("Helytelen sorszam..\n");
		return 1;
	}
	
	int ret = kos_addVendeg(rd, rIdx, argv[3], argv[4]);
	if( ret < 0 ) {
		printf("Ismeretlen hiba..\n");
		return ret;
	} else {
		printf("Sikeres regisztracio. Sorszam: %d.\n", ret);
		
		return 0;
	}
}

int vendeg_modositas_main(int argc, char** argv, struct RendezvenyDesc* rd) {
	if( argc < 6 ) {
		printf("Parameterek hianyoznak..\n");
		return 1;
	}
	unsigned int rIdx = atoi(argv[2]) - 1;
	if( rIdx >= rd->rendezvenyekSzama ) {
		printf("Helytelen rendezveny sorszam..\n");
		return 1;
	}
	
	unsigned int vIdx = atoi(argv[3]) - 1;
	int err = kos_modVendeg(rd, rIdx, vIdx, argv[4], argv[5]);
	if( err != 0 ) {
		printf("Helytelen vendeg sorszam..\n");
	}
	return err;
}

int vendeg_torles_main(int argc, char** argv, struct RendezvenyDesc* rd) {
	if( argc < 4 ) {
		printf("Parameterek hianyoznak..\n");
		return 1;
	}
	unsigned int rIdx = atoi(argv[2]) - 1;
	if( rIdx >= rd->rendezvenyekSzama ) {
		printf("Helytelen rendezveny sorszam..\n");
		return 1;
	}
	
	unsigned int vIdx = atoi(argv[3]) - 1;	
	return kos_removeVendeg(rd, rIdx, vIdx);
}


int main(int argc,char** argv) {
	
	if(argc < 2 || strcmp(argv[1], "help") == 0 ) {
		help();
		return 0;
	}
	
	struct RendezvenyDesc dbDesc = kos_openDB("adatok");
	int ret;
	if( argc >= 2 && strcmp(argv[1], "rl") == 0 ) {
		ret = rendezvenyek_main(argc, argv, &dbDesc);
	} 
	else if ( argc >= 2 && strcmp(argv[1], "ru") == 0 ) {
		ret = ujrendezveny_main(argc, argv, &dbDesc);
	}
	else if ( argc >= 2 && strcmp(argv[1], "rt") == 0 ) {
		ret = rendezveny_torles_main(argc, argv, &dbDesc);
	}
	else if ( argc >= 2 && strcmp(argv[1], "vl") == 0 ) {
		ret = vendegek_main(argc, argv, &dbDesc);
	}
	else if ( argc >= 2 && strcmp(argv[1], "vu") == 0 ) {
		ret = ujvendeg_main(argc, argv, &dbDesc); 
	}
	else if ( argc >= 2 && strcmp(argv[1], "vm") == 0 ) {
		ret = vendeg_modositas_main(argc, argv, &dbDesc);
	}
	else if ( argc >= 2 && strcmp(argv[1], "vt") == 0 ) {
		ret = vendeg_torles_main(argc, argv, &dbDesc);
	}
	else {
		printf("Ismeretlen argumentum..\n\n");
		help();
	}
	
	close(dbDesc.fileDesc);
	return ret;
}
