A=[1,-1,1,0;1,0,0,0;1,1,1,1;0,1,-2,0];
b=[-1,0,1,0]';
c=A\b;

xx=linspace(-1,1,51);

%kiszedj�k c els� h�rom elem�t �s megford�tjuk:
p = flipud(c(1:3));   %flipud - megcser�li a fentet �s a lentet, fliplr lenne a m�sik

pp = polyval(p, xx);
ss = pp + c(4) * y3;
yy = sin(pi/2 * xx);

y3=zeros(1,51);
y3(26:51)=(xx(26:51)-0).^2;

plot(xx,yy,xx,ss);
legend('fv.', 'spline')