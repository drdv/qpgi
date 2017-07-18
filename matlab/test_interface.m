addpath('./matlab_implementation')

clear;clc

if 1
    H = [1 -1; -1 2];
    h = [-2; -6];
    C = [1 1; -1 2; 2 1; -1 0; 0 -1];
    c = [2; 2; 3; 0; 0];
else
    n = 100;
    m = 60;
    H = randn(n,n); H = H'*H;
    h = randn(n,1);
    C = randn(m,n);
    c = randn(m,1);
end

[x,u,W,status,iter] = qpgi_matlab(H,h,C,c,false);

[x1, status1, u1, iter1] = qpgi(H, h, C, c);

norm(x-x1)

%{
options = optimset('Algorithm','interior-point-convex','Display','off');
[x0,fval,exitflag,output,lambda] = quadprog(H,h,C,c,[],[],[],[],[],options);
fprintf('error = %e, %e \n', norm(x0-x), norm(H*x + h + C'*u))
%}
