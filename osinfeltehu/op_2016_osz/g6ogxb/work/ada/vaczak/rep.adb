with Ada.Text_IO, hasrepetition;
use ada.text_IO;

procedure rep is
function HDL is new hasrepetition(character,positive,string);
s1:String:="almafa";
s2:String:="aaba";
s3:String:="abaa";
s4:String:="abba";
s5:String:="";
s6:String:="a";
begin

put_line(boolean'image(hdl(s1)));
put_line(boolean'image(hdl(s2)));
put_line(boolean'image(hdl(s3)));
put_line(boolean'image(hdl(s4)));
put_line(boolean'image(hdl(s5)));
put_line(boolean'image(hdl(s6)));
end;