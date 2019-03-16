os_bead1.run : os_bead1.c
	cd tmp
	gcc -O2 -std=c99 -Wall -pedantic -pedantic-errors -o os_bead1.run os_bead1.c
	cd ..
tmp/os_bead1.o : os_bead1.c
	gcc -m64 -O2 -std=c99 -Wall -pedantic -pedantic-errors os_bead1.c -o tmp/os_bead1.o

tmp/registry_entry.o : tmp/struct_io.o tmp/read_helpers.o
	gcc -m64 -O2 -std=c99 -Wall -pedantic -pedantic-errors registry_entry.h -o tmp/registry_entry.o
tmp/struct_io.o : struct_io.h
	gcc -m64 -O2 -std=c99 -Wall -pedantic -pedantic-errors struct_io.h -o tmp/struct_io.o

tmp/read_helpers.o : read_helpers.h
	gcc -m64 -O2 -std=c99 -Wall -pedantic -pedantic-errors read_helpers.h -o tmp/read_helpers.o

upload :
	scp -rp ./* n5haim@os.inf.elte.hu:~/os_bead1/

clean :
	rm ./*~
	rm ./tmp/*
