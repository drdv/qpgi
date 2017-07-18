addpath('..')

clear;clc

options = optimset('Algorithm','active-set','Display','off');
tol = 1e-10;

format long
for n = 2:2:10
    [H,h,C,c,x_ref] = problem_2(n);

    [x,exitflag,u,iter] = qpgi(H,h,C,c,tol);
    opt_cond = norm(H*x + h + C'*u);
    violated = find(C*x-c > tol);

    if ~isempty(violated)
	disp('qpgi: violated constraints')
	v = C*x-c; v(violated)
    end

    [x0,~,~,~,lambda] = quadprog(H,h,C,c,[],[],[],[],[],options);
    u0 = lambda.ineqlin;
    opt_cond0 = norm(H*x0 + h + C'*u0);
    violated0 = find(C*x0-c > tol);

    if ~isempty(violated0)
	disp('quadprog: violated constraints')
	v = C*x-c; v(violated)
    end

    %% The solvers identify different active sets (to be expected on this problem)
    fprintf('qpgi: %e, quadprog: %e\n', opt_cond, opt_cond0);
end
