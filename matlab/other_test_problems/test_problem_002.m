addpath('~/local/bin/qp_mex')

clear;clc
format long

for n = 2:2:10
    [H,h,C,c,x_ref] = problem_002(n);
    [X,F] = solve_and_verify(x_ref,H,h,C,c);
end
