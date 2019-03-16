with Ada.Numerics.Discrete_Random, Ada.Text_IO;
use Ada.Text_IO;

procedure Main1 is
	
	package Integer_IO_Pkg is new Integer_IO(Integer);
	use Integer_IO_Pkg;

	package Random_Pkg is new Ada.Numerics.Discrete_Random(Integer);
	use Random_Pkg;

F : File_Type;
G : Generator;
I : Integer;

begin
	Reset(G);
	I := Random(G);
	Put(Integer'Image(I));
	
	Create(F, Name => "egeszek.txt"); -- Opening File
	Put(F, Integer'Image(I));
	New_Line;
	Close(F); -- Closing File
	
	Open(F, Name => "egeszek.txt", Mode => In_File); -- Opening File
	
	while not End_Of_File(F) loop
		Get(F, I);
		Put(Integer'Image(I));
	end loop;
	
	Close(F); -- Closing File
	
end Main1;