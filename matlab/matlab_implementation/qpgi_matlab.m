function [x,u,W,status,iter] = qpgi_matlab(H,h,C,c,qpgi_log);
%%%
%%% solve a strictly convex QP
%%%
%%%   minimize   f(x) = 0.5*x'*H*x + x'*h
%%%   subject to s(x) = C*x - c <= 0
%%%
%%% using the Goldfarb-Idnani dual method
%%%

    if nargin < 5
	qpgi_log = false;
    end

    tol    = 1e-12;
    iter   = 0;
    status = 'UNKNOWN';
    W      = [];  % working set;

    %% Lagrange multipliers for all constraints
    u = zeros(length(c),1);

    %% unconstrained solution
    x = factorize_solve(H,[],h);

    full_step_was_performed = true;
    while true

	if qpgi_log
	    fprintf('=============================================\n\n')
	    fprintf('iter = %d\n\n', iter)

	    fprintf('constraints_register:\n');
	    fprintf('  numb constraints      : %d \n', length(c));
	    fprintf('  numb active inequality: %d \n', length(W));
	    fprintf('  numb active equality  : %d \n', 0);
	    fprintf('  active: {');
	    fprintf(' %d', W-1); % -1 is for convenience when comparing with the C++ implementation
	    fprintf(' } \n\n');
	end

	%% ------------------------------------
	%% STEP1: check for violated constraints
	%% ------------------------------------

	if full_step_was_performed

	    candidate_ctr = get_candidate_ctr(x,W,C,c,tol);

	    if isempty(candidate_ctr.ind)
		status = 'SOLVED';
		return
	    end
	end

	if qpgi_log
	    fprintf('candidate_constraint:\n');
	    fprintf('  index    : %d \n', candidate_ctr.ind-1);
	    fprintf('  violation: %f \n', -(candidate_ctr.b - candidate_ctr.a'*x));
	    fprintf('  status   : %s \n', 'ConstraintActivationStatus::ACTIVE_UPPER_BOUND');
	    fprintf('  dual     : %f \n\n', candidate_ctr.u);
	end

	%% ------------------------------------
	%% STEP2: find primal and dual step directions
	%% ------------------------------------

	[z,r] = factorize_solve(H,C(W,:),candidate_ctr.a);

	if qpgi_log
	    fprintf('dx = ...\n'); fprintf('%.15f\n',z); fprintf('\n')
	    fprintf('dy = ...\n'); fprintf('%.15f\n',r); fprintf('\n')
	end

	%% ------------------------------------
	%% find primal and dual step lengths
	%% ------------------------------------

	if norm(z) < tol
	    tau_primal = inf;
	else
	    tau_primal = (candidate_ctr.b - candidate_ctr.a'*x)/(candidate_ctr.a'*z);
	end

	negative_dual_step_ind = find(r < -tol);
	if isempty(W) || isempty(negative_dual_step_ind)
	    tau_dual = inf;
	    dual_blocking_index = 0;
	else
	    [tau_dual,ind] = min(-u(W(negative_dual_step_ind))./r(negative_dual_step_ind));
	    dual_blocking_index = negative_dual_step_ind(ind);
	end

	%% if tau_primal == tau_dual, tau_primal is chosen
	[tau, tau_case] = min([tau_primal, tau_dual]);

	if qpgi_log
	    fprintf('step_length:\n');
	    fprintf('  primal        : %f\n', tau_primal);
	    fprintf('  dual          : %f\n', tau_dual);
	    fprintf('  actual        : %f\n', tau);
	    fprintf('  blocking_index: %d\n', dual_blocking_index-1);

	    if tau_primal == inf && tau_dual == inf
		step_type = 'StepType::STOP';
	    elseif tau_primal == inf
		step_type = 'StepType::DUAL_STEP';
	    else
		if tau_case == 1
		    step_type = 'StepType::FULL_STEP';
		else
		    step_type = 'StepType::PARTIAL_STEP';
		end
	    end
	    fprintf('  step_type     : %s\n\n', step_type);
	end

	%% ------------------------------------
	%% determine new S-pair and make a step
	%% ------------------------------------

	%% no step in primal and dual space
	if tau_primal == inf && tau_dual == inf
	    status = 'INFEASIBLE';
	    return

	elseif tau_primal == inf % step in dual space

	    if ~isempty(W)
		u(W) = u(W) + tau * r;
	    end
	    candidate_ctr.u = candidate_ctr.u + tau;

	    %% drop blocking constraint
	    W(dual_blocking_index) = [];

	    full_step_was_performed = false;  %% goto STEP2

	else % step in primal and dual space
	    %% tau_primal <  inf
	    %% tau_dual   <= inf

	    x = x + tau*z;
	    if ~isempty(W)
		u(W) = u(W) + tau * r;
	    end
	    candidate_ctr.u = candidate_ctr.u + tau;

	    if tau_case == 1 % tau_primal: take a full step
		%% tau_primal <= tau_dual

		%% add selected primal constraint
		W = [W,candidate_ctr.ind];
		u(candidate_ctr.ind) = candidate_ctr.u;

		full_step_was_performed = true;  %% goto STEP1

	    else % partial step
		%% tau_primal > tau_dual
		%% tau_dual   <  inf

		%% drop blocking constraint
		W(dual_blocking_index) = [];

		full_step_was_performed = false;  %% goto STEP2

	    end
	end

	iter = iter + 1;
    end
