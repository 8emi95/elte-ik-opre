x2 = 0:5;
y2 = sin(pi/2*x2);

xx = linspace(0,5,50);
yy = sin(pi/2*xx);

yy1 = pchip(x2,y2,xx);
yy2 = spline(x2,[pi/2,y2,0],xx); % Hermite peremfeltételû spline

plot(x2,y2,'o',xx,yy,xx,yy1,xx,yy2);
legend('adatok','sin','pchip','spline',4)