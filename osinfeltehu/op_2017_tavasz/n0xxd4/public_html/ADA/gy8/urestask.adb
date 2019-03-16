with Ada.Text_IO; use Ada.Text_IO;

procedure UresTask is
Task Egyik;
Task Masik;
	
	Task body Egyik is
	begin
		for I in 1..100 loop
			Put_Line("Egyik");
		end loop;
	end Egyik;
	
	Task body Masik is
	begin
		for I in 1..100 loop
			Put_Line("Masik");
		end loop;
	end Masik;

begin
	null;
end Urestask;
