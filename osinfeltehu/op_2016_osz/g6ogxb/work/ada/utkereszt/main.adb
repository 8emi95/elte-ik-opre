with ada.text_IO; use ada.text_IO;
with eszkozok; use eszkozok;
procedure main is
	type pstring is access string;
	type pduration is access duration;
	task type auto (p:pstring; d:pduration);
	task body auto is
		b:boolean:=false;
		begin
			put_line("elindut");
			delay d.all;
			put_line("...");
			while not b loop
				select
					jelzolampa.athalad;
					put(p.all);put_line(" atment"); b:=true;
				or delay 0.2; put_line("var a lampanal");
				end select;
			end loop;
	end auto;
	type pauto is access auto;
	autok:array(1..3) of pauto;
	p:pstring:=new string'("abc1");
	d:pduration:=new duration'(1.5);
begin
	autok(1):=new auto(p,d); delay(0.2);
	p:= new string'("abc2");
	autok(2):=new auto(p,d); delay(0.2);
	p:= new string'("abc3");
	autok(3):=new auto(p,d); delay(0.2);
	null;
end main;