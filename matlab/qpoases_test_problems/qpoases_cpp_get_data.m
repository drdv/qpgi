function P = qpoases_cpp_get_data(folder_name, tolerance)
%%%
%%% get data for qpoases problems in folder_name
%%%
    if nargin < 2
	tolerance = 1e-12;
    end

    %% load dimensions
    dims = load([folder_name, '/dims.oqp']);
    P.numb_qps    = dims(1);
    P.numb_var    = dims(2);
    P.numb_ctr_in = dims(3); %% general inequality constraints
    P.numb_ctr_eq = dims(4);

    %% load problem data
    P.H  = load([folder_name, '/H.oqp']);
    P.g  = load([folder_name, '/g.oqp']);
    P.lb = load([folder_name, '/lb.oqp']);
    P.ub = load([folder_name, '/ub.oqp']);

    %% load general inequality constraints (if any)
    if ( P.numb_ctr_in > 0 )
	P.A   = load([folder_name, '/A.oqp']);
	P.lbA = load([folder_name, '/lbA.oqp']);
	P.ubA = load([folder_name, '/ubA.oqp']);
    end

    %% load solution
    P.x_opt = load([folder_name, '/x_opt.oqp']);
    P.y_opt = load([folder_name, '/y_opt.oqp']);
    P.obj_opt = load([folder_name, '/obj_opt.oqp']);

    %% form output
    P.folder_name = folder_name;
    P.skip = false;
    if min(eig(P.H)) < tolerance
	P.skip = true;
    end

%%% EOF
