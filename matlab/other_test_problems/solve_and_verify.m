function [X,F] = solve_and_verify(x_ref,H,h,C,c)
%%%
%%%
%%%
    options = optimset('Algorithm','active-set','Display','off');

    tol = 1e-10;

    %% -------------------------------------------------------------

    [x_qpgi,exitflag,u,iter] = qpgi(H,h,C,c);

    opt_cond = norm(H*x_qpgi + h + C'*u);
    violated = find(C*x_qpgi-c > tol);

    if ~isempty(violated)
	disp('qpgi: violated constraints')
	v = C*x-c; v(violated)
    end

    %% -------------------------------------------------------------

    [x_matlab,~,~,~,lambda] = quadprog(H,h,C,c,[],[],[],[],[],options);
    u_matlab = lambda.ineqlin;
    opt_cond0 = norm(H*x_matlab + h + C'*u_matlab);
    violated0 = find(C*x_matlab-c > tol);

    if ~isempty(violated0)
	disp('quadprog: violated constraints')
	v = C*x_matlab-c; v(violated)
    end

    %% -------------------------------------------------------------

    lbC = -1e+10*ones(size(c));
    x_qld   = qld        ([], H, h, [], [], [], [], C, c);
    x_qpmad = qpmad      (H, h, [], [], [], [], C, lbC, c);
    x_quadp = quadprogpp (H, h, [], [], [], [], C, lbC, c);

    f_ref   = get_obj_val(x_ref,H,h);
    f_qpgi  = get_obj_val(x_qpgi,H,h);
    f_qld   = get_obj_val(x_qld,H,h);
    f_qpmad = get_obj_val(x_qpmad,H,h);
    f_quadp = get_obj_val(x_quadp,H,h);

    %% -------------------------------------------------------------

    fprintf('--------------------------------------------------------\n')
    fprintf('qpgi  : opt_cond = %e \n', opt_cond)
    fprintf('matlab: opt_cond = %e \n', opt_cond)
    fprintf('--------------------------------------------------------\n')
    fprintf('   qpgi : err(x) = %e, err(f) = %e\n', norm(x_ref-x_qpgi) , abs(f_ref-f_qpgi));
    fprintf('   qld  : err(x) = %e, err(f) = %e\n', norm(x_ref-x_qld)  , abs(f_ref-f_qld));
    fprintf('   qpmad: err(x) = %e, err(f) = %e\n', norm(x_ref-x_qpmad), abs(f_ref-f_qpmad));
    fprintf('   quadp: err(x) = %e, err(f) = %e\n', norm(x_ref-x_quadp), abs(f_ref-f_quadp));

    X = [x_ref,x_qpgi,x_qld,x_qpmad,x_quadp];
    F = [f_ref,f_qpgi,f_qld,f_qpmad,f_quadp];

%%%EOF
