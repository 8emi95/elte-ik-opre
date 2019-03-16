-- 1.) Az elsõ feladat: Írjuk ki PL/SQL blokkból: 'Szia Világ!', majd egészítsük ki, 
--          kérjen be egy nevet, számot, dátumot és ezeket is írassuk ki a programból! 


SET SERVEROUTPUT ON;
DECLARE
  begin
    DBMS_OUTPUT.PUT_LINE( 'Szia világ!'|| &szam ||'&string'|| TO_DATE ('&date', 
'YYYY MM DD'));
  end;
  /
  
 --2.) Írjuk ki KING fizetését (olvasás táblából változóba), abban az esetben, 
   --       ha ismert, hogy pontosan egy KING nevû dolgozó szerepel a táblában, 
     --     lásd példa-2-25 select-into (csak ha a lekérdezés pontosan egy sort ad). 

select * from dolgozo;

DECLARE
  --fiz   NUMBER(8,2);
  fiz      dolgozo.fizetes%type;
BEGIN
  SELECT FIZETES INTO fiz
  FROM dolgozo
  WHERE dnev='KING';
  DBMS_OUTPUT.PUT_LINE('Fizetés = ' || TO_CHAR(fiz));
END;
/

--3.Adjuk meg egy fõnök azonosítóját, és töröljük a közvetlen beosztottjait, és 
--írassuk ki hány beosztottja volt, lásd  példa-6-4 implicit kurzor attribútumok.-
select * from dolgozo;

DROP TABLE dolgozo2;
CREATE TABLE dolgozo2 AS
  SELECT * FROM dolgozo;

DECLARE
  fonok NUMBER(6) := 7839;
BEGIN
  DELETE FROM dolgozo2 WHERE fonoke = fonok;
  DBMS_OUTPUT.PUT_LINE('Beosztotjainak száma: ' || TO_CHAR(SQL%ROWCOUNT));
END;
/       
          
-- 10gyak--------------------------------------------------------------------------------------------------------------------------------


-- 5.) Írjuk ki a dolgozók nevét és fizetését! 
-- több sort visszaadó lekérdezés, kurzor használata, lásd  példa-6-7
Select * from dolgozo;

SET SERVEROUTPUT ON;
DECLARE
  nev    dolgozo.dnev%TYPE;    
  fizu  dolgozo.fizetes%TYPE;
    CURSOR c1 IS
    SELECT dnev, fizetes FROM dolgozo;
  begin
    open c1;
      loop
        FETCH c1 INTO nev, fizu;
        EXIT WHEN c1%NOTFOUND;
        DBMS_OUTPUT.PUT_LINE( nev || ' ' || fizu );
      end loop;
    close c1;
    null;
  end;
  /
  ----------------------------------------------------------------------------------
SET SERVEROUTPUT ON;  
DECLARE
  CURSOR c1 IS
    SELECT last_name FROM employees
    WHERE ROWNUM < 11;

  name  employees.last_name%TYPE;
BEGIN
  OPEN c1;
  LOOP
    FETCH c1 INTO name;
    EXIT WHEN c1%NOTFOUND OR c1%NOTFOUND IS NULL;
    DBMS_OUTPUT.PUT_LINE(c1%ROWCOUNT || '. ' || name);
    IF c1%ROWCOUNT = 5 THEN
       DBMS_OUTPUT.PUT_LINE('--- Fetched 5th record ---');
    END IF;
  END LOOP;
  CLOSE c1;
END;
/
  
Select * from dolgozo;

SELECT DNEV, FIZETES FROM dolgozo
    --WHERE ROWNUM < 4
    ORDER BY FIZETES DESC;
 
-- 6.) Írjuk ki a 3. 5. és 8. legnagyobb fizetésû dolgozó nevét, fizetését! 
SET SERVEROUTPUT ON;  
DECLARE
  CURSOR c1 IS
    SELECT DNEV, FIZETES FROM dolgozo
    ORDER BY FIZETES DESC;
  
  nev  DOLGOZO.DNEV%TYPE;
  fizu  dolgozo.fizetes%TYPE;
