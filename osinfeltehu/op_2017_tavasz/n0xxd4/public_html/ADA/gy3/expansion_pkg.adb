package Body Expansion_pkg is
	
	procedure FillVectors_size3(v1, v2 : in out Vector) is
	begin
		Put("Elso vektor elemei: ");
		for i in 1..3 loop
			Get(v1(i));
		end loop;
		
		Put("Masodik vektor elemei: ");
		for i in 1..3 loop
			Get(v2(i));
		end loop;
	end FillVectors_size3;
	
	procedure FillVector_size8(v : in out Vector) is
	begin
		Put("Vektor elemei: ");
		for i in 1..8 loop
			Get(v(i));
		end loop;
	end FillVector_size8;
	
	function DotProd_size3(v1, v2 : in Vector) return Integer is
	Sum : Integer := 0;
	begin
		for i in 1..3 loop
			Sum := Sum + v1(i) * v2(i);
		end loop;
		return Sum;
	end DotProd_size3;
	
	function FindMax_size8(v : in Vector) return Integer is
	maxIndex : Integer := 1;
	begin
		for i in 2..8 loop
			if v(i) > v(maxIndex) then maxIndex := i;
			end if;
		end loop;
		
		return v(maxIndex);
	end FindMax_size8;
	
end Expansion_pkg;