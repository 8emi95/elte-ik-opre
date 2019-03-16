with Ada.Text_IO, Ada.Integer_Text_IO, Ada.Command_Line;
use Ada.Text_IO, Ada.Integer_Text_IO, Ada.Command_Line;

Procedure Main is
--Nap: Integer;
Draw : Boolean;

	begin
	-- 2-es feladat ----------------------------------------------
		--Put_Line("Nap sorszama: ");
		--Get(Nap);
		
		-- Megoldas if-else
		--if Nap >= 1 and Nap <= 5 then Put_Line("Hetkoznap.");
		--elsif Nap = 6 or Nap = 7 then Put_Line("Hetvege.");
		--else Put_Line("Hibas sorszam.");
		--end if;
		
		--case Nap is
			--when 1..5 => Put_Line("Hetkoznap.");
			--when 6 | 7 => Put_Line("Hetvege.");
			--when others =>	Put_Line("Hibas sorszam.");
		--end case;
		
	-- 3-as feladat ----------------------------------------------
		
		for i in 1..12 loop
			for j in 1..18 loop
				Draw := false;
				
				-- "+"
				if (i = 1 and j = 1) or (i = 1 and j = 18)
				or (i = 12 and j = 1) or (i = 12 and j = 18)
				then Put("+"); Draw := true;			
				end if;
				
				-- "-"
				if (i = 1 and j /= 1 and j /= 18)
				or (i = 12 and j /= 1 and j /= 18)
				then Put("-"); Draw := true;
				end if;
				-- "|"
				if (i >= 2 and i <= 11 and (j = 1 or j = 18))
				then Put("|"); Draw := true;
				end if;
				
				if(Draw)
				
				-- odd line
				
				-- even line
				
			end loop;
			
			-- empty line
				if (i = 1 or i = 11) then Put_Line("");
				end if;
			-- new line
			Put_Line("");
		end loop;
		
	-- 4-es feladat ----------------------------------------------
		--Put_Line("A program neve: " & Command_Name);
		--for i in 1..Argument_Count loop
			--Put_Line("A(z) " & Integer'Image(i) & ". parameter: " & Argument(i));
		--end loop;
			
	end Main;
	
-- 18*12