with cserel,max_index;
procedure rendez(T: in out Tomb) is
	procedure Cserel_Elem is new cserel(Elem);
	function Max_Index_Tomb is new Max_Index(Elem,Index,Tomb);
	M:Index;
begin
	for i in reverse T'Range loop
		M:= Max_Index_Tomb(T(T'First..I));
		Cserel_Elem(T(I),T(M));
	end loop;
end rendez;