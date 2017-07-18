addpath('../matlab_implementation')
addpath('~/local/bin/qp_mex')

clear;clc

tolerance = 1e-09;

file_name = './matlab/alexInfeas2.mat';

load(file_name)

numb_var = length(g);

C = [eye(numb_var);-eye(numb_var)];
c = [ub;-lb];
C = [C;A;-A];
c = [c;ubA;-lbA];

try
    chol(H);
catch
    %% here we should exit due to problems when factorizing H
    [x0,exitflag0] = qpgi(H,g,C,c);

    disp('WARNING: REGULARIZE HESSIAN')
    H = H + 0.001*eye(size(H));
end

%% here we should recognize that the problem is infeasible
[x,exitflag] = qpgi(H,g,C,c);

fprintf('exitflag0 = %s\n', exitflag0)
fprintf('exitflag  = %s\n', exitflag)
