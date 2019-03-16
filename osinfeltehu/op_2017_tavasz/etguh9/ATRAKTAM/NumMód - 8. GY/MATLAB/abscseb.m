function [p] = abscseb(n)
% Az abs(x) fv-t interpolálja Csebisev alappontrendszeren
% Hívása: p = abscseb(11)


    k = [0:n]; 
    x = cos((1+2*k)*pi/2/(n+1)); 
    y = abs(x);
    p = polyfit(x,y,n);
    xx = linspace(-1,1,100);
    yy = polyval(p,xx); 
    yf = abs(xx);
    plot(x,y,'x',xx,yy,xx,yf);
    legend('pontok','intpol','fv')
end
