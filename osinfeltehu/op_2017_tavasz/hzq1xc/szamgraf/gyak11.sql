set server on
DECLARE
  v_fiz dolgozo.fizetes;
BEGIN
  SELECT fizetes into v_fiz
  FROM dolgozo
  where Dnev='KING';
  DBMS_OUTPUT.PUT_LINE('bonus = '|| to_char(bonus));
end;
/