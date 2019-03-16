xx=linspace(-1,1,51);
a0=1;
y0=ones(1,51);
a1=[1,0];
y1=polyval(a1,xx);
a2=[1,0,0];
y2=polyval(a2,xx);
y3=zeros(1,51);
y3(26:51)=(xx(26:51)-0).^2;

subplot(2,2,1);
plot(xx,y0);
subplot(2,2,2);
plot(xx,y1);
subplot(2,2,3);
plot(xx,y2);
subplot(2,2,4);
plot(xx,y3);