function [x, exitflag] = qpgi(varargin)
%%%
%%% A rudimentary interface for qpgi_interface.cpp
%%%
%%% [x, exitflag] = qpgi(H,h,C,c): min. 0.5*x'*H*x + x'*h
%%%                                 st. C*x <= c
%%%
%%% [x, exitflag] = qpgi(H,h,C,lbC,ubC): min. 0.5*x'*H*x + x'*h
%%%                                       st. lbC <= C*c <= ubC
%%%

    H = varargin{1};
    h = varargin{2};
    C = varargin{3};
    c = varargin{4};

    if length(varargin) > 4
	%% -----------------
	%% lbC <= C <=  ubC
	%% -----------------
	%%        C <=  ubC
	%%       -C <= -lbC
	%% -----------------

	C = [C;-C];
	c = [c;-varargin{5}];
    end

    [x, exitflag] = qpgi_interface(H, h, C, c);

%%%EOF
