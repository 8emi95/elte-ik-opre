function Lnko(k, l : Positive) return Positive is
begin
	if (k mod l = 0) then return l;
	else return Lnko(l, k mod l);
	end if;
end Lnko;