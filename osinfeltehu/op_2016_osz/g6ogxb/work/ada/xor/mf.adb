function mf(v:vector) return elem is
	type mult is array (v'range) of natural;
	--function multiplicity(v:in vector; e:elem) return natural is
		--c:natural:=0;
	--begin
		--for i in v'range loop
			--if v(i)=e then
				--c:=c+1;
			--end if;
		--end loop;
		--return c;
	--end multiplicity;
	
	multiplicity:mult;
	r:index:=v'first;
	temp:natural:=0;
begin
	for i in multiplicity'range loop
		multiplicity(i):=0;
	end loop;
	if v'length = 1 then return v(v'first);
	else
		multiplicity(v'first):=1;
		for i in index'succ(v'first)..v'last loop
			r:= index'pred(i);
			while r/=v'first loop
				if v(r)=v(i) and multiplicity(r)>multiplicity(i) then multiplicity(i):=multiplicity(r); end if;
				r:=index'pred(r);
			end loop;
			if v(r)=v(i) and multiplicity(r)>multiplicity(i) then multiplicity(i):=multiplicity(r); end if;
			multiplicity(i):=multiplicity(i)+1;
		end loop;
		for i in v'range loop
			if multiplicity(i)>temp then
				temp:=multiplicity(i);
				r:=i;
			end if;
		end loop;
	end if;
	
	return v(r);
end mf;
	