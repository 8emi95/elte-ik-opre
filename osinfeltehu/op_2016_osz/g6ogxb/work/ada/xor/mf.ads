generic	
	type Elem is private;
    type Index is (<>);
    type Vector is array (Index range <>) of Elem;
	
function mf(v:vector) return elem;
	
