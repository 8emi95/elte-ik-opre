select d1.dnev
from dolgozo d1 join dolgozo d2 on (d1.fonoke = d2.dkod)
where d2.fonoke = (select dkod from dolgozo where dnev = 'KING');

select d.dnev
from dolgozo d
where 2000 >= (select max(fizetes) from dolgozo where fonoke = d.dkod);

select nvl(o.onev,'Ismeretlen'), sum(fizetes)
from dolgozo d left outer join osztaly o on (d.oazon = o.oazon)
group by o.onev
having count(*) >= 2;

select o.telephely, nvl(count(d.dkod),0)
from dolgozo d right outer join osztaly o on (d.oazon = o.oazon)
group by o.telephely
having  min(fizetes) > 4000 or count(d.dkod) = 0;

delete from dolgozo
where fizetes = (select max(fizetes)
        from dolgozo d natural join osztaly o
        group by o.telephely
        having o.telephely = 'DALLAS');
        
update dolgozo d
set jutalek = nvl(jutalek,'0') + 1000
where 2 <= (select count(dkod) from dolgozo group by fonoke having fonoke = d.dkod);