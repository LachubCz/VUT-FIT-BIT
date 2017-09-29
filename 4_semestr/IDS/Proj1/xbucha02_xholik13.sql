-- IDS Projekt: Zadani c. 20 – Zkousky (IUS)
-- Authors: Petr Buchal(xbucha02), Tomas Holik (xholik13)
-- Date: 03/2017

-- ORACLE
-- generator posloupnosti cisel "1."=[klic] [r_cislo]
-- 								"2."=[klic] [r_cislo]
-- create sequence [atribut] START WITH [pocatecni velikost] INCREMENT BY [pri kazdem vytvoreni nove polozky o kolik zvetsit]

-- MySQL
-- u atributu AUTO_INCREMENT, pri kazdem vytvoreni polozky se zvedne o jedno


--Delete tables
--BEGIN
 -- EXECUTE IMMEDIATE 'DROP TABLE Osoba, Student, Vyucujici, Predmet, Termin, Prihlaseni_na_termin, Termin_Zkousky, Otazka, Hodnoceni, Otazka_Hodnoceni, Termin_Otazka CASCADE CONSTRAINTS';
 -- EXCEPTION
 -- WHEN OTHERS THEN
 -- IF SQLCODE != -942 THEN
  --  RAISE;
 -- END IF;
--END;

--drop and create sequences
--BEGIN
 -- EXECUTE IMMEDIATE 'DROP SEQUENCE id_osoba, id_kod_terminu, id_kod_predmetu, id_kod_zkousky, id_kod_hodnoceni, id_cislo_otazky';
 -- EXCEPTION
 -- WHEN OTHERS THEN
 --   IF SQLCODE != -2289 THEN
  --    RAISE;
  --  END IF;
    
  --EXECUTE IMMEDIATE 'CREATE SEQUENCE id_osoba, id_kod_terminu, id_kod_predmetu, id_kod_zkousky, id_cislo_otazky, id_kod_hodnoceni START WITH 1 INCREMENT BY 1 NO CYCLE';
--END;

--CREATE SEQUENCE id_osoba,id_kod_terminu, id_kod_predmetu, id_kod_zkousky, id_cislo_otazky, id_kod_hodnoceni START WITH 1 INCREMENT BY 1 NO CYCLE;
-- prvni normalni forma 	- atomicke hodnoty
-- druha normalni forma 	- kazdy neklicovy je plne funkce zavisly
-- treti normalnio forma 	- neexistuje zadny neklicovy atribut, ktery je tranzitivne zavisly
-- BCNF 					- pro kazdou netrivialni funkcni zavislost X->Y je X superklicem, to znamena, ze dva klicove pokud maji navzajem vztah vytvori se nova tabulka, aby se odstranila redundance


DROP TABLE Osoba CASCADE constraints;
DROP TABLE Student CASCADE constraints;
DROP TABLE Osoba_Titul CASCADE constraints;
DROP TABLE Vyucujici CASCADE constraints;
DROP TABLE Predmet CASCADE constraints;
DROP TABLE Zkouska CASCADE constraints;
DROP TABLE Termin CASCADE constraints;
DROP TABLE Prihlaseni_na_termin CASCADE constraints;
DROP TABLE Otazka CASCADE constraints;
DROP TABLE Hodnoceni CASCADE constraints;
DROP TABLE Otazka_Hodnoceni CASCADE constraints;
DROP TABLE Termin_Otazka CASCADE constraints;
DROP TABLE Vyucujici_Predmetu CASCADE constraints;
DROP TABLE Student_Predmet CASCADE constraints;


DROP SEQUENCE id_osoba_seq;
DROP SEQUENCE id_kod_terminu_seq;
DROP SEQUENCE id_kod_zkousky_seq;
DROP SEQUENCE id_kod_hodnoceni_seq;
DROP SEQUENCE id_cislo_otazky_seq;

CREATE SEQUENCE id_osoba_seq START WITH 1 INCREMENT BY 1 NOCYCLE;
CREATE SEQUENCE id_kod_terminu_seq START WITH 1 INCREMENT BY 1 NOCYCLE;
CREATE SEQUENCE id_kod_zkousky_seq START WITH 1 INCREMENT BY 1 NOCYCLE;
CREATE SEQUENCE id_cislo_otazky_seq START WITH 1 INCREMENT BY 1 NOCYCLE;
CREATE SEQUENCE id_kod_hodnoceni_seq START WITH 1 INCREMENT BY 1 NOCYCLE;

--Create new tables
CREATE TABLE Osoba (
	id_osoba INT PRIMARY KEY,
  jmeno varchar(120) NOT NULL,
	prijmeni varchar(120) NOT NULL,
	datum_narozeni varchar(80),
	rodne_cislo varchar(60) NOT NULL
);

