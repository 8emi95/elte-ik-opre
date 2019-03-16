function Fact(n : Natural) return Natural is
begin
	if (n <= 0) then return 1;
	else return n * Fact(n - 1);
	end if;
end Fact;
