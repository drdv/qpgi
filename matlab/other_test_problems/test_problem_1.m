addpath('~/local/bin/qp_mex')

clear;clc
format long

for n=3:3:9
    [H,h,C,c,x_ref] = problem_1(n);
    [X,F] = solve_and_verify(x_ref,H,h,C,c);
end