CREATE TABLE Osoba_Titul (
  id_osoba_s_titulem INT NOT NULL PRIMARY KEY,
	id_titul_pred varchar(50),
  id_titul_za varchar(50),
  CONSTRAINT con_osoba_s_titulem FOREIGN KEY(id_osoba_s_titulem) REFERENCES Osoba(id_osoba)
);

CREATE TABLE Student (
	id_osoba INT NOT NULL PRIMARY KEY,
  CONSTRAINT con_osoba_student FOREIGN KEY(id_osoba) REFERENCES Osoba(id_osoba)
  ON DELETE CASCADE
);

CREATE TABLE Vyucujici (
	id_osoba INT NOT NULL PRIMARY KEY,
  CONSTRAINT con_osoba_vyucujici FOREIGN KEY(id_osoba) REFERENCES Osoba(id_osoba)
  ON DELETE CASCADE
);

CREATE TABLE Predmet (
	id_kod_predmetu varchar(20) NOT NULL PRIMARY KEY,
	pocet_kreditu INT NOT NULL,
  id_garant INT NOT NULL,
  CONSTRAINT con_garant FOREIGN KEY(id_garant) REFERENCES Vyucujici(id_osoba)
  ON DELETE CASCADE
);

CREATE TABLE Vyucujici_Predmetu (
  id_vyucujici INT NOT NULL,
  id_kod_predmetu varchar(20) NOT NULL,
  CONSTRAINT con_vyucujici FOREIGN KEY(id_vyucujici) REFERENCES Vyucujici(id_osoba)
  ON DELETE CASCADE,
  CONSTRAINT con_predmet FOREIGN KEY(id_kod_predmetu) REFERENCES Predmet(id_kod_predmetu)
  ON DELETE CASCADE
);

CREATE TABLE Zkouska (
	id_kod_zkousky INT NOT NULL	PRIMARY KEY,
  typ varchar(64),
  max_pocet_bodu float,
  min_pocet_bodu_pro_slozeni float,
  pocet_terminu INT,
  max_pocet_terminu_pro_studenta INT,
  id_kod_predmetu varchar(20) NOT NULL,
  CONSTRAINT con_zkousky_predmet FOREIGN KEY(id_kod_predmetu) REFERENCES Predmet(id_kod_predmetu)
  ON DELETE CASCADE
);

CREATE TABLE Termin (
	id_kod_terminu INT NOT NULL PRIMARY KEY,
	datum_cas varchar(64),
	misto varchar(90),
  zacatek_prihlasovani varchar(64),
  konec_prihlasovani varchar(64),
  max_pocet_studentu INT,
  cislo_terminu INT NOT NULL,
  id_kod_zkousky INT NOT NULL,
  CONSTRAINT con_kod_zkousky FOREIGN KEY(id_kod_zkousky) REFERENCES Zkouska(id_kod_zkousky)
  ON DELETE CASCADE
);

CREATE TABLE Prihlaseni_na_termin (
	id_osoba INT,
  id_kod_terminu INT NOT NULL,
	CONSTRAINT con_prihlaseni_studenti FOREIGN KEY(id_osoba) REFERENCES Student(id_osoba),
  CONSTRAINT con_na_termin FOREIGN KEY(id_kod_terminu) REFERENCES Termin(id_kod_terminu)
);

CREATE TABLE Otazka (
  id_cislo_otazky INT NOT NULL PRIMARY KEY,
	typ_otazky INT,
  zadani_otazky varchar(1024),
  max_pocet_bodu float,
  min_pocet_bodu_pro_slozeni float
);

CREATE TABLE Hodnoceni (
	id_kod_hodnoceni INT NOT NULL PRIMARY KEY,
	pocet_bodu float NOT NULL,
  procentualni_uspesnost float,
  percentilova_uspesnost float,
  komentar varchar(1024),
  id_vyucujici INT NOT NULL,
	id_student INT NOT NULL,
  CONSTRAINT con_udeluje FOREIGN KEY(id_vyucujici) REFERENCES Vyucujici(id_osoba)
  ON DELETE CASCADE,
 	CONSTRAINT con_jehodnocen FOREIGN KEY(id_student) REFERENCES Student(id_osoba)
	ON DELETE CASCADE
);

CREATE TABLE Student_Predmet (
id_student INT NOT NULL,
id_predmet varchar(20) NOT NULL,
CONSTRAINT con_student_v_predmetu FOREIGN KEY(id_student) REFERENCES Student(id_osoba),
CONSTRAINT con_studovany_predmet FOREIGN KEY(id_predmet) REFERENCES Predmet(id_kod_predmetu)
);

