package isort is
	type elem is new integer;
	type index is new integer;
	type tomb is array (index range <>) of elem;
	
	procedure swap (a,b: in out elem);
	function max_hely(T:Tomb) return index;
	procedure rendez(t: in out tomb);
end isort;