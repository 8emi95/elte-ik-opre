function [ gyokok ] = csebisev_gyokok( n,a,b )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
gyokok=[];
for k=0:n-1,
    gyokok=[(cos(2*k+1)/(2*n)*pi),gyokok];
end
if b-a~=2
    for i=1:n,
        gyokok(i)=gyokok(i)*(b-a)+a+1;
    end
else
    for i=1:n,
        gyokok(i)=gyokok(i)+a+1;
    end
end
end

