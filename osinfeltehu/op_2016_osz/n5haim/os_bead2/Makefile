normal : os_bead2.c
	gcc -D_POSIX_C_SOURCE -O2 -std=c99 -pedantic -pedantic-errors -o os_bead2.run os_bead2.c

os_bead2.run : os_bead2.o registry_entry.o event_entry.o struct_io.o read_helpers.o fork.o
	gcc -D_POSIX_SOURCE -O2 -std=c99 -pedantic -pedantic-errors -o os_bead2.run os_bead2.o registry_entry.o struct_io.o read_helpers.o event_entry.o registry_entry.o fork.o

os_bead2.o : os_bead2.c registry_entry.o event_entry.o struct_io.o read_helpers.o fork.o
	gcc -D_POSIX_SOURCE -O2 -c -std=c99 -pedantic -pedantic-errors os_bead2.c -o os_bead2.o

registry_entry.o : registry_entry.h registry_entry.h struct_io.o read_helpers.o
	gcc -D_POSIX_SOURCE -O2 -c -std=c99 -pedantic -pedantic-errors registry_entry.h -o registry_entry.o

event_entry.o : event_entry.h event_entry.h struct_io.o read_helpers.o
	gcc -D_POSIX_SOURCE -O2 -c -std=c99 -pedantic -pedantic-errors event_entry.h -o event_entry.o

struct_io.o : struct_io.h struct_io.h
	gcc -D_POSIX_SOURCE -O2 -c -std=c99 -pedantic -pedantic-errors struct_io.h -o struct_io.o

read_helpers.o : read_helpers.h
	gcc -D_POSIX_SOURCE -O2 -c -std=c99 -pedantic -pedantic-errors read_helpers.h -o read_helpers.o

fork.o : fork.h
	gcc -D_POSIX_SOURCE -O2 -c -std=c99 -pedantic -pedantic-errors fork.h -o fork.o
