procedure cserel(A,B : in out T) is
	tmp : T := A;
begin
	A := B;
	B := tmp;
end cserel;