CREATE TABLE Otazka_Hodnoceni (
  id_cislo_otazky INT NOT NULL,
	CONSTRAINT con_otazky FOREIGN KEY(id_cislo_otazky) REFERENCES Otazka(id_cislo_otazky),
  id_kod_hodnoceni INT NOT NULL,
 	CONSTRAINT con_kod_hodnoceni FOREIGN KEY(id_kod_hodnoceni) REFERENCES Hodnoceni(id_kod_hodnoceni)
);

CREATE TABLE Termin_Otazka (
  id_cislo_otazky INT NOT NULL,
	CONSTRAINT con_cislo_otazky FOREIGN KEY(id_cislo_otazky) REFERENCES Otazka(id_cislo_otazky),
  id_kod_terminu INT NOT NULL,
 	CONSTRAINT con_kod_terminu FOREIGN KEY(id_kod_terminu) REFERENCES Termin(id_kod_terminu)
);

CREATE OR REPLACE TRIGGER auto_increment_osoba BEFORE 
INSERT ON Osoba
FOR EACH ROW
BEGIN
  SELECT id_osoba_seq.nextval
  INTO :new.id_osoba
  from dual;
END;
/

CREATE OR REPLACE TRIGGER rodne_cislo_kontrola BEFORE INSERT OR UPDATE ON Osoba
FOR EACH ROW 
DECLARE 
  den INTEGER;
  r_cislo INTEGER;
  mesic INTEGER;
  rok INTEGER;
  datum DATE;
BEGIN
  IF REGEXP_LIKE(:new.rodne_cislo, '^[0-9]{9,10}$') THEN
    den := CAST(SUBSTR(:new.rodne_cislo, 5, 2) AS INTEGER);
    mesic := CAST(SUBSTR(:new.rodne_cislo, 3, 2) AS INTEGER);
    rok := CAST(SUBSTR(:new.rodne_cislo, 1, 2) AS INTEGER);
  
    IF REGEXP_LIKE(:new.rodne_cislo, '^[0-9]{10}$') THEN
      IF NOT ((MOD(:new.rodne_cislo,11)) = 0) THEN
        RAISE_APPLICATION_ERROR(-20005, 'Rodne cislo '||:new.rodne_cislo||' je neplatne.');
      END IF;
    END IF;

        IF (((mesic BETWEEN 1 AND 12) OR (mesic BETWEEN 51 AND 62) OR (mesic BETWEEN 21 AND 32) OR (mesic BETWEEN 71 AND 82)) AND (den BETWEEN 1 AND 31)) THEN
          IF (LENGTH(:new.rodne_cislo) = 9 AND SUBSTR(:new.rodne_cislo, 7, 3) = '000') THEN
            RAISE_APPLICATION_ERROR(-20001, 'Rodne cislo '||:new.rodne_cislo||' je neplatne.');
          END IF;
          IF (mesic BETWEEN 51 AND 62) THEN
            mesic := mesic - 50;
          END IF;
          IF (mesic BETWEEN 21 AND 32) THEN
            mesic := mesic - 20;
          END IF;
          IF (mesic BETWEEN 71 AND 82) THEN
            mesic := mesic - 70;
          END IF;
        
          BEGIN
            datum := den||'.'||mesic||'.'||rok;
                EXCEPTION WHEN OTHERS THEN RAISE_APPLICATION_ERROR(-20000, 'Rodne cislo '||:new.rodne_cislo||' je neplatne.');
          END;
        ELSE
          RAISE_APPLICATION_ERROR(-20003, 'Rodne cislo '||:new.rodne_cislo||' je neplatne.');
        END IF;
  ELSE
    RAISE_APPLICATION_ERROR(-20002, 'Rodne cislo '||:new.rodne_cislo||' je neplatne.');
  END IF;
END;
/

