function max_index(T : in Tomb) return Index is
	M: Index := T'First;
begin
	for I in T'Range loop
		if T(M) < T(I) then
			M:=I;
		end if;
	end loop;
	return M;
end max_index;