with Ada.Text_IO; use Ada.Text_IO; with Ada.Numerics.Discrete_Random;


procedure Csirke is

MEGPUSZTULOK:Exception;
Csibe_Elszallt_Lany_Szomoru:Exception;

subtype coin is Integer range 1..2;
package coinflip is new Ada.Numerics.Discrete_Random(coin);
use coinflip;
G:Generator;
--C:Coin;

task csibe is
	entry etet;
	entry jatek;
	
end csibe;

task body csibe is
	begy:Integer:=4;
	unalom:Integer:=6;
begin
	Put_Line(begy'Image);
	while 1=1 loop
		
		begy:=begy-1;
		unalom:=unalom-1;
		
		if begy<=0 or unalom<=0 then
			Put_Line("MEGPUSZTULOK");
			raise MEGPUSZTULOK;
		elsif begy>=30 then 
			Put_Line("Kislany elszomorodott, Csibe felnott es elrepult");
			raise Csibe_Elszallt_Lany_Szomoru;
		end if;
		
		
		
		select
			accept etet do
				begy:=begy+4;
			end etet;
		or
			accept jatek do
				unalom:=6;
			end jatek;
		or
			delay 1.0;
			
			Put("Begy"); Put(begy'Image);
			Put(" Unalom"); Put_Line(unalom'Image);
		end select;
	end loop;
end csibe;

task gazda is
	
end gazda;

task body gazda is
	state:Coin:=1;
begin
	while 1=1 loop
		delay 1.0;
		state:=Random(G);
		if state=1 then Csibe.etet;
		else csibe.jatek;
		end if;
	end loop;
end gazda;
begin
ada.text_IO.Put_Line("Kezdodik");
end Csirke;