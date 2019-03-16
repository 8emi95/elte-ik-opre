%van egy lineáris folyamatunk, aminek 2*x+1 az elméleti változása
%x-et akarjuk becsülni tapasztalati értékek alapján

x = 1:10;
y_pontos = 2*x+1;
y_mert = y_pontos + (rand(1,10)-1/2)/5;

%most illesszünk lineáris fv-t az adatokra:
p = polyfit(x, y_mert, 1);
xx = linspace(1, 10, 100);
pp = polyval(p, xx);

plot(x, y_mert, '*', xx, pp)

hiba = sum((y_pontos-polyval(p,x)).^2);
disp(['hiba = ', num2str(hiba)])
disp(p)   %ilyen faszán közelítünk
disp(['p = ', num2str(p[1])])