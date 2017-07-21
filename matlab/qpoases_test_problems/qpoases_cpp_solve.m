function log = qpoases_cpp_solve(P, solver_tolerance, solution_tolerance)
%%%
%%% solve qpoases problems is structure P
%%%

    log.normal_solve   = 0;
    log.fail           = 0;
    log.singular_stop  = 0;
    log.singular_solve = 0;
    for i=1:P.numb_qps

	if 0
	    C = [eye(P.numb_var);-eye(P.numb_var)];
	    c = [P.ub(i,:)';-P.lb(i,:)'];
	    if (P.numb_ctr_in > 0)
		C = [C;P.A;-P.A];
		c = [c;P.ubA(i,:)';-P.lbA(i,:)'];
	    end

	    [x, exitflag] = qpgi(P.H,P.g(i,:)',C,c,solver_tolerance);
	else
	    C = eye(P.numb_var);
	    c_lb = P.lb(i,:)';
	    c_ub = P.ub(i,:)';
	    if (P.numb_ctr_in > 0)
		C = [C;P.A];
		c_lb = [c_lb;P.lbA(i,:)'];
		c_ub = [c_ub;P.ubA(i,:)'];
	    end

	    [x, exitflag, u, iter] = qpgi(P.H,P.g(i,:)',C,c_lb,c_ub,solver_tolerance);
	end

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
	if norm(err) <= solution_tolerance && strcmp(exitflag, 'SOLVED')
            fprintf('SOLVED   Problem [%s (%d/%d)]: %e \n', ...
		    P.folder_name, i, P.numb_qps, norm(err));

	    %% compare objective functions
	    %{
	    obj = 0.5*x'*P.H*x + P.g(i,:)*x;
            if obj > P.obj_opt(i) + 1e-07
                fprintf('\n\n ----  err(x): %e, err(f): %e \n', norm(err), obj - P.obj_opt(i));
		keyboard
            end
	    %}
	    log.normal_solve = log.normal_solve + 1;
	else
	    fprintf('FAILED   Problem [%s (%d/%d)]: %e \n', ...
		    P.folder_name, i, P.numb_qps, norm(err));

	    log.fail = log.fail + 1;
	    %% keyboard
	end
    end

%%% EOF
