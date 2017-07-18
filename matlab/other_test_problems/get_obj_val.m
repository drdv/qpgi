function f = get_obj_val(x,H,h)
%%%
%%% compute objective value
%%%

    f = 0.5*x'*H*x + x'*h;

%%% EOF
