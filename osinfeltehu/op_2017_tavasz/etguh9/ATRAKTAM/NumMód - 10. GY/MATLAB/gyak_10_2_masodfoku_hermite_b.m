%herite peremfelt�tellel, t�bb alapponttal

xx = linspace(-1,5,100);

yy = sin(pi/2 * xx);

x = -1:5;   %alappontok
y = [0, sin(pi/2*x), 0];   %fv. �rt�kek + a deriv�ltak a vektor k�t sz�l�n

ss = Spline(x,y,xx);

plot(xx,yy,xx,ss)

hiba = max(abs(yy-ss));   %hiba
disp hiba