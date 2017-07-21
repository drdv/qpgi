function [x, exitflag, u, iter] = qpgi(varargin)
%%%
%%% A rudimentary interface for qpgi_interface.cpp
%%%
%%% ---------------------------------------------
%%% [x, exitflag, u, iter] = qpgi(H,h,C,c_lb,c_ub,tol)
%%% ---------------------------------------------
%%%  min. 0.5*x'*H*x + x'*h
%%%   st. c_lb <= C*x <= c_ub
%%%

    H = varargin{1};
    h = varargin{2};
    C = varargin{3};
    c_lb = varargin{4};
    c_ub = varargin{5};

    if length(varargin) < 6
	tol = 1e-12; %% default tolerance
    else
	tol = varargin{6};
    end

    [x, exitflag, u, iter] = qpgi_interface(H, h, C, c_lb, c_ub, tol);

%%%EOF
