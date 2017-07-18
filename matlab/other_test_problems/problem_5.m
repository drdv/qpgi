function [H,h,C,c,x_ref] = problem_5()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problem 1-10"
%%%
%%% problem 3 from
%%% "Test Examples for Nonlinear Programming Codes: Hock-Schittkowski-Collection"
%%%

    H = 2*10^-5*[1 -1;-1 1];
    h = [0;1];

    C = [0,-1];
    c = 0;

    x_ref = [0;0];

%%%EOF
