generic
	type Key is private;
	type Value is private;

package Maps is
	type Map(Capacity : Integer) is limited private;
	type Item is private;
	type Item_Array is array(Integer range<>) of Item;
	
	function Size(M : in Map) return Integer;
	function Contains(M : in Map; K : in Key) return Boolean;
	procedure Put(M : in out Map; K : in Key; V : in Value);
	function Get(M : in Map; K : in Key) return Value;
	
private
	type Item is record
	K : Key;
	V : Value;
	end record;
	
	type Map(Capacity : Integer) is record
	Size : Integer := 0;
	Items : Item_Array(1..Capacity);
	end record;

end Maps;
