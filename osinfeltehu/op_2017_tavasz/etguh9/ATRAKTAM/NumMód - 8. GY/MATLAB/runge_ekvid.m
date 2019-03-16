function [p] = runge_ekvid(n)
% A Runge fv-t interpol�lja egyenletes alappontrendszeren
% H�v�sa: p = runge_ekvid(11)

    x = linspace(-1,1,n+1);
    y = 1 ./ (1 + 25 * x.^2);
    p = polyfit(x,y,n);
    xx = linspace(-1,1,100); 
    yy = polyval(p,xx); 
    yf = 1 ./ (1 + 25 * xx.^2);
    plot(x,y,'x',xx,yy,xx,yf);
    legend('pontok','intpol','fv')
end