--Aktualizuje Otazku na zaklade id_cislo_otazky . Pokud zadany id_cislo_otazky neexistuje zapise na output, ze nebylo nic pridane. Pokud je NULL, hodi vyjimku, jestlize byla nalezena jina chyba naprikald ve formatu vyhodi obecnou vyjimku. Pokud uspesne vykona proceduru vrati na output, uspesne vykonanani
CREATE OR REPLACE PROCEDURE update_otazka (id Otazka.id_cislo_otazky%type, typ Otazka.typ_otazky%type, zadani Otazka.zadani_otazky%type, max Otazka.max_pocet_bodu%type, min Otazka.min_pocet_bodu_pro_slozeni%type) AS
BEGIN
    DECLARE 
        test_na_id INT;
        exception_not_exists EXCEPTION;
        exception_null EXCEPTION;
    BEGIN
        IF(update_otazka.id IS NULL) THEN
            RAISE exception_null;
        END IF;
        
        SELECT COUNT(*) INTO test_na_id FROM Otazka WHERE Otazka.id_cislo_otazky = update_otazka.id;
        
        IF(test_na_id = 0) THEN
            RAISE exception_not_exists;
        END IF;
        
        UPDATE Otazka
        SET Otazka.typ_otazky = update_otazka.typ, Otazka.zadani_otazky = update_otazka.zadani, Otazka.max_pocet_bodu = update_otazka.max, Otazka.min_pocet_bodu_pro_slozeni = update_otazka.min
        WHERE Otazka.id_cislo_otazky = update_otazka.id;
    EXCEPTION
        WHEN exception_not_exists THEN 
            dbms_output.put_line('Kvuli chybnemu zadani id_cislo_otazky nebyla otazka aktualizovana - otazka s danym id neexistuje.');
        WHEN exception_null THEN
            RAISE_APPLICATION_ERROR(-20030, 'Na miste id_cislo_otazky je NULL!');
        WHEN OTHERS THEN
            RAISE_APPLICATION_ERROR(-20040, 'Doslo k chybe pri chodu procedury. Spatny format?');
    END;
    dbms_output.put_line('Otazka byla aktualizovana');
    
    IF(update_otazka.typ = 1) THEN
        dbms_output.put_line('Typ otazky je nyni fulltext');
    END IF;
    IF(update_otazka.typ = 2) THEN
        dbms_output.put_line('Typ otazky je nyni testove otazky');
    END IF;
    IF(update_otazka.typ = 3) THEN
        dbms_output.put_line('Typ otazky je nyni pocetni priklad');
    END IF;
    
END;
/

--Procedura vyuzivajici kurzor
--Vklada osobu s danym rodnym cislem, pokud se osoba jiz vyskytuje v databazi aktualizuje informace. 
--Nasledne vypise zda-li doslo k aktualizaci nebo ke vlozeni nove osoby. 
--Zaroven vlozi/aktualizuje osobu do tabulky studentu nebo ucitelu podle argumentu a do tabulky osoba_titul.
CREATE OR REPLACE PROCEDURE insert_or_actualize_person(rodne_cislo Osoba.rodne_cislo%TYPE, jmeno Osoba.jmeno%TYPE, prijmeni Osoba.prijmeni%TYPE, datum Osoba.datum_narozeni%TYPE, typ VARCHAR, titul_pred Osoba_titul.id_titul_pred%TYPE, titul_za Osoba_titul.id_titul_za%TYPE) AS
BEGIN
    DECLARE
        CURSOR osoba_seq IS SELECT last_number FROM all_sequences WHERE sequence_name = 'id_osoba_seq';
        CURSOR person_exist_test IS
            SELECT id_osoba, jmeno, prijmeni, datum_narozeni, rodne_cislo
            FROM Osoba;
        CURSOR student_exist_test IS
            SELECT id_osoba
            FROM student;
        CURSOR vyucujici_exist_test IS
            SELECT id_osoba
            FROM vyucujici;
        id_osoba_test INT;
        id_osoba_st_vy INT;
        id_osoba_exist INT;
        jmeno VARCHAR(120);
        prijmeni VARCHAR(120);
        datum_narozeni VARCHAR(80);
        rodne_cislo VARCHAR(60);  
    BEGIN
        id_osoba_test := 0;
        OPEN person_exist_test;
        LOOP
            FETCH person_exist_test INTO id_osoba_exist, jmeno, prijmeni, datum_narozeni, rodne_cislo;
            EXIT WHEN person_exist_test%NOTFOUND;
            IF(rodne_cislo = insert_or_actualize_person.rodne_cislo) THEN
                id_osoba_test := 1;
                UPDATE Osoba
                SET Osoba.rodne_cislo = insert_or_actualize_person.rodne_cislo, Osoba.jmeno = insert_or_actualize_person.jmeno, Osoba.prijmeni = insert_or_actualize_person.prijmeni, Osoba.datum_narozeni = insert_or_actualize_person.datum
                WHERE Osoba.id_osoba = id_osoba_exist;
                
                UPDATE Osoba_titul
                SET Osoba_titul.id_titul_pred = insert_or_actualize_person.titul_pred, Osoba_titul.id_titul_za = insert_or_actualize_person.titul_za
                WHERE Osoba_titul.id_osoba_s_titulem = id_osoba_exist;
                
                IF(insert_or_actualize_person.typ = 'Student') THEN
                    OPEN student_exist_test;
                    LOOP
                        FETCH student_exist_test INTO id_osoba_st_vy;
                        EXIT WHEN student_exist_test%NOTFOUND;
                        IF(id_osoba_exist = id_osoba_st_vy) THEN
                            id_osoba_test := 2;
                        END IF;
                    END LOOP;
                    CLOSE student_exist_test;
                    IF NOT(id_osoba_test = 2) THEN
                         INSERT INTO student(id_osoba) VALUES (id_osoba_exist);
                    END IF;
                ELSE
                    OPEN vyucujici_exist_test;
                    LOOP
                        FETCH vyucujici_exist_test INTO id_osoba_st_vy;
                        EXIT WHEN vyucujici_exist_test%NOTFOUND;
                        IF(id_osoba_exist = id_osoba_st_vy) THEN
                            id_osoba_test := 2;
                        END IF;
                    END LOOP;
                    CLOSE vyucujici_exist_test;
                    IF NOT(id_osoba_test = 2) THEN
                         INSERT INTO vyucujici(id_osoba) VALUES (id_osoba_exist);
                    END IF;
                END IF;
                dbms_output.put_line('Doslo k aktualizaci osoby');
            END IF;
        END LOOP;
        CLOSE person_exist_test;
        
        IF(id_osoba_test = 0) THEN
        INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,insert_or_actualize_person.datum,insert_or_actualize_person.jmeno,insert_or_actualize_person.prijmeni,insert_or_actualize_person.rodne_cislo);
        OPEN osoba_seq;
        FETCH osoba_seq INTO id_osoba_exist;
          INSERT INTO Osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (id_osoba_exist+1,insert_or_actualize_person.titul_pred, insert_or_actualize_person.titul_za);
          IF(insert_or_actualize_person.typ = 'Student') THEN
            INSERT INTO student(id_osoba) VALUES (id_osoba_exist+1);
          ELSE
            INSERT INTO vyucujici(id_osoba) VALUES (id_osoba_exist+1);
          END IF;  
        CLOSE osoba_seq;
        dbms_output.put_line('Doslo k vlozeni osoby');
        END IF;
    END;
