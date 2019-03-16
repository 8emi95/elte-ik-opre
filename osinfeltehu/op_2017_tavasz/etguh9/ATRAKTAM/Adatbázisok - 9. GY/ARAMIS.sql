-- PL/SQL:


-- anonimus blokkokat használunk (most)


-- alap példa:
begin
    null;   -- kell a null, mint az ADA-ban
end;
/


-- 2-25. példa (javítva, kell elé SET SERVEROUTPUT ON;):
SET SERVEROUTPUT ON;
DECLARE
    bonus   NUMBER(8,2);
BEGIN
    SELECT salary * 0.10 INTO bonus   --csak akkor használható a SELECT INTO, ha egy sort kapunk vissza (egyéb esetben hiba (akkor is ha 0, akkor is ha > 1))
    FROM employees
    WHERE employee_id = 100;
  
    DBMS_OUTPUT.PUT_LINE('bonus = ' || TO_CHAR(bonus));
END;
/


-- Szia világ:
SET SERVEROUTPUT ON;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Szia ' || 'világ!');
END;
/


-- Szia Józsi:
SET SERVEROUTPUT ON;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Szia ' || '&nev' || '!');
END;
/


-- Szia Józsi, mi a kedvenc számod?:
SET SERVEROUTPUT ON;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Szia ' || '&nev' || '!');
    DBMS_OUTPUT.PUT_LINE('mi a kedvenc számod? ' || &szam || ' Ez a kedvenc szamod|');
    DBMS_OUTPUT.PUT_LINE('mi a kedvenc számod? ' || to_char(&szam) || ' Ez a kedvenc szamod|');
END;
/


-- King fizetése:
SET SERVEROUTPUT ON;
DECLARE
    king   dolgozo.fizetes%type;   --hivatkozási típus
BEGIN
    SELECT fizetes INTO king
    FROM dolgozo
    WHERE dnev = 'KING';
    --WHERE dnev = 'KINGKONG';   --hibaüzenet (sztenderd hiba, nincs sor)
    --WHERE dnev LIKE 'K%';   --ez most véletlen nem hiba, mert csak egy ember neve kezdõdik 'K'-val :)
    
    DBMS_OUTPUT.PUT_LINE('King fizetése: ' || TO_CHAR(king));
END;
/


-- 6-4 átírva
DROP TABLE employees_temp;
CREATE TABLE employees_temp AS
    SELECT * FROM employees;

DECLARE
  mgr_no NUMBER(6) := &dolgazon;   --itt van átírva, hogy meg lehessen adni a számot
BEGIN
  DELETE FROM employees_temp WHERE manager_id = mgr_no;
  DBMS_OUTPUT.PUT_LINE
    ('Number of employees deleted: ' || TO_CHAR(SQL%ROWCOUNT));
END;
/


-- Eljut feladat:   NEM MÛKÖDIK!!
DROP TABLE eljut;
CREATE TABLE eljut AS
    SELECT * FROM employees;
    
SET SERVEROUTPUT ON;

DECLARE
    RegiSzamlalo INTEGER := 0;
    UJSzamlalo INTEGER := 0;
BEGIN
    -- A ciklus elõtt kezdeti értékek beállítása (a járatok alapján honnan-hova várospárok)
    delete from eljut;
    RegiSzamlalo:= 0;
    insert into eljut
        (select distinct honnan, hova from jaratok);
    select count(*) into UjSzamlalo from eljut;

    -- A ciklust addig kell végrehajtani, ameddig növekszik az eredmény, fontos,
    -- hogy csak olyan várospárokat vegyünk az eredményhez, ami még nem volt!
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