function [ hiba_e, hiba_cs ] = gyak_8_feladat_4( n )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

    x1 = linspace(-1, 1, n+1);
    k = 0:n;
    x2 = cos((2*k+1) * pi/2 / (n+1));
    
    w1 = poly(x1);
    w2 = poly(x2);
    
    xx = linspace(-1, 1, 100);
    ww1 = polyval(w1, xx);
    ww2 = polyval(w2, xx);
    plot(xx, ww1, xx, ww2);
    legend('ekvidisztans', 'csebisev');
    
    hiba_e = max(abs(w1));
    hiba_cs = max(abs(w2));

end

