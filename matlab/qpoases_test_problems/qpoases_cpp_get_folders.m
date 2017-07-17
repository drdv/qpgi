function problem_folders = qpoases_cpp_get_folders(root_folders)
%%%
%%% get folder names with qpoases test problems
%%%

    problem_folders = {};
    for i=1:length(root_folders)
	L{i} = dir(root_folders{i});
	nontrivial_folders = ~(strcmp({L{i}(:).name}, '.') | strcmp({L{i}(:).name},'..'));
	L{i} = strcat(root_folders{i}, '/', {L{i}(nontrivial_folders).name});

	problem_folders = {problem_folders{:},L{i}{:}};
    end

%%% EOF
