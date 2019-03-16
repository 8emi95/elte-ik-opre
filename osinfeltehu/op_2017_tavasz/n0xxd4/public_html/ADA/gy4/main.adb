with Generikus_Vermek, Ada.Text_IO, Ada.Integer_Text_IO;
use Ada.Text_IO, Ada.Integer_Text_IO;

procedure Main is

	package Integer_Verem is new Generikus_Vermek(Integer);
	use Integer_Verem;

	procedure Menu_Kiir is
	begin
		new_line;
		Put_Line("Elem beszurasa: 1");
		Put_Line("Elem kivetele: 2");
		Put_Line("Legfelso elem lekerdezese: 3");
		Put_Line("Kilepes: 4");
		Put("-----Valasztando menupont: ");
	end Menu_Kiir;
	
V : Verem(5);
E : Integer;
Valasztas : Integer range 1..4;
begin
	loop
	Menu_Kiir;
	Get(Valasztas);
	new_line;
	
	case Valasztas is
	when 1 =>
		Put("Beszurando elem: ");
		Get(E);
		Push(V, E);
	when 2 => Pop(V);
	when 3 => 
		Put("Legfelso elem erteke: " & Integer'Image(Top(V)));
		new_line;
	when 4 => exit;
	end case;
	end loop;
end Main;
