addpath('~/local/bin/qp_mex')

clear;clc

format long
[H,h,C,c,x_ref] = problem_3();

[X,F] = solve_and_verify(x_ref,H,h,C,c);

H = H+1e-14*eye(2);

fprintf('\nWITH regularization\n')
[X,F] = solve_and_verify(x_ref,H,h,C,c);
