-- PL/SQL:


-- anonimus blokkokat haszn�lunk (most)


-- alap p�lda:
begin
    null;   -- kell a null, mint az ADA-ban
end;
/


-- 2-25. p�lda (jav�tva, kell el� SET SERVEROUTPUT ON;):
SET SERVEROUTPUT ON;
DECLARE
    bonus   NUMBER(8,2);
BEGIN
    SELECT salary * 0.10 INTO bonus   --csak akkor haszn�lhat� a SELECT INTO, ha egy sort kapunk vissza (egy�b esetben hiba (akkor is ha 0, akkor is ha > 1))
    FROM employees
    WHERE employee_id = 100;
  
    DBMS_OUTPUT.PUT_LINE('bonus = ' || TO_CHAR(bonus));
END;
/


-- Szia vil�g:
SET SERVEROUTPUT ON;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Szia ' || 'vil�g!');
END;
/


-- Szia J�zsi:
SET SERVEROUTPUT ON;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Szia ' || '&nev' || '!');
END;
/


-- Szia J�zsi, mi a kedvenc sz�mod?:
SET SERVEROUTPUT ON;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Szia ' || '&nev' || '!');
    DBMS_OUTPUT.PUT_LINE('mi a kedvenc sz�mod? ' || &szam || ' Ez a kedvenc szamod|');
    DBMS_OUTPUT.PUT_LINE('mi a kedvenc sz�mod? ' || to_char(&szam) || ' Ez a kedvenc szamod|');
END;
/


-- King fizet�se:
SET SERVEROUTPUT ON;
DECLARE
    king   dolgozo.fizetes%type;   --hivatkoz�si t�pus
BEGIN
    SELECT fizetes INTO king
    FROM dolgozo
    WHERE dnev = 'KING';
    --WHERE dnev = 'KINGKONG';   --hiba�zenet (sztenderd hiba, nincs sor)
    --WHERE dnev LIKE 'K%';   --ez most v�letlen nem hiba, mert csak egy ember neve kezd�dik 'K'-val :)
    
    DBMS_OUTPUT.PUT_LINE('King fizet�se: ' || TO_CHAR(king));
END;
/


-- 6-4 �t�rva
DROP TABLE employees_temp;
CREATE TABLE employees_temp AS
    SELECT * FROM employees;

DECLARE
  mgr_no NUMBER(6) := &dolgazon;   --itt van �t�rva, hogy meg lehessen adni a sz�mot
BEGIN
  DELETE FROM employees_temp WHERE manager_id = mgr_no;
  DBMS_OUTPUT.PUT_LINE
    ('Number of employees deleted: ' || TO_CHAR(SQL%ROWCOUNT));
END;
/


-- Eljut feladat:   NEM M�K�DIK!!
DROP TABLE eljut;
CREATE TABLE eljut AS
    SELECT * FROM employees;
    
SET SERVEROUTPUT ON;

DECLARE
    RegiSzamlalo INTEGER := 0;
    UJSzamlalo INTEGER := 0;
BEGIN
    -- A ciklus el�tt kezdeti �rt�kek be�ll�t�sa (a j�ratok alapj�n honnan-hova v�rosp�rok)
    delete from eljut;
    RegiSzamlalo:= 0;
    insert into eljut
        (select distinct honnan, hova from jaratok);
    select count(*) into UjSzamlalo from eljut;

    -- A ciklust addig kell v�grehajtani, ameddig n�vekszik az eredm�ny, fontos,
    -- hogy csak olyan v�rosp�rokat vegy�nk az eredm�nyhez, ami m�g nem volt!
    LOOP
    insert into eljut
        (select distinct eljut.honnan,jaratok.hova
        from eljut, jaratok  
        where eljut.hova = jaratok.honnan
        and (eljut.honnan,jaratok.hova)
            NOT IN (select * from eljut));
        select count(*) into UjSzamlalo from eljut;
        EXIT WHEN UjSzamlalo = RegiSzamlalo;
        RegiSzamlalo := UjSzamlalo;
    END LOOP;
END;
  /