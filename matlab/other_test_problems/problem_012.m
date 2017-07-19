function [H,h,C,c,x_ref] = problem_012()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problem 1-10"
%%%
%%% problem 53 from
%%% "Test Examples for Nonlinear Programming Codes: Hock-Schittkowski-Collection"
%%%

    H = 2*[1 -1  0  0  0;
	  -1  2  1  0  0;
	   0  1  1  0  0;
	   0  0  0  1  0;
	   0  0  0  0  1];
    h = [0;-4;-4;-2;-2];

    Aeq = [1,3,0,0, 0;
	   0,0,1,1,-2;
	   0,1,0,0,-1];
    beq = [0;0;0];

    Ain = eye(5);
    Aub =  10*ones(5,1);
    Alb = -10*ones(5,1);

    C = [Aeq;-Aeq;Ain;-Ain];
    c = [beq;-beq;Aub;-Alb];

    x_ref = [-33;11;27;-5;11]/43;

%%%EOF
