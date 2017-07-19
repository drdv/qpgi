function [H,h,C,c,x_ref] = problem_013()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problem 1-10"
%%%
%%% problem 76 from
%%% "Test Examples for Nonlinear Programming Codes: Hock-Schittkowski-Collection"
%%%

    H = 2*[  1    0  -0.5    0;
	     0  0.5     0    0;
	  -0.5    0     1  0.5;
	     0    0   0.5  0.5];
    h = [-1;-3;1;-1];

    A = [-1, -2, -1, -1;
	 -3, -1, -2,  1;
	 0,  1,  4,  0;
	 eye(4)];
    b = [-5;-4;1.5;0;0;0;0];

    C = -A;
    c = -b;

    x_ref = [0.2727273;2.090909;-0.26e-10;0.5454545];

%%%EOF
