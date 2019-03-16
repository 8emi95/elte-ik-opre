with tools;
use tools;

procedure main is
	
	kert:integer:=10;
	type parcella is new integer range 1 .. kert;
	permet:parcella:=4; --perfectly random, determined by a 100% fair dice roll
	package dirgen is new Random_Generator(parcella);
	
	
	protected type permetes is
		entry is_permetes(k:in parcella; b:out boolean);
		entry permetez(k: in parcella);
		entry saska_meghal;
		entry van_saska(b:out boolean);
	private
		p:parcella:=dirgen.getrandom;
		M:Integer:=1;
	end permetes;
	protected body permetes is
		entry is_permetes(k:in parcella; b:out boolean) 
			when true is
		begin
			b:= k=p;
		end is_permetes;
		entry permetez(k: in parcella) 
		when true is
		begin
			p:=k;
		end permetez;
		
		entry saska_meghal when true is
		begin
			m:=m-1;
		end saska_meghal;
		entry van_saska(b:out boolean) when true is
		begin
			b:=M>0;
		end van_saska;
	end permetes;
	
	p:permetes;
	
	
	
	task type locust;
	task body locust is
		g:parcella;
		next: parcella;
		v1, v2:boolean;
	begin
		--M:=M+1;
		next:=dirgen.getrandom;
		g:=next;
		p.is_permetes(g,v1);
		p.is_permetes(next,v2);
		while v1 or v2 loop
			next:=dirgen.getrandom;
			p.is_permetes(g,v1);
			p.is_permetes(next,v2);
			g := next;
			delay 1.0;
		end loop;
	end locust;
	
	
	task type gazda;
	task body gazda is
		wait:Duration:=10.0;
		b:boolean;
	begin
		p.van_saska(b);
		while b loop
			p.permetez(dirgen.getrandom);
			delay wait;
			p.van_saska(b);
		end loop;
		
	end gazda;
	
	g:gazda;

begin
	null;
end;