with Ada.Text_IO;
use Ada.Text_IO;

procedure Matrixos is
	
    type Index is new Integer;
    type Elem is new Integer;
    type Matrix is array (Index range <>, Index range <>) of Elem;
	type tomb is array (index range <>) of elem;

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
	
	function Min_Hely ( T: Tomb ) return Index is
        Mh: Index := T'First;
    begin
        for I in T'Range loop
            if T(Mh) > T(I) then
                Mh := I;
            end if;
        end loop;
        return Mh;
	end min_hely;
	
    procedure Hozzaad( A: in out Matrix; B: in Matrix ) is
    begin
        -- A := A+B;
        for I in A'Range(1) loop
            for J in A'Range(2) loop
                A(I,J) := A(I,J) + B( I-A'First(1)+B'First(1),
                                      J-A'First(2)+B'First(2) );
            end loop;
        end loop;
    end Hozzaad;

    function "+"( A, B: Matrix ) return Matrix is
        -- C: Matrix := A;
        C: Matrix(A'Range(1),A'Range(2));
    begin
        -- Hozzaad(C,B);
        -- return C;
        for I in A'Range(1) loop
            for J in A'Range(2) loop
                C(I,J) := A(I,J) + B( I-A'First(1)+B'First(1),
                                      J-A'First(2)-B'First(2) );
            end loop;
        end loop;
        return C;
    end "+";
	
	procedure LM(A:In Matrix) is
	mr : index;
	begin
		for i in A'Range(1) loop
			put('0');
		end loop;
	end LM;

    procedure Kiir ( M: in Matrix ) is
    begin
        for I in M'Range(1) loop
            for J in M'Range(2) loop
                Put( Elem'Image(M(I,J)) );
                Put( Ascii.HT );
            end loop;
            New_Line;
        end loop;
    end Kiir;

    M: Matrix := ((1,2),(1,2),(3,2));

begin
    Hozzaad(M,M);
    M := M + M;
    Kiir(M);
end Matrixos;