BEGIN
  OPEN c1;
  LOOP
    FETCH c1 INTO nev, fizu;
    EXIT WHEN c1%NOTFOUND OR c1%NOTFOUND IS NULL;
    IF  c1%ROWCOUNT = 3 or c1%ROWCOUNT = 5 or c1%ROWCOUNT = 8 THEN
       DBMS_OUTPUT.PUT_LINE(c1%ROWCOUNT || '. ' || nev || ' ' || fizu);
    END IF;
    
  END LOOP;
  CLOSE c1;
END;
/

--7. Írjuk ki azon dolgozók nevét és fizetését, akik fizetése nagyobb mint 
--egy olyan szám, amelyet a felhasználó fog majd futás közben megadni! 
-- felhasználói/helyettesítõ változók: &numerikus_valt, '&karakteres_valt'
SET SERVEROUTPUT ON;  
DECLARE
  CURSOR c1 IS
    SELECT DNEV, FIZETES FROM dolgozo;
  
  nev  DOLGOZO.DNEV%TYPE;
  fizu  dolgozo.fizetes%TYPE;
  --nume INTEGER;
BEGIN
  OPEN c1;
  --DBMS_OUTPUT.PUT_LINE('Kerem azt a számot aminél a nagyobb dolgozók fizetése kell:'|| nume);
  LOOP
    FETCH c1 INTO nev, fizu;
    EXIT WHEN c1%NOTFOUND OR c1%NOTFOUND IS NULL;
    IF  fizu>&nume THEN
       DBMS_OUTPUT.PUT_LINE( nev || ' ' || fizu);
    END IF;
    
  END LOOP;
  CLOSE c1;
END;
/





--10gyak 8.) Írjuk ki azon dolgozók nevét, fizetését és osztálykódját, akik a felhasználó 
--által megadott osztályon dolgoznak! A felhasználó által megadott betû 
--legyen A, R, S (Accounting ...) a 10, 20, 30-as osztály esetén. 
SELECT * FROM employees;
SELECT Dnev, FIZETES,oazon FROM dolgozo
where oazon=30;

SELECT oazon FROM osztaly
where SUBSTR(ONEV, 1, 1)='A';


--osztaly tabala onan jönnek  a betük;
SET SERVEROUTPUT ON;
DECLARE
  kod VARCHAR2(1):=SUBSTR('&kod',1,1);
  recordd   osztaly.OAZON%TYPE;
  record2   dolgozo%ROWTYPE;
  
  CURSOR c1 (ko VARCHAR2) IS
    SELECT oazon FROM osztaly
    where SUBSTR(ONEV, 1, 1)=ko;
    
  CURSOR c2 (oazonosito osztaly.oazon%type) is
    SELECT * FROM dolgozo
    where oazon=oazonosito;

BEGIN
  OPEN c1(kod);
    LOOP
       FETCH c1 INTO recordd;
       EXIT WHEN c1%NOTFOUND;
       --DBMS_OUTPUT.PUT_LINE(recordd );
    END LOOP;
    close c1;
    OPEN c2(recordd);
    LOOP
       FETCH c2 INTO record2;
       EXIT WHEN c2%NOTFOUND;
       DBMS_OUTPUT.PUT_LINE('Név = ' || record2.DNEV || ' Fizetés = ' || record2.fizetes || ' Oazon = ' || record2.oazon);
    END LOOP;
    close c2;
END;
/
          
          
-- 10gy 9.) Növeljük meg a hivatalnokok (CLERK) fizetését a saját fizetésük 20%-ával!          
select * from dolgozo;


DROP TABLE dolgozo2;
CREATE TABLE dolgozo2 AS
  SELECT * FROM dolgozo;
DECLARE
  Nev VARCHAR2(10);
  fiz    NUMBER(8,2);
  fog VARCHAR2(10);
  CURSOR c1 IS
    SELECT  fizetes, FOGLALKOZAS
    FROM dolgozo2 FOR UPDATE;
