with Ada.Integer_Text_IO, Ada.Text_IO;
use Ada.Integer_Text_IO;

procedure lnko is
	a, b: Positive;
begin
	Ada.Text_IO.Put_Line("LNKO Szamitas, kerem az egyik szamot");
	get(A);
	Ada.Text_IO.Put_Line("kerem a masik szamot");
	get(B);
	while a /= b loop
		if a>b then
			a:=a-b;
		else
			b:=b-a;
		end if;
	end loop;
	Ada.Text_IO.Put_Line("Az LNKO: ");
	put(A);
end lnko;