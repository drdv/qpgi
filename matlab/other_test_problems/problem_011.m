function [H,h,C,c,x_ref] = problem_011()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problem 1-10"
%%%
%%% problem 52 from
%%% "Test Examples for Nonlinear Programming Codes: Hock-Schittkowski-Collection"
%%%

    H = 2*[16 -4  0  0  0;
	   -4  2  1  0  0;
	    0  1  1  0  0;
	    0  0  0  1  0;
	    0  0  0  0  1];
    h = [0;-4;-4;-2;-2];

    A = [1,3,0,0,0;
	 0,0,1,1,-2;
	 0,1,0,0,-1];
    b = [0;0;0];

    C = [A;-A];
    c = [b;-b];

    x_ref = [-33;11;180;-158;11]/349;

%%%EOF