BEGIN
  OPEN c1;
  LOOP
    FETCH c1 INTO fiz, fog;
    IF fog = 'CLERK' THEN
      UPDATE dolgozo2
      SET fizetes = fizetes * 1.2
      WHERE CURRENT OF c1;
    END IF;
    EXIT WHEN c1%NOTFOUND;
  END LOOP;
END;
/ 

select * from dolgozo2;

-- 10.) Módosítsuk a dolgozók nevét írjuk át, hogy csak a kezdõbetû legyen nagy, 
--         a többi betû kicsi, továbbá a dolgozók fizetését is növeljük meg 2 %-kal!    
select  * from dolgozo2;

DROP TABLE dolgozo2;
CREATE TABLE dolgozo2 AS
  SELECT * FROM dolgozo;
DECLARE
  Nev VARCHAR2(10);
  fiz    NUMBER(8,2);
  CURSOR c1 IS
    SELECT Dnev, fizetes
    FROM dolgozo2 FOR UPDATE;
BEGIN
  OPEN c1;
  LOOP
    FETCH c1 INTO Nev,fiz;
      UPDATE dolgozo2
      SET fizetes = fizetes * 1.02,
      dnev=INITCAP(dnev);
    EXIT WHEN c1%NOTFOUND;
  END LOOP;
  close c1;
END;
/ 

--hibakezelés
--nincs ilyen nevü ember
DECLARE
  --fiz   NUMBER(8,2);
  fiz      dolgozo.fizetes%type;
BEGIN
  SELECT FIZETES INTO fiz
  FROM dolgozo
  WHERE dnev='KINGA';
  DBMS_OUTPUT.PUT_LINE('Fizetés = ' || TO_CHAR(fiz));
  EXCEPTION
  WHEN NO_DATA_FOUND THEN
    DBMS_OUTPUT.PUT_LINE ('Nincs ilyen nevu emberke');
END;
/

DECLARE
  --fiz   NUMBER(8,2);
  fiz      dolgozo.foglalkozas%type;
BEGIN
  SELECT FOGLALKOZAS INTO fiz
  FROM dolgozo
  WHERE FOGLALKOZAS='CLERK';
  DBMS_OUTPUT.PUT_LINE('Foglalkozas = ' || TO_CHAR(fiz));
  EXCEPTION
    WHEN TOO_MANY_ROWS THEN
      DBMS_OUTPUT.PUT_LINE ('Nem egy ilyen található');
END;
/

CREATE FUNCTION azonToName(azon IN NUMBER) 
   RETURN Varchar2
   IS szemely varchar2(10);
   BEGIN 
      SELECT dnev INTO szemely 
      FROM dologozo
      WHERE dazon = azon; 
      RETURN(szemely); 
    END;
/

SET SERVEROUTPUT ON
DECLARE
szam1 NUMBER(2) := 1;
szam2 NUMBER(2);
szam3 NUMBER(2) := 3;
PROCEDURE muvelet (inp IN NUMBER, outp OUT NUMBER, inout IN OUT NUMBER) IS
BEGIN
dbms_output.put_line('in parameter: ' || inp || ', in out parameter: ' || inout); outp := inp + inout; inout := outp + inp; END muvelet;
BEGIN
muvelet(szam1, szam2, szam3);
dbms_output.put_line('out parameter: ' || szam2 || ', in out parameter: ' || szam3);
END;
/


set SERVEROUTPUT ON
DECLARE 
   a number; 
   b number; 
   c number; 
FUNCTION findMax(x IN number, y IN number)  
RETURN number 
IS 
    z number; 
BEGIN 
   IF x > y THEN 
      z:= x; 
   ELSE 
      Z:= y; 
   END IF;  
   RETURN z; 
END; 
BEGIN 
   a:= 23; 
   b:= 45;  
   c := findMax(a, b); 
   dbms_output.put_line(' Maximum of (23,45): ' || c); 
END; 
/ 