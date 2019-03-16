package body Mat is

   function Lnko ( A, B : Positive ) return Positive is
      X: Positive := A;
      Y: Positive := B;
   begin
      while X /= Y loop
         if X > Y then
            X := X - Y;
         else
            Y := Y - X;
         end if;
      end loop;
      return X;
   end Lnko;

   function Faktorialis( N: Natural ) return Positive is
      Fakt : Positive := 1;
   begin
      for I in 1..N loop
         Fakt := Fakt * I;
      end loop;
      return Fakt;
   end Faktorialis;
   
   function max2(A, B: Natural) return Natural is
   begin
		if A>=B then return A;
		else return B;
		end if;
   end max2;
   
   function max3(A,B,C:Natural) return natural is
   begin
		return max2(A,max2(B,C));
	end max3;
	
	function sumn(n:Natural) return Natural is
		s : Natural := 0;
	begin
		for I in 1..n loop
			s := s+i;
		end loop;
		return s;
	end sumn;
	
	function signx(x:in Integer) return Integer is
		sig: Integer := 0;
	begin
		if x > 0 then sig:=1;
		elsif x < 0 then sig:=-1;
		end if;
		return sig;
	end signx;
	function pi(n:Positive) return float is
		p:float :=1.0;
		i:float;
	begin
		for j in 1..(n+2) loop
			i:=float(j);
			P:=p*(float(i) / (float(i)-1.0))*(float(i)/(float(i)+1.0));
		end loop;
		return 2.0*p;
	end pi;
	
	function mod2check(n:natural) return boolean is
	begin
		return ( n mod 2 =0);
	end mod2check;
	
	function euler(precision:Positive) return float is
		e:float:=1.0;
	begin
		for i in 1..precision loop
			e:=e+1.0/float(faktorialis(i));
		end loop;
		return e;
	end euler;
	
	function combination(n,k:natural) return natural is
	begin
		return 0;
	end combination;
	
	
	
	function digitsum(n:natural) return natural is
	begin
		if n=0 then return 0;
		else return (n mod 10) + digitsum(n / 10);
		end if;
	end digitsum;
	
	function d9(n:natural) return boolean is
	begin
		return digitsum(n) mod 9 = 0;
	end d9;
	
	function isperfect(n:natural) return boolean is
		s:natural:=1;
	begin
		for i in 2..n-1 loop
			if n mod i = 0 then
				s := s + i;
			end if;
		end loop;
		return s = n;
	end isperfect;
end Mat;
   
