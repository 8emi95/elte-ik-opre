SET SERVEROUTPUT ON;




-- Kivételkezeléssel az egyik feladat:

DECLARE
  v_fizetes dolgozo.fizetes%TYPE;
BEGIN
  SELECT fizetes * 0.10 INTO v_fizetes
  FROM dolgozo
  WHERE dkod = 7369;  --rossz pl.: 100
  
  DBMS_OUTPUT.PUT_LINE('fizetes = ' || TO_CHAR(v_fizetes));
  
EXCEPTION
WHEN NO_DATA_FOUND THEN
  DBMS_OUTPUT.PUT_LINE ('Nem volt megfelelõ sor');
WHEN TOO_MANY_ROWS then
  DBMS_OUTPUT.PUT_LINE ('Túl sok sor volt');
END;
/




-- 6-7 egy az egyben:

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




-- 6-17 egy az egyben:

DECLARE
  emp_job     employees.job_id%TYPE := 'ST_CLERK';
  emp_salary  employees.salary%TYPE := 3000;
  my_record   employees%ROWTYPE;

  CURSOR c1 (job VARCHAR2, max_wage NUMBER) IS
    SELECT * FROM employees
    WHERE job_id = job
    AND salary > max_wage;
BEGIN
/* Open the cursor with one of these statements:
   OPEN c1('ST_CLERK', 3000);
   OPEN c1('ST_CLERK', emp_salary);
   OPEN c1(emp_job, 3000);
   OPEN c1(emp_job, emp_salary); */

  OPEN c1(emp_job, emp_salary);

  LOOP
     FETCH c1 INTO my_record;
     EXIT WHEN c1%NOTFOUND;
     DBMS_OUTPUT.PUT_LINE
       ('Name = ' || my_record.last_name || ', salary = ' ||
        my_record.salary || ', Job Id = ' || my_record.job_id );
  END LOOP;
END;
/




-- 9.) Növeljük meg a hivatalnokok (CLERK) fizetését a saját fizetésük 20%-ával!
          -- for update kurzor módosításhoz, lásd ugyanez a feladat: példa-6-43
          
-- 6-43 egy az egyben:

DECLARE
  my_emp_id NUMBER(6);
  my_job_id VARCHAR2(10);
  my_sal    NUMBER(8,2);
  CURSOR c1 IS
    SELECT employee_id, job_id, salary
    FROM employees FOR UPDATE;
BEGIN
  OPEN c1;
  LOOP
    FETCH c1 INTO my_emp_id, my_job_id, my_sal;
    IF my_job_id = 'SA_REP' THEN
      UPDATE employees
      SET salary = salary * 1.02
      WHERE CURRENT OF c1;
    END IF;
    EXIT WHEN c1%NOTFOUND;
  END LOOP;
END;
/

DROP TABLE CUSTOMERS;
CREATE TABLE CUSTOMERS(
   ID   INT              NOT NULL,
   NAME VARCHAR (20)     NOT NULL,
   AGE  INT              NOT NULL,
   ADDRESS  CHAR (25) ,
   SALARY   DECIMAL (18, 2),       
   PRIMARY KEY (ID)
);
COMMIT