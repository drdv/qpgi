function log = qpoases_cpp_solve(P,tolerance)
%%%
%%% solve qpoases problems is structure P
%%%

    log.normal_solve   = 0;
    log.fail           = 0;
    log.singular_stop  = 0;
    log.singular_solve = 0;
    for i=1:P.numb_qps

	C = [eye(P.numb_var);-eye(P.numb_var)];
	c = [P.ub(i,:)';-P.lb(i,:)'];
	if (P.numb_ctr_in > 0)
	    C = [C;P.A;-P.A];
	    c = [c;P.ubA(i,:)';-P.lbA(i,:)'];
	end

	[x,exitflag] = qpgi(P.H,P.g(i,:)',C,c);

	if strcmp(exitflag, 'HESSIAN_FACTORIZATION_PROBLEMS')
	    fprintf('-STOP-   <POSSIBLY_SINGULAR_HESSIAN: %e> Problem [%s (%d/%d)] \n', ...
		    min(eig(P.H)), P.folder_name, i, P.numb_qps);
	    log.singular_stop = log.singular_stop + 1;
	    continue
	end

	if P.skip
	    %% We can end-up here when the Hessian is singular to working precision but Eigen could
	    %% Cholesky decompose it so we proceed solving anyway.
	    fprintf('-RISK-   <HESSIAN SINGULAR TO WORKING PRECISION: %e> but we continue solving anyway ... \n %s', ...
		    min(eig(P.H)), '        ')
	    log.singular_solve = log.singular_solve + 1;
	end

	err = x - P.x_opt(i,:)';
	if norm(err) <= tolerance
            fprintf('SOLVED   Problem [%s (%d/%d)]: %e \n', ...
		    P.folder_name, i, P.numb_qps, norm(err));

	    log.normal_solve = log.normal_solve + 1;

	    if ~strcmp(exitflag, 'SOLVED')
		fprintf('ERROR: we didn''t recognize that we have solved the problem!')
		keyboard
	    end
	else
	    fprintf('FAILED   Problem [%s (%d/%d)]: %e \n', ...
		    P.folder_name, i, P.numb_qps, norm(err));

	    log.fail = log.fail + 1;
	    %% keyboard
	end
    end

%%% EOF
