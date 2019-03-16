select dnev, oazon from dolgozo; --group by oazon;

select * from osztaly;

select o.telephely a
from dolgozo d full outer JOIN osztaly o
on d.oazon=o.OAZON 
group by o.TELEPHELY
having count(d.DNEV)=0 or min(d.FIZETES)>4000;

select d.dnev, o.telephely , d.FIZETES
from dolgozo d, osztaly o
where o.oazon=d.OAZON and o.TELEPHELY='DALLAS';

DELETE (select d.dnev, o.telephely , d.FIZETES
from dolgozo d, osztaly o
where o.oazon=d.OAZON and o.TELEPHELY='DALLAS' and
d.FIZETES >= ALL( select d.FIZETES
from dolgozo d, osztaly o
where o.oazon=d.OAZON and o.TELEPHELY='DALLAS'));

DELETE from dolgozo  osztaly 
where osztaly.oazon=dolgozo.OAZON and osztaly.TELEPHELY='DALLAS' and
dolgozo.FIZETES >= ALL( select d.FIZETES
from dolgozo d, osztaly o
where o.oazon=d.OAZON and o.TELEPHELY='DALLAS');