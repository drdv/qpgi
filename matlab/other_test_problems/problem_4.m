function [H,h,C,c,x_ref] = problem_4()
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 56, "Problem 20"
%%%

    %% equation (4.9)
    H = eye(10);
    h = zeros(10,1);

    %% equation (4.10)
    for k=1:9
	A(k,k+1) =  1;
	A(k,k)   = -1;
	b(k)     = -0.5 + (-1)^k*k;
    end
    A(10,1)  =  1;
    A(10,10) = -1;
    b(10)    = 9.5;
    b = b(:);

    C = -A;
    c = -b;

    for k=1:10
	x_ref(k) = 2.75 - (1 + (-1)^k)*(0.5*k - 0.25);
    end
    x_ref = x_ref(:);

%%%% EOF
