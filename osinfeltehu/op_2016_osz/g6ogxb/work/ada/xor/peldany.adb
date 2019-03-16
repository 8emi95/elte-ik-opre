with ada.text_IO, mf;
use ada.text_IO;

procedure peldany is
	function lgy is new mf(character, positive, String);
	s1:string:="";
	s2:string:="a";
	s3:string:="ab";
	s4:string:="aabc";
	s5:string:="abbc";
	s6:string:="abcc";
begin
put_line(character'image(lgy(s2)));
put_line(character'image(lgy(s3)));
put_line(character'image(lgy(s4)));
put_line(character'image(lgy(s5)));
put_line(character'image(lgy(s6)));
--put_line(lgy(s2));
--put_line(lgy(s3));
--put_line(lgy(s4));
--put_line(lgy(s5));
--put_line(lgy(s6));
end;


