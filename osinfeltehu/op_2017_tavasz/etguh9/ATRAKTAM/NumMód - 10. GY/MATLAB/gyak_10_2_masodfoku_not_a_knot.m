%not-a-knot peremfeltétellel, a MATLAB Spline fv-ével
%(elég rossz közelítés, ui. a peremfeltételek elvesznek)

xx=linspace(-1,1,51);

yy = sin(pi/2 * xx);

x = [-1, 0, 1];   %alappontok
y = [-1, 0, 1];   %fv. értékek

ss = Spline(x,y,xx);

plot(xx,yy,xx,ss)