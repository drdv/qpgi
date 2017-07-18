function [x, exitflag, u, iter] = qpgi(varargin)
%%%
%%% A rudimentary interface for qpgi_interface.cpp
%%%
%%% ---------------------------------------------
%%% [x, exitflag, u, iter] = qpgi(H,h,C,c,tol)
%%% ---------------------------------------------
%%%  min. 0.5*x'*H*x + x'*h
%%%   st. C*x <= c
%%%

    H = varargin{1};
    h = varargin{2};
    C = varargin{3};
    c = varargin{4};

    if length(varargin) < 5
	tol = 1e-12; %% default tolerance
    else
	tol = varargin{5};
    end

    [x, exitflag, u, iter] = qpgi_interface(H, h, C, c, tol);

%%%EOF
