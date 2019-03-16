%not-a-knot peremfelt�tellel, a MATLAB Spline fv-�vel
%(el�g rossz k�zel�t�s, ui. a peremfelt�telek elvesznek)

xx=linspace(-1,1,51);

yy = sin(pi/2 * xx);

x = [-1, 0, 1];   %alappontok
y = [-1, 0, 1];   %fv. �rt�kek

ss = Spline(x,y,xx);

plot(xx,yy,xx,ss)