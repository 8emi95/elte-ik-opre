%herite peremfelt�tellel

xx = linspace(-1,1,51);

yy = sin(pi/2 * xx);

x = [-1, 0, 1];   %alappontok
y = [0, -1, 0, 1, 0];   %fv. �rt�kek + a deriv�ltak a vektor k�t sz�l�n

ss = Spline(x,y,xx);

plot(xx,yy,xx,ss)

hiba = max(abs(yy-ss));   %hiba
disp hiba