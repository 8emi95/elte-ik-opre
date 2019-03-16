/* 1 */
select owner from DBA_INDEXES where table_name in (select table_name from DBA_PART_TABLES) and index_type='BITMAP' group by owner,table_name having count(*)>=2;

/* 2 */ 
select * from 
(select owner,sum(bytes) from dba_segments where segment_type='INDEX' group by owner order by sum(bytes) desc)
where rownum=1;

/* 3 */
(select owner,table_name,max(column_id) from dba_tab_columns group by owner,table_name
intersect
select owner,table_name,column_id from dba_tab_columns where data_type='DATE')
intersect
(select owner,table_name,max(column_id)-1 from dba_tab_columns group by owner,table_name
intersect
select owner,table_name,column_id from dba_tab_columns where data_type='DATE');

/* 4 */
select * from 
(select tablespace_name,count(*) from DBA_TABLES where table_name in ( select distinct table_name from DBA_INDEXES) group by tablespace_name order by count(*) asc)
where rownum=1;

/* 5 */
SELECT owner, table_name FROM dba_tables 
WHERE owner='NIKOVITS' AND iot_type = 'IOT'
intersect
select owner, table_name from dba_tab_columns where owner='NIKOVITS' AND column_id=2;

/* 6 */
select * from DBA_TAB_PARTITIONS where table_owner = 'SH' and table_name = 'SALES' and NUM_ROWS>3000;


