with Ada.Text_IO;
use Ada.Text_IO;

procedure Szemafor is
	-- kritikus szakasz: egyszerre csak 1 task lehet kritikus
	-- Dijkstra-féle szemafor: bináris változat P/V
	
type pStr is access String; -- pointer
task type Printer(N : pStr := new String'(""));
	
task Semaphore is
	entry P;
	entry V;
end Semaphore;

task body Semaphore is -- PV sorozatokat fog elfogadni
begin
	loop
		accept P;
		accept V;
	end loop;
end Semaphore;

task body Printer is
	procedure Print(S : in String) is
	begin
		for I in S'Range loop
			Put(S(I));
		end loop;
		New_Line;
	end Print;
begin
	for I in 1..1000000 loop
		Semaphore.P; -- leengedem a sorompot
		Print(N.all & ": " & Integer'Image(i));
		Semaphore.V; -- felemelem a sorompot
		delay Duration(0);
	end loop;
end Printer;

Egyik : Printer(new String'("Egyik"));
Masik : Printer(new String'("Masik"));

begin
	null;
end Szemafor;