%herite peremfeltétellel, több alapponttal

xx = linspace(-1,5,100);

yy = sin(pi/2 * xx);

x = -1:5;   %alappontok
y = [0, sin(pi/2*x), 0];   %fv. értékek + a deriváltak a vektor két szélén

ss = Spline(x,y,xx);

plot(xx,yy,xx,ss)

hiba = max(abs(yy-ss));   %hiba
disp hiba