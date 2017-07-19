addpath('~/local/bin/qp_mex')

clear;clc
format long

[H,h,C,c,x_ref] = problem_007();
[X,F] = solve_and_verify(x_ref,H,h,C,c);
