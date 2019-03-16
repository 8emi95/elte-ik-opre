with Ada.Integer_Text_IO, boolean_text_IO, Ada.Text_IO, Ada.float_text_IO;
use Ada.Integer_Text_IO;
with mat;



procedure projekt1 is
procedure listperfs(n: natural) is
begin
	ada.text_IO.put_line("");
	for i in 1..n loop
		if mat.isperfect(i) then put(i);
		end if;
	end loop;
end listperfs;
	A,B,C:Natural;
begin
	A:=1;
	b:=5;
	c:=3;
	Put(mat.max2(a,b));
	put(mat.max3(a,b,c));
	put(mat.sumn(5));
	put(mat.signx(-11));
	put(mat.signx(11));
	put(mat.signx(0));
	ADA.Float_text_IO.put(mat.pi(8000));
	boolean_text_IO.put(mat.mod2check(2));
	Ada.text_IO.put_line("");
	ADA.Float_text_IO.put(mat.euler(10));
	put(mat.digitsum(12345));
	Ada.text_IO.put_line("");
	Ada.text_IO.put_line("");
	boolean_text_IO.put(mat.d9(9));
	boolean_text_IO.put(mat.d9(27));
	boolean_text_IO.put(mat.d9(18));
	boolean_text_IO.put(mat.d9(93));
	Ada.text_IO.put_line("");
	listperfs(10000);
end projekt1;