with Ada.Integer_Text_IO, Ada.Text_IO, isort;
use Ada.Integer_Text_IO;

-- Integer'Image(2345) = "2345"
--Natural'Pred(0) = -1:Integer



procedure p2 is
	procedure Szummazas  is

    type Index is new Integer;
    type Elem is new Integer;
    type Tomb is array (Index range <>) of Elem;

    function Szumma ( T: Tomb ) return Elem is
        S: Elem := 0;
    begin
        if T'Length = 0 then return 0;
		else
			return T(T'First)+Szumma(T(Index'Succ(T'First)..T'Last));
		end if;
    end Szumma;

begin

    Ada.Text_IO.Put_Line( Elem'Image( Szumma((3,2,5,7,1)) ) );

end Szummazas;
	
	
	T:isort.Tomb := (3,2,5,7,1);
	a:natural;
begin
	A:=0;
	put(Natural'pred(A));
	szummazas;
	
	
	isort.rendez(T);
	for i in t'first..t'last loop
		ada.text_io.put(isort.Elem'Image(T(i)));
	end loop;
	
end p2;