END;
/      
        





INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1950-07-08','Petr','Dvorak','500708019');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1996-08-08','Jiri','Novak','9608080020');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1942-04-10','Ondra','Valach','420410005');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1997-10-21','Michal','Hodes','9709210104');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1995-09-21','Petr','Stastny','9509210106');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1995-09-28','Petr','Rohit','9509284928');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1991-08-07','Domingo','Coenraad','9108070202');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1990-09-15','Olufunmilola','Siemen','9009150106');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1953-05-05','Petr','Prabhakara','530505005');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1952-03-24','Zdenka','Premek','520324099');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1992-04-15','Vladimira','Marketa','9204151000');
INSERT INTO Osoba(id_osoba,datum_narozeni,jmeno,prijmeni,rodne_cislo) VALUES (NULL,'1996-12-24','Lubos','Katka','9612241001');

INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (12,NULL,NULL);
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (11,NULL,NULL);
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (10,'Ing.','Ph.D');
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (9,'Ing.','Ph.D, CSc.');
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (8,NULL,NULL);
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (7,NULL,NULL);
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (6,NULL,NULL);
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (5,NULL,NULL);
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (4,NULL,NULL);
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (3,'Ing.','Ph.D, CSc.');
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (2,NULL,NULL);
INSERT INTO osoba_titul(id_osoba_s_titulem, id_titul_pred, id_titul_za) VALUES (1,'Ing.',NULL);

INSERT INTO student(id_osoba) VALUES (12);
INSERT INTO student(id_osoba) VALUES (11);
INSERT INTO vyucujici(id_osoba) VALUES (10);
INSERT INTO vyucujici(id_osoba) VALUES (9);
INSERT INTO Student(id_osoba) VALUES (8);
INSERT INTO Student(id_osoba) VALUES (7);
INSERT INTO Student(id_osoba) VALUES (6);
INSERT INTO Student(id_osoba) VALUES (5);
INSERT INTO Student(id_osoba) VALUES (4);
INSERT INTO Vyucujici(id_osoba) VALUES (3);
INSERT INTO Student(id_osoba) VALUES (2);
INSERT INTO Vyucujici(id_osoba) VALUES (1);

INSERT INTO Predmet(id_kod_predmetu,pocet_kreditu,id_garant) VALUES ('IDS',5,3);
INSERT INTO Predmet(id_kod_predmetu,pocet_kreditu,id_garant) VALUES ('IZP',7,1);

INSERT INTO vyucujici_predmetu(id_vyucujici, id_kod_predmetu) VALUES (3, 'IDS');
INSERT INTO vyucujici_predmetu(id_vyucujici, id_kod_predmetu) VALUES (9, 'IDS');
INSERT INTO vyucujici_predmetu(id_vyucujici, id_kod_predmetu) VALUES (1, 'IZP');
INSERT INTO vyucujici_predmetu(id_vyucujici, id_kod_predmetu) VALUES (10, 'IZP');


