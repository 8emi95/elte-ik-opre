function [p] = absekvid(n)
% Az abs(x) fv-t interpolálja egyenletes alappontrendszeren
% Hívása: p = absekvid(11)

    x = linspace(-1,1,n+1);
    y = abs(x);
    p = polyfit(x,y,n);
    xx = linspace(-1,1,100); 
    yy = polyval(p,xx); 
    yf = abs(xx);
    plot(x,y,'x',xx,yy,xx,yf);
    legend('pontok','intpol','fv')
end
