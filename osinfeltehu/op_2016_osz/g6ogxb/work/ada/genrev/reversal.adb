procedure reversal (T: in out tomb) is
--r:tomb(t'range);
i:index:=t'first;
j:Index := t'last;

	
begin
	while i < j loop
	--swap(t(i),t(j));
	paramswap(t(i),t(j));
	j := index'pred(j);
	i := index'succ(i);
	end loop;
	--t := r;
end reversal;