function ctr = get_candidate_ctr(x,W,C,c,tol)
%%%
%%% return the most violated constraint
%%%

    n = size(C,1);

    most_voilated_ind = [];
    val = tol;
    for i = 1:n
	if isempty(find(W == i))
	    s_i = C(i,:)*x - c(i);

	    if s_i > val
		val = s_i;
		most_voilated_ind = i;
	    end
	end
    end

    ctr.ind = most_voilated_ind;
    ctr.u   = 0;
    ctr.a   = C(most_voilated_ind,:)';
    ctr.b   = c(most_voilated_ind);

%%%EOF