INSERT INTO Zkouska(id_kod_zkousky,typ,max_pocet_bodu,min_pocet_bodu_pro_slozeni,pocet_terminu,max_pocet_terminu_pro_studenta,id_kod_predmetu) VALUES (id_kod_zkousky_seq.nextval,'Pulsemestralni zkouska',10,5,1,1,'IDS');
INSERT INTO Zkouska(id_kod_zkousky,typ,max_pocet_bodu,min_pocet_bodu_pro_slozeni,pocet_terminu,max_pocet_terminu_pro_studenta,id_kod_predmetu) VALUES (id_kod_zkousky_seq.nextval,'Pulsemestralni zkouska',10,5,1,1,'IZP');

INSERT INTO Termin(id_kod_terminu,datum_cas,misto,zacatek_prihlasovani,konec_prihlasovani,max_pocet_studentu,cislo_terminu,id_kod_zkousky) VALUES (id_kod_terminu_seq.nextval,'1996-07-08 22:00','E112','21.4.','28.4.',10,1,1);
INSERT INTO Termin(id_kod_terminu,datum_cas,misto,zacatek_prihlasovani,konec_prihlasovani,max_pocet_studentu,cislo_terminu,id_kod_zkousky) VALUES (id_kod_terminu_seq.nextval,'1996-07-09 22:00','E112','22.4.','29.4.',10,1,2);

INSERT INTO Otazka(id_cislo_otazky,max_pocet_bodu,min_pocet_bodu_pro_slozeni,typ_otazky,zadani_otazky) VALUES (id_cislo_otazky_seq.nextval,5,0,1,'Spocitej 1+18');
INSERT INTO Otazka(id_cislo_otazky,max_pocet_bodu,min_pocet_bodu_pro_slozeni,typ_otazky,zadani_otazky) VALUES (id_cislo_otazky_seq.nextval,5,0,1,'Spocitej 18/2');
INSERT INTO Otazka(id_cislo_otazky,max_pocet_bodu,min_pocet_bodu_pro_slozeni,typ_otazky,zadani_otazky) VALUES (id_cislo_otazky_seq.nextval,5,0,1,'Spocitej 18/9');
INSERT INTO Otazka(id_cislo_otazky,max_pocet_bodu,min_pocet_bodu_pro_slozeni,typ_otazky,zadani_otazky) VALUES (id_cislo_otazky_seq.nextval,5,0,1,'Spocitej 18/3');

INSERT INTO Student_Predmet(id_student,id_predmet) VALUES (2,'IDS');
INSERT INTO Student_Predmet(id_student,id_predmet) VALUES (4,'IDS');
INSERT INTO Student_Predmet(id_student,id_predmet) VALUES (5,'IDS');
INSERT INTO Student_Predmet(id_student,id_predmet) VALUES (6,'IDS');
INSERT INTO Student_Predmet(id_student,id_predmet) VALUES (7,'IZP');
INSERT INTO Student_Predmet(id_student,id_predmet) VALUES (8,'IZP');
INSERT INTO Student_Predmet(id_student,id_predmet) VALUES (8,'IDS');
INSERT INTO Student_Predmet(id_student,id_predmet) VALUES (11,'IZP');
INSERT INTO Student_Predmet(id_student,id_predmet) VALUES (12,'IZP');

INSERT INTO hodnoceni(id_kod_hodnoceni,pocet_bodu,procentualni_uspesnost,percentilova_uspesnost,komentar,id_vyucujici,id_student) VALUES (id_kod_hodnoceni_seq.nextval,5,100,100,' ',1,7);
INSERT INTO hodnoceni(id_kod_hodnoceni,pocet_bodu,procentualni_uspesnost,percentilova_uspesnost,komentar,id_vyucujici,id_student) VALUES (id_kod_hodnoceni_seq.nextval,5,100,100,' ',1,8);
INSERT INTO hodnoceni(id_kod_hodnoceni,pocet_bodu,procentualni_uspesnost,percentilova_uspesnost,komentar,id_vyucujici,id_student) VALUES (id_kod_hodnoceni_seq.nextval,0,0,0,' ',1,11);
INSERT INTO hodnoceni(id_kod_hodnoceni,pocet_bodu,procentualni_uspesnost,percentilova_uspesnost,komentar,id_vyucujici,id_student) VALUES (id_kod_hodnoceni_seq.nextval,5,100,100,' ',1,12);

INSERT INTO hodnoceni(id_kod_hodnoceni,pocet_bodu,procentualni_uspesnost,percentilova_uspesnost,komentar,id_vyucujici,id_student) VALUES (id_kod_hodnoceni_seq.nextval,5,100,100,'',1,7);
INSERT INTO hodnoceni(id_kod_hodnoceni,pocet_bodu,procentualni_uspesnost,percentilova_uspesnost,komentar,id_vyucujici,id_student) VALUES (id_kod_hodnoceni_seq.nextval,3,66.6,66.6,'',1,8);
INSERT INTO hodnoceni(id_kod_hodnoceni,pocet_bodu,procentualni_uspesnost,percentilova_uspesnost,komentar,id_vyucujici,id_student) VALUES (id_kod_hodnoceni_seq.nextval,0,0,0,'',1,11);
INSERT INTO hodnoceni(id_kod_hodnoceni,pocet_bodu,procentualni_uspesnost,percentilova_uspesnost,komentar,id_vyucujici,id_student) VALUES (id_kod_hodnoceni_seq.nextval,0,0,0,'',1,12);

