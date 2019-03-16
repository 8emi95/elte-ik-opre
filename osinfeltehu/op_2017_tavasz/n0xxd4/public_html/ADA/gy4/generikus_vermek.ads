generic
	type Elem is private;
	
package Generikus_Vermek is
	type Verem(Meret : Integer) is limited private;
	
	procedure Push(V : in out Verem; E : in Elem);
	procedure Pop(V : in out Verem);
	function Top(V : in Verem) return Elem;
	function isEmpty(V : in Verem) return Boolean;
	
private
	subtype Index is Natural range 0..100;
	type Tomb is array(Index range<>) of Elem;
	type Verem(Meret : Integer) is record
	Elemek : Tomb(1..Meret);
	Veremteto : Index := 0;
	end record;
	
end Generikus_Vermek;
