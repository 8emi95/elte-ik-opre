with Ada.text_IO;
package body boolean_text_IO is
	procedure Put(b:in boolean) is
	begin
		if b then
			ada.text_IO.put("True");
		else
			ada.text_IO.put("False");
		end if;
	end put;
end boolean_text_IO;