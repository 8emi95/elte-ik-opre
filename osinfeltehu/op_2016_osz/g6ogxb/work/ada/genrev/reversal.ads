generic
	type elem is limited private;
	type index is (<>);
	type tomb is array (index range <>) of elem;
	with procedure paramswap(a,b:in out elem);
procedure reversal(t:in out Tomb);