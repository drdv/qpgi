function [z,r] = factorize_solve(H,A,a)
%%%
%%%
%%%

    L = chol(H)'; % L is lower triangular so that H = L*L'
    J = inv(L');

    if isempty(A)
	d =  J'*a;
	z = -J*d;
	r =  [];
    else
	[m,n] = size(A);
	[Q,R_] = qr(L\A');
	R = R_(1:m,:);

	J = J * Q;
	d = J'*a;

	z = -J(:,m+1:end)*d(m+1:end);
	r = -R\d(1:m);
    end

%%%EOF
