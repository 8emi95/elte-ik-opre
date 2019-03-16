package body Generikus_Vermek is

	procedure Push(V : in out Verem; E : in Elem) is
	begin
		V.Veremteto := V.Veremteto + 1;
		V.Elemek(V.Veremteto) := E;
	end Push;
	
	procedure Pop(V : in out Verem) is
	begin
		V.Veremteto := V.Veremteto - 1;
	end Pop;
	
	function Top(V : in Verem) return Elem is
	begin
		return V.Elemek(V.Veremteto);
	end Top;
	
	function isEmpty(V : in Verem) return Boolean is
	begin
		return V.Veremteto = V.Elemek'First - 1;
	end isEmpty;
	
begin
	null;
end Generikus_Vermek;
