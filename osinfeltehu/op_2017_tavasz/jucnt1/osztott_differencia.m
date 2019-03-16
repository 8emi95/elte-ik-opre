function [ output_args ] = osztott_differencia( x,y )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
[n,m]=size(x);
A=zeros(m,m+1);
for i=1:m,
    A(i,1)=x(i);
    A(i,2)=y(i);
end
for j=2:m,
    for i=j:m,
        A(i,j+1)=(A(i,j)-A(i-1,j))/(x(i)-x(i-j+1));
    end
end
disp(A)

end

