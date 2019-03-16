x1 = 1:4; 
y1 = sin(pi/2*x1);
x2 = 0:5;
y2 = [1,y1,0];

xx = linspace(0,5,50);
yy = sin(pi/2*xx);

yy1 = pchip(x2,y2,xx);
yy2 = spline(x2,y2,xx); % ’not a knot’ spline

plot(x2,y2,'o',xx,yy,xx,yy1,xx,yy2)
legend('adatok','sin','pchip','spline',4)