--with ada.text_IO; use ada.text_IO;
with Ada.Numerics.Discrete_Random;
--use Ada.Numerics.Discrete_Random;
Package eszkozok is
	type lampaszinek is (piros, pirossarga, zold, sarga);
	type iranyok is (egyenesen_megy, fordul);
	type intgenerator: Ada.numerics.discrete_random(Integer);
	use intgenerator;
	protected jelzolampa is
		entry athalad;
		procedure valt;
		function szin return lampaszinek;
		
	private
		aktszin: lampaszinek:= piros;
	end jelzolampa;
	
	task utemezo;
	protected Veletlen is
		procedure Inicializal;
		entry meghiv(returnvalue:iranyok);
	private
		G: Generator;
		Inicializalt: Boolean := False;

	end Veletlen;
end eszkozok;