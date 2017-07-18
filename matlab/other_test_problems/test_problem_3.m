addpath('~/local/bin/qp_mex')

clear;clc

format long

%% input argument = 1: ill-conditioned Hessian
%% input argument = 0: singular        Hessian
[H,h,C,c,x_ref] = problem_3(1);

[X,F] = solve_and_verify(x_ref,H,h,C,c);

H = H+1e-14*eye(2);

fprintf('\nWITH regularization\n')
[X,F] = solve_and_verify(x_ref,H,h,C,c);

%% note: I know that internally QLD regularizes the Hessian if it is ill-conditioned.
%%       It seems that quadprogpp does the same.
