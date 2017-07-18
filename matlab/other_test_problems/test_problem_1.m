addpath('..')

clear;clc

for n=3:3:9
    [H,h,C,c] = problem_1(n);
    [x,exitflag] = qpgi(H,h,C,c);
    x0 = verify_solution(x,H,h,C,c,exitflag);
end
