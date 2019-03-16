package body Maps is
	
	function Size(M : in Map) return Integer is
	begin
		return M.Size;
	end Size;
	
	function Contains(M : in Map; K : in Key) return Boolean is
	Found : Boolean := false;
	begin
		for I in 1..M.Size loop
			if M.Items(I).K = K then Found := true;
			end if;
		end loop;
		
		return Found;
	end Contains;
	
	procedure Put(M : in out Map; K : in Key; V : in Value) is
	begin
		for I in 1..M.Size loop
			if M.Items(I).K = K then
				M.Items(I).V := V;
			end if;
		end loop;
		
		if not Contains(M, K) then
			M.Size := M.Size + 1;
			M.Items(M.Size).K := K;
			M.Items(M.Size).V := V;
		end if;
	end;
	
	function Get(M : in Map; K : in Key) return Value is
	retValue : Value;
	begin
		for I in 1..M.Size loop
			if M.Items(I).K = K then retValue := M.Items(I).V;
			end if;
		end loop;
		
		return retValue;
	end;
	
	procedure Remove(M : in out Map; K : in Key) is
	begin
		if Contains(M, K) then M.Size := M.Size - 1;		
		end if;
	end;
	
begin
	null;
end Maps;



