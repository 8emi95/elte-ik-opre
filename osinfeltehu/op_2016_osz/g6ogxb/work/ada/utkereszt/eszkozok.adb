
with ada.text_IO; use ada.text_IO;
Package body eszkozok is
	protected body jelzolampa is
		entry athalad when aktszin=zold is
		begin
			put_line("Athalad egy auto");
		end athalad;
		procedure valt is
		begin
			if aktszin=lampaszinek'last then aktszin:= lampaszinek'first;
			else aktszin:=lampaszinek'succ(aktszin);
			end if;
			put_line(lampaszinek'Image(aktszin));
		end valt;
		function szin return lampaszinek is
		begin
			return aktszin;
		end szin;
	end jelzolampa;
	
	task body utemezo is
		begin
			for i in 1..3 loop
				jelzolampa.valt; delay 3.0;
				jelzolampa.valt; delay 0.5;
				jelzolampa.valt; delay 2.0;
				jelzolampa.valt; delay 1.0;
			end loop;
		end utemezo;
		
	protected body Veletlen is
		procedure Inicializal is
		begin
			Reset(G);
			Inicializalt := True;
		end Inicializal;

		entry meghiv(returnvalue:iranyok)
		when Inicializalt is
		begin
			returnvalue := iranyok( Random(G) );
		end Ido;
	end Veletlen;

end eszkozok;