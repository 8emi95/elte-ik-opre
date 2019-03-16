with Ada.Text_IO, Ada.Integer_Text_IO;
use Ada.Integer_Text_IO;

procedure hello is
 A: Positive := 3;
 B: Positive := 1;
 
 function Square (n : Integer) return Integer is 
 
 begin
	return n**2;
 end Square;
begin
	--Get(A);
	if A mod 3 = 0 then
		Ada.Text_IO.Put_Line("Valami");
	elsif A mod 3 = 1 then
		Put(5);
	elsif A mod 3 = 2 then
		Ada.Text_IO.Put_Line("Valami mas");
	end if;
	-- asdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasd
	for I in 2..10 loop
		b := b * I;
		put(Square(i));
	end loop;
	put(b);
end;