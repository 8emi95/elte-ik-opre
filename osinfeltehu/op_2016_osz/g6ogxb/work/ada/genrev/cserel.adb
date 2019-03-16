procedure Cserel ( A, B: in out Elem) is
    Tmp: Elem := A;
begin
    A := B;
    B := Tmp;
end Cserel;