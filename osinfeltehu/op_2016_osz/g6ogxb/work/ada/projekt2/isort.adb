package body isort is

	procedure swap (a,b: in out elem) is
	Tmp: Elem := A;
    begin
        A := B;
        B := Tmp;
	end swap;
	
	procedure rendez(T:in out tomb) is
		Mh: Index;
    begin
        for I in reverse T'Range loop
            Mh := Max_Hely( T(T'First..I) );
	    swap( T(I), T(Mh) );
        end loop;
	end rendez;
	
	function Max_Hely ( T: Tomb ) return Index is
        Mh: Index := T'First;
    begin
        for I in T'Range loop
            if T(Mh) < T(I) then
                Mh := I;
            end if;
        end loop;
        return Mh;
	end max_hely;

end isort;