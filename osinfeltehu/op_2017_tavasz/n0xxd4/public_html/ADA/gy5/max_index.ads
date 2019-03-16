generic
	type Elem is private;
	type Index is (<>);
	type Tomb is array(Index range<>) of Elem;
	with function "<"(A, B: Elem) return Boolean is <>;
function max_index(T : in Tomb) return Index;