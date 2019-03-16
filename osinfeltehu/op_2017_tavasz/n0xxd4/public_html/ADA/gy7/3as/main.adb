with Maps, Ada.Text_IO, Ada.Integer_Text_IO, Ada.Strings;
use Ada.Text_IO, Ada.Integer_Text_IO, Ada.Strings;

procedure Main is
	package myMap is new Maps(Integer, Character);
	use myMap;
	
M : Map(10);
Found : Boolean := false;
begin
	-- Test Put-Size
	Put_Line(Integer'Image(Size(M))); -- 0
	
	Put(M, 2, 'C');
	Put(M, 3, 'B');
	Put(M, 20, 'L');
	
	Put_Line(Integer'Image(Size(M))); -- 3
	
	-- Test Contains
	Found := Contains(M, 10);
	if Found then Put_Line("True");
	else Put_Line("False");
	end if; -- false
	
	Found := Contains(M, 2);
	if Found then Put_Line("True");
	else Put_Line("False");
	end if; -- true
	
	-- Test Get
	Put(Get(M, 3)); -- 'B'
	new_line;
	Put(Get(M, 20)); -- 'L'
	new_line;
	Put(Get(M, 2)); -- 'C'
	new_line;
	
end Main;
