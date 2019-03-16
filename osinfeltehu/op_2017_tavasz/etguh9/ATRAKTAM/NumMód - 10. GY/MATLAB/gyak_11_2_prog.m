x=1:10; y=[0.5 2 4 5 8 9 11 15 17 22];
p1=polyfit(x,y,1)
xx=1:0.5:10; 
yy1=polyval(p1,xx);
plot(x,y,'x',xx,yy1);
hiba1=sum((y-polyval(p1,x)).^2)