INSERT INTO otazka_hodnoceni(id_cislo_otazky,id_kod_hodnoceni) VALUES (1,1);
INSERT INTO otazka_hodnoceni(id_cislo_otazky,id_kod_hodnoceni) VALUES (1,2);
INSERT INTO otazka_hodnoceni(id_cislo_otazky,id_kod_hodnoceni) VALUES (1,3);
INSERT INTO otazka_hodnoceni(id_cislo_otazky,id_kod_hodnoceni) VALUES (1,4);

INSERT INTO otazka_hodnoceni(id_cislo_otazky,id_kod_hodnoceni) VALUES (2,5);
INSERT INTO otazka_hodnoceni(id_cislo_otazky,id_kod_hodnoceni) VALUES (2,6);
INSERT INTO otazka_hodnoceni(id_cislo_otazky,id_kod_hodnoceni) VALUES (2,7);
INSERT INTO otazka_hodnoceni(id_cislo_otazky,id_kod_hodnoceni) VALUES (2,8);

INSERT INTO prihlaseni_na_termin(id_kod_terminu,id_osoba) VALUES (1,7);
INSERT INTO prihlaseni_na_termin(id_kod_terminu,id_osoba) VALUES (1,8);
INSERT INTO prihlaseni_na_termin(id_kod_terminu,id_osoba) VALUES (1,11);
INSERT INTO prihlaseni_na_termin(id_kod_terminu,id_osoba) VALUES (1,12);

CALL update_otazka(1, 1, 'Co je treba splnit pri BNFS', 6, 1);
CALL update_otazka(2, 2, 'Co je treba splnit pri BNFS'||chr(10)||'a) Druhou normalni formu'||chr(10)||'b) Treti normalni formu'||chr(10)||'c) Vse vyse', 6, 1);
CALL update_otazka(3, 3, 'Normalizujte tabulky [c_stud, jmeno_s, zkratka, nazev, kredity]'||chr(10)||'[body, os_c_gar, jmeno_g, ak_rok]', 6, 1);
CALL update_otazka(NULL, 1, 'Co je treba splnit pri BNFS', 6, 1);
CALL update_otazka(100, 1, 'Co je treba splnit pri BNFS', 6, 1);


CALL insert_or_actualize_person('9509284928', 'Tomas', 'Holik', '1995-09-28', 'Student', NULL, NULL);
CALL insert_or_actualize_person('9004020201', 'Marek', 'Ciliak', '1990-04-02', 'Student', NULL, NULL);
CALL insert_or_actualize_person('8001010204', 'Jozef', 'Kovacik', '1980-01-01', 'Vyucujici', 'Ing.', NULL);

--poradi select
--SELECT column(s)
--FROM table_name(s)
--WHERE condition
--GROUP BY column(s)
--HAVING condition
--ORDER BY column(s);

--vybere vsechny osoby s titulem
SELECT osoba_titul.id_osoba_s_titulem, osoba_titul.id_titul_pred, osoba_titul.id_titul_za, Osoba.id_osoba, Osoba.jmeno, Osoba.prijmeni
FROM osoba
INNER JOIN Osoba_titul ON id_osoba_s_titulem = id_osoba AND (Osoba_titul.id_titul_za IS NOT NULL OR Osoba_titul.id_titul_pred IS NOT NULL);

--vybere korespondujici predmet k danemu terminu
SELECT t.id_kod_terminu, p.id_kod_predmetu
FROM termin t
INNER JOIN zkouska z ON t.id_kod_zkousky = z.id_kod_zkousky
INNER JOIN predmet p ON z.id_kod_predmetu = p.id_kod_predmetu;

--vybere vsechny Petry z osob, kteri studuji IDS
SELECT osoba.*
FROM Osoba, student_predmet
WHERE jmeno IN('Petr') and osoba.id_osoba = student_predmet.id_student and student_predmet.id_predmet = 'IDS';

--vybere vsechny studenty, kteri studuji jen IZP

SELECT DISTINCT osoba.*
FROM student, student_predmet, hodnoceni, osoba
WHERE student.id_osoba = osoba.id_osoba AND student.id_osoba = student_predmet.id_student AND student_predmet.id_predmet = 'IZP' AND NOT EXISTS 
                                                                    (SELECT *
                                                                    FROM student_predmet
                                                                    WHERE student.id_osoba = student_predmet.id_student AND student_predmet.id_predmet<>'IZP');
                                                                  

