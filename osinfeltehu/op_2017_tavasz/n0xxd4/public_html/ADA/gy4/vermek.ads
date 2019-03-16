package Vermek is
type Verem is private;
	
	procedure Push(V : in out Verem; E : in Integer);
	procedure Pop(V : in out Verem);
	function Top(V : in Verem) return Integer;
	function isEmpty(V : in Verem) return Boolean;
	
private
	type Tomb is array(1..80) of Integer;
	type Verem is record
	Elemek : Tomb;
	Veremteto : Integer := 0; 
	end record;
	
end Vermek;
