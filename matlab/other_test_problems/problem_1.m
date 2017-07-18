function [H,h,C,c] = problem_1(n)
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 55, "Problems 11-13"
%%%

    %% equation (4.1)
    for i=1:n
	for j=1:n
	    H(i,j) = 1/(i+j);
	end
    end
    h = H(:,1);

    %% equation (4.2)
    for i=2:n
	A(i-1,i)   = 1;
	A(i-1,i-1) = -1;
    end
    ubA =  10*ones(n-1,1);
    lbA = -10*ones(n-1,1);

    C = [A;-A];
    c = [ubA;-lbA];

%%%% EOF
