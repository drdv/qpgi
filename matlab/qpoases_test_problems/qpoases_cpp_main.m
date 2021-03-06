addpath('../matlab_implementation')
addpath('~/local/bin/qp_mex')

%% tolenrace used when comparing the solution
solution_tolerance = 1e-09;

solver_tolerance = 1e-08;

root_folders{1} = 'cpp/oqp';
root_folders{2} = 'cpp/problems';
root_folders{3} = '/Users/drdv/git/github/qp-collection/inverse_kinematics_pepper';

folders_with_test_problems = qpoases_cpp_get_folders(root_folders);

for i=1:length(folders_with_test_problems)
    P = qpoases_cpp_get_data(folders_with_test_problems{i});
    log(i) = qpoases_cpp_solve(P, solver_tolerance, solution_tolerance);
end

%% statistics
fprintf('===============================\n')
fprintf('                 [SOLVE]: %d\n', sum([log(:).normal_solve]));
fprintf('                 [ FAIL]: %d\n', sum([log(:).fail]));
fprintf('singular hessian [ STOP]: %d\n', sum([log(:).singular_stop]));
fprintf('singular hessian [SOLVE]: %d\n', sum([log(:).singular_solve]));
fprintf('===============================\n')
