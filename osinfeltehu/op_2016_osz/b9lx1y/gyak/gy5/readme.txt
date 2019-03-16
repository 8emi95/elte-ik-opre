ipcs
ipcrm -q <msqid>

a messagequeue-k kozott a regiben cimezni lehetett, mig az ujabb posixosban
pedig feliratokozni lehet

osztott memoria
minden folzamat amelzik rendelkezik egz megosztott kulcssal ay fel tud csatlakozni (ftok nak kell egy letezo file neve, aki ismeri a file nevet az hozza tud ferni az ftok segitsegevel)

shmget keri le az osztott memoriat, letre is hozza ha kerjuk
shmat kapcsolodik hozza
char *s az osztott memoria
shmdat elengedi a memoriat

