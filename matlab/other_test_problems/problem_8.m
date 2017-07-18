function [H,h,C,c,x_ref] = problem_8()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problem 1-10"
%%%
%%% problem 35 from
%%% "Test Examples for Nonlinear Programming Codes: Hock-Schittkowski-Collection"
%%%

    H = 2*[2 1 1;1 2 0; 1 0 1];
    h = -[8;6;4];

    C = [1 1 2; -eye(3)];
    c = [3;0;0;0];

    x_ref = [4/3;7/9;4/9];

%%%EOF
