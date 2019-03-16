%van egy line�ris folyamatunk, aminek 2*x+1 az elm�leti v�ltoz�sa
%x-et akarjuk becs�lni tapasztalati �rt�kek alapj�n

x = 1:10;
y_pontos = 2*x+1;
y_mert = y_pontos + (rand(1,10)-1/2)/5;

%most illessz�nk line�ris fv-t az adatokra:
p = polyfit(x, y_mert, 1);
xx = linspace(1, 10, 100);
pp = polyval(p, xx);

plot(x, y_mert, '*', xx, pp)

hiba = sum((y_pontos-polyval(p,x)).^2);
disp(['hiba = ', num2str(hiba)])
disp(p)   %ilyen fasz�n k�zel�t�nk
disp(['p = ', num2str(p[1])])