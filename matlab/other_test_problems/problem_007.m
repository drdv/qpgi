function [H,h,C,c,x_ref] = problem_007()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problem 1-10"
%%%
%%% problem 28 from
%%% "Test Examples for Nonlinear Programming Codes: Hock-Schittkowski-Collection"
%%%

    H = 2*[1 1 0;1 2 1; 0 1 1];
    h = [0;0;0];

    A = [1 2 3];
    b = 1;

    C = [A;-A];
    c = [b;-b];

    x_ref = [0.5;-0.5;0.5];

%%%EOF
