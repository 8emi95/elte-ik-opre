function [p] = runge_cseb(n)
% A Runge fv-t interpolálja Csebisev alappontrendszeren
% Hívása: p = runge_cseb(11)

    k = [0:n];
    x = cos((1+2*k)*pi/2/(n+1)); 
    y = 1 ./ (1 + 25 * x.^2);
    p = polyfit(x,y,n);
    xx = linspace(-1,1,100);
    yy = polyval(p,xx);
    yf = 1 ./ (1 + 25 * xx.^2);
    plot(x,y,'x',xx,yy,xx,yf);
    legend('pontok','intpol','fv')
end
