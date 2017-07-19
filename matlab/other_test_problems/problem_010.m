function [H,h,C,c,x_ref] = problem_010()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problem 1-10"
%%%
%%% problem 51 from
%%% "Test Examples for Nonlinear Programming Codes: Hock-Schittkowski-Collection"
%%%

    H = 2*[1 -1  0  0  0;
	  -1  2  1  0  0;
	   0  1  1  0  0;
	   0  0  0  1  0;
	   0  0  0  0  1];
    h = [0;-4;-4;-2;-2];

    A = [1, 3, 0, 0,  0;
	 0, 0, 1, 1, -2;
	 0, 1, 0, 0, -1];
    b = [4;0;0];

    C = [A;-A];
    c = [b;-b];

    x_ref = ones(5,1);

%%%EOF
