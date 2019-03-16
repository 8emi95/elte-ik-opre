with Rendez, Ada.Text_IO, Ada.Integer_Text_IO;
use Ada.Text_IO, Ada.Integer_Text_IO;

procedure Demo is
	type Tomb is array(Character range<>) of Integer;
	procedure Sajat_Rendez is new Rendez(Integer, Character, Tomb);
	T : Tomb := (3, 4, 6, 1, 2, 3);
	
begin
	Sajat_Rendez(T);
	
	for i in T'Range loop
		Put_Line(Integer'Image(i));
	end loop;
	
end;