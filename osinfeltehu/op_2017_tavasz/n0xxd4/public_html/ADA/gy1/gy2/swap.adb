procedure Swap(p1, p2 : in out Integer) is
tmp : Integer := p1;
begin
	p1 := p2;
	p2 := tmp;
end;