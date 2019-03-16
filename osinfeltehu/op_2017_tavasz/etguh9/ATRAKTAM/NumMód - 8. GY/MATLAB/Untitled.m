% Készítsünk iterációt adott alappontú inverz interpoláció felhasználásával az
% f(x) = sin(x) - 1/2 = 0 egyenlet megoldására

x = [0,1/2,1];

for i = 1:3
    disp(['i = ', num2str(i)]);
    y = sin(x) - 1/2;
    p = polyfit(y,x,2);

    disp(['polyval(p,0) = ', num2str(polyval(p,0))]);
    x(2) = polyval(p,0);   %x(2) = polyval(p,0);
    disp(['x = ', num2str(x)]);
    disp(['ellenorzes: ', num2str(fzero(@(x) sin(x)-1/2,0,1))]);
end