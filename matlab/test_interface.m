addpath('./matlab_implementation')
addpath('~/local/bin/qp_mex')

clear;clc

Inf = 1e10;

H = [1 -1; -1 2];
h = [-2; -6];
C = [1 1; -1 2; 2 1; 1 0; 0 1];
c_lb = [-Inf; -Inf; -Inf; 0; 0];
c_ub = [2; 2; 3; Inf; Inf];

x_ref = [0.666666666666667; 1.333333333333333];

[x, exitflag, u, iter] = qpgi(H, h, C, c_lb, c_ub);

fprintf('exitflag  = %s\n', exitflag)
fprintf('err(x)    = %e\n', norm(x-x_ref))
fprintf('numb iter = %d\n', iter)
