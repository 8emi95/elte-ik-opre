select * from employees;


DECLARE
  v_jobid     employees.job_id%TYPE;     -- variable for job_id
  v_lastname  employees.last_name%TYPE;  -- variable for last_name

  CURSOR c1 IS
    SELECT last_name, job_id FROM employees
    WHERE REGEXP_LIKE (job_id, 'S[HT]_CLERK');

  v_employees employees%ROWTYPE;         -- record variable for row

  CURSOR c2 IS
    SELECT * FROM employees
    WHERE REGEXP_LIKE (job_id, '[ACADFIMKSA]_M[ANGR]');

BEGIN
  OPEN c1;
  LOOP  -- Fetches 2 columns into variables
    FETCH c1 INTO v_lastname, v_jobid;
    EXIT WHEN c1%NOTFOUND;
    DBMS_OUTPUT.PUT_LINE( RPAD(v_lastname, 25, ' ') || v_jobid );
  END LOOP;
  CLOSE c1;
  DBMS_OUTPUT.PUT_LINE( '-------------------------------------' );

  OPEN c2;
  LOOP  -- Fetches entire row into the v_employees record
    FETCH c2 INTO v_employees;
    EXIT WHEN c2%NOTFOUND;
    DBMS_OUTPUT.PUT_LINE( RPAD(v_employees.last_name, 25, ' ') ||
                               v_employees.job_id );
  END LOOP;
  CLOSE c2;
END;
/


Select * from dolgozo;

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
  
  
