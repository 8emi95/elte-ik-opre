with reversal, ada.text_IO, ada.integer_text_io, ada.float_text_io;
use ada.integer_text_io;
procedure forditdemo is
	type tomb is array(integer range <>) of integer;
	type ftomb is array(integer range <>) of float;
	procedure Cserel ( A, B: in out integer) is
		Tmp: integer := A;
	begin
		A := B;
		B := Tmp;
	end Cserel;
	procedure fCserel ( A, B: in out float) is
		Tmp: float := A;
	begin
		A := B;
		B := Tmp;
	end fCserel;
	--procedure csere is new cserel(integer);
	--procedure fcsere is new cserel(float);
	procedure freverse is new reversal(float, integer, ftomb, fcserel);
	procedure intreverse is new reversal(integer,integer, tomb, cserel);
	it:tomb:=(1,2,3,4,5);
	ft:ftomb:=(1.0,2.0,3.0,4.0,5.0);
	--i:integer;
begin
	intreverse(it);
	freverse(ft);
	for i in it'range loop
		put(it(i));
	end loop;

	for i in ft'range loop
		ada.float_text_io.put(ft(i));
	end loop;
end forditdemo;