function [H,h,C,c,x_ref] = problem_2(n)
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problems 14-18"
%%%

    %% equation (4.3)
    for k=1:2*n
	for j=1:n
	    A(k,j) = 1/(j+k);
	end
    end
    b = 10*A*ones(n,1);

    %% equation (4.4)
    for j=1:n
	H(j,j) = sum(A(1:n,j));
    end
    h = zeros(n,1);

    C = -A;
    c = -b;

    x_ref = 10*ones(n,1);

%%%% EOF
