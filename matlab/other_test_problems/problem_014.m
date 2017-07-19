function [H,h,C,c,x_ref] = problem_014()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problem 1-10"
%%%
%%% problem 118 from
%%% "Test Examples for Nonlinear Programming Codes: Hock-Schittkowski-Collection"
%%%

    n = 15;
    H = zeros(n,n);
    h = zeros(n,1);
    for k=0:4
	H(3*k+1,3*k+1) = 0.0001;
	H(3*k+2,3*k+2) = 0.0001;
	H(3*k+3,3*k+3) = 0.00015;

	h(3*k+1) = 2.3;
	h(3*k+2) = 1.7;
	h(3*k+3) = 2.2;
    end
    H = 2*H;

    A1 = zeros(4*3,n);
    lbA1 = zeros(4*3,1);
    ubA1 = zeros(4*3,1);
    i = 1;
    for j = 1:4
	A1(i,[3*j+1,3*j-2]) = [1,-1];
	lbA1(i) = -7;
	ubA1(i) =  6;
	i = i+1;

	A1(i,[3*j+2,3*j-1]) = [1,-1];
	lbA1(i) = -7;
	ubA1(i) =  7;
	i = i+1;

    	A1(i,[3*j+3,3*j]) = [1,-1];
	lbA1(i) = -7;
	ubA1(i) =  6;
	i = i+1;
    end

    A2 = zeros(5,n);
    b2 = zeros(5,1);
    i = 1;
    A2(i,[1,2,3]) = [1,1,1];
    b2(i) = 60;
    i = i+1;
    A2(i,[4,5,6]) = [1,1,1];
    b2(i) = 50;
    i = i+1;
    A2(i,[7,8,9]) = [1,1,1];
    b2(i) = 70;
    i = i+1;
    A2(i,[10,11,12]) = [1,1,1];
    b2(i) = 85;
    i = i+1;
    A2(i,[13,14,15]) = [1,1,1];
    b2(i) = 100;

    A3 = eye(n);
    lbA3 = zeros(n,1);
    ubA3 = zeros(n,1);
    lbA3([1,2,3]) = [8,43,3];
    ubA3([1,2,3]) = [21,57,16];
    for k=1:4
	ubA3(3*k+1) = 90;
	ubA3(3*k+2) = 120;
	ubA3(3*k+3) = 60;
    end

    C = [A1;-A1;-A2;A3;-A3];
    c = [ubA1;-lbA1;-b2;ubA3;-lbA3];

    x_ref = [8;49;3;1;56;0;1;63;6;3;70;12;5;77;18];

%%%EOF