--pocet vyucujicich v jednotlivych predmetech, ktere maji vice jak jednoho vyucujiciho
SELECT id_kod_predmetu, COUNT(id_vyucujici) 
FROM Vyucujici_Predmetu
GROUP BY id_kod_predmetu
HAVING COUNT(id_vyucujici) > 1
ORDER BY COUNT(id_vyucujici) DESC;

--pocet studentu prihlasenych na jednotlive terminy zkousek
SELECT COUNT(p.id_osoba), p.id_kod_terminu
FROM prihlaseni_na_termin p
GROUP BY p.id_kod_terminu
ORDER BY COUNT(p.id_osoba);

--vybere vsechny vyucujici s titulem Ph.D a bez titulu CSc..
SELECT osoba_titul.id_osoba_s_titulem, osoba_titul.id_titul_za, vyucujici.id_osoba
FROM vyucujici, osoba_titul
WHERE id_osoba = id_osoba_s_titulem AND id_titul_za like '%Ph.D%' AND id_titul_za not like '%CSc.%';
  
--vybere vsechny studenty, kteri studuji IZP nebo IDS
SELECT Osoba.id_osoba, Osoba.jmeno, Osoba.prijmeni
FROM osoba
WHERE osoba.id_osoba IN (SELECT student.id_osoba
                          FROM Student
                          INTERSECT
                          SELECT student_predmet.id_student
                          FROM Student_Predmet
                          WHERE id_predmet IN('IZP') OR id_predmet IN('IDS'));

--vybere vsechny studenty, prihlasene na termin pulsemestralky z IDS, kteri jsou narozeni v roce 1996
SELECT Osoba.id_osoba, Osoba.jmeno, Osoba.prijmeni
FROM osoba
WHERE to_char(osoba.datum_narozeni) LIKE '%96' AND osoba.id_osoba IN (SELECT id_osoba
                                                                      FROM Prihlaseni_na_termin
                                                                      WHERE id_kod_terminu IN (SELECT id_kod_terminu
                                                                                                FROM termin
                                                                                                WHERE id_kod_zkousky IN (SELECT id_kod_zkousky
                                                                                                                          FROM Zkouska
                                                                                                                         WHERE Zkouska.id_kod_predmetu in ('IDS') and zkouska.typ IN ('Pulsemestralni zkouska'))));


EXPLAIN PLAN FOR
SELECT O.jmeno, COUNT(*) pocet 
FROM Osoba O
NATURAL JOIN Student S
GROUP BY O.jmeno;

SELECT PLAN_TABLE_OUTPUT FROM TABLE(dbms_xplan.display);

CREATE INDEX PersonIndex ON Osoba (jmeno, prijmeni, rodne_cislo);

EXPLAIN PLAN FOR
SELECT O.jmeno, COUNT(*) pocet 
FROM Osoba O
NATURAL JOIN Student S
GROUP BY O.jmeno;

SELECT PLAN_TABLE_OUTPUT FROM TABLE(dbms_xplan.display);

DROP INDEX PersonIndex; 

-- Pristupy pro vyucujiciho
GRANT SELECT ON Student TO XHOLIK13;
GRANT SELECT ON Vyucujici TO XHOLIK13;
GRANT SELECT ON Predmet TO XHOLIK13;
GRANT SELECT ON Vyucujici_Predmetu TO XHOLIK13;
GRANT SELECT,INSERT,DELETE ON Prihlaseni_na_termin TO XHOLIK13;
GRANT SELECT,INSERT,DELETE ON Student_Predmet TO XHOLIK13;

GRANT ALL ON Zkouska TO XHOLIK13;
GRANT ALL ON Termin TO XHOLIK13;
GRANT ALL ON Otazka TO XHOLIK13;
GRANT ALL ON Hodnoceni TO XHOLIK13;
GRANT ALL ON Otazka_Hodnoceni TO XHOLIK13;
GRANT ALL ON Termin_Otazka TO XHOLIK13;

-- Nutny View log pro fast refresh - vytvari snapshot, ktery refreshne materialized view
DROP MATERIALIZED VIEW Count_Student_IDS;

CREATE MATERIALIZED VIEW LOG ON Student_Predmet WITH ROWID(id_student,id_predmet) INCLUDING NEW VALUES;
CREATE MATERIALIZED VIEW Count_Student_IDS
  REFRESH FAST ON COMMIT
  AS 
  SELECT Count(P.id_student) as "Pocet studentu", P.id_predmet as "Predmet" FROM student_predmet P GROUP BY id_predmet;

GRANT ALL ON Count_Student_IDS TO XHOLIK13;

SELECT * FROM Count_Student_IDS;
INSERT INTO Student_Predmet(id_student,id_predmet) VALUES (12,'IZP');
COMMIT;
SELECT * FROM Count_Student_IDS;

    