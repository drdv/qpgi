function [H,h,C,c,x_ref] = problem_006()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problem 1-10"
%%%
%%% problem 21 from
%%% "Test Examples for Nonlinear Programming Codes: Hock-Schittkowski-Collection"
%%%

    H = 2*diag([0.01,1]);
    h = [0;0];

    C = [-10, 1; 1, 0; 0, 1; -1, 0; 0,-1];
    c = [-10;50;50;-2;50];

    x_ref = [2;0];

%%%EOF
