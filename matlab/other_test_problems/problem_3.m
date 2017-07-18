function [H,h,C,c,x_ref] = problem_3(case_flag)
%%%
%%% M. Powell, "On the quadratic programming algorithm
%%%             of Goldfarb and Idnani," 1984.
%%%
%%% p. 56, "Problem 19"
%%%

    if nargin == 0
	case_flag = 1;
    end

    %% equation (4.6)
    if case_flag == 1
	H = diag([10^-10,10^-20]); % standard case
    else
	H = diag([0,0]); % testing
    end
    h = [1;1];

    %% equation (4.7), (4.8)
    for k=1:20
	A(k,1) = cos(0.68 + 0.01*k);
	A(k,2) = sin(0.68 + 0.01*k);
    end
    b = -ones(20,1);

    C = -A;
    c = -b;

    x_ref = A([10,11],:)\b([10,11]);

%%%% EOF
