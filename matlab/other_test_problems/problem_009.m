function [H,h,C,c,x_ref] = problem_009()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problem 1-10"
%%%
%%% problem 48 from
%%% "Test Examples for Nonlinear Programming Codes: Hock-Schittkowski-Collection"
%%%

    H = 2*[1  0  0  0  0;
	   0  1 -1  0  0;
	   0 -1  1  0  0;
	   0  0  0  1 -1;
	   0  0  0 -1  1];
    h = [-2;0;0;0;0];

    A = [1, 1, 1, 1,  1;
	 0, 0, 1,-2, -2];
    b = [5;-3];

    C = [A;-A];
    c = [b;-b];

    x_ref = ones(5,1);

%%%EOF
