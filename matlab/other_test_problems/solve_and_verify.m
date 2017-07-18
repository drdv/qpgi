function [X,F] = solve_and_verify(x_ref,H,h,C,c)
%%%
%%%
%%%
    options = optimset('Algorithm','active-set','Display','off');

    tol_ctr_violation = 1e-10;

    %% -------------------------------------------------------------

    [x_qpgi,exitflag,u,iter] = qpgi(H,h,C,c);
    if strcmp(exitflag,'SOLVED')
	status_qpgi = 'SOLVED';
    else
	status_qpgi = 'FAILED';
    end

    opt_cond_qpgi = norm(H*x_qpgi + h + C'*u);
    violated_qpgi = find(C*x_qpgi-c > tol_ctr_violation);

    if ~isempty(violated_qpgi)
	disp('qpgi: violated constraints')
	v = C*x-c; v(violated_qpgi)
    end

    %% -------------------------------------------------------------

    [x_matlab,~,exitflag,~,lambda] = quadprog(H,h,C,c,[],[],[],[],[],options);
    u_matlab = lambda.ineqlin;
    opt_cond_matlab = norm(H*x_matlab + h + C'*u_matlab);
    violated_matlab = find(C*x_matlab-c > tol_ctr_violation);
    if exitflag == 1
	status_matlab = 'SOLVED';
    else
	status_matlab = 'FAILED';
    end

    %% -------------------------------------------------------------

    lbC = -1e+10*ones(size(c));
    [x_qld,~,   exitflag] = qld        ([], H, h, [], [], [], [], C, c);
    if exitflag.info == 0
	status_qld = 'SOLVED';
    else
	status_qld = 'FAILED';
    end

    [x_qpmad, exitflag] = qpmad      (H, h, [], [], [], [], C, lbC, c);
    if exitflag.status == 0
	status_qpmad = 'SOLVED';
    else
	status_qpmad = 'FAILED';
    end

    [x_quadp, exitflag] = quadprogpp (H, h, [], [], [], [], C, lbC, c);
    if exitflag.status == 0
	status_quadp = 'SOLVED';
    else
	status_quadp = 'FAILED';
    end

    f_ref    = get_obj_val(x_ref,H,h);
    f_qpgi   = get_obj_val(x_qpgi,H,h);
    f_matlab = get_obj_val(x_matlab,H,h);
    f_qld    = get_obj_val(x_qld,H,h);
    f_qpmad  = get_obj_val(x_qpmad,H,h);
    f_quadp  = get_obj_val(x_quadp,H,h);

    %% -------------------------------------------------------------

    fprintf('--------------------------------------------------------\n')
    fprintf('qpgi  : opt_cond = %e \n', opt_cond_qpgi)
    fprintf('matlab: opt_cond = %e \n', opt_cond_matlab)
    fprintf('--------------------------------------------------------\n')
    fprintf(' [%s] qpgi  : err(x) = %e, err(f) = %e\n', status_qpgi, norm(x_ref-x_qpgi) , abs(f_ref-f_qpgi));
    fprintf(' [%s] matlab: err(x) = %e, err(f) = %e\n', status_matlab, norm(x_ref-x_matlab) , abs(f_ref-f_matlab));
    fprintf(' [%s] qld   : err(x) = %e, err(f) = %e\n', status_qld, norm(x_ref-x_qld)  , abs(f_ref-f_qld));
    fprintf(' [%s] qpmad : err(x) = %e, err(f) = %e\n', status_qpmad, norm(x_ref-x_qpmad), abs(f_ref-f_qpmad));
    fprintf(' [%s] quadp : err(x) = %e, err(f) = %e\n', status_quadp, norm(x_ref-x_quadp), abs(f_ref-f_quadp));

    X = [x_ref,x_qpgi,x_matlab,x_qld,x_qpmad,x_quadp];
    F = [f_ref,f_qpgi,f_matlab,f_qld,f_qpmad,f_quadp];

%%%EOF
