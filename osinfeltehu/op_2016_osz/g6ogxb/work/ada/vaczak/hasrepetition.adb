function HasRepetition (T: in vector) return boolean is
	--b:boolean := false;
begin
	if t'length>1 then
		for i in T'first..index'pred(t'last) loop
			if t(i)=t(index'succ(i)) then
				return true;
			end if;
		end loop;
	end if;
	return false;
end;