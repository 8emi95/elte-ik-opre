
# Compiler variables
CCFLAGS = -Wall

# Rule to link object code files to create executable file
mlsz: main.o
	g++ $(CCFLAGS) -o mlsz main.o

# Rules to compile source code files to object code
assign3.o: main.c
	g++ $(CCFLAGS) -c main.c

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o mlsz
