% isnumber(X) is true if X is a isnumber

isnumber(0).
isnumber(s(X)) :- isnumber(X).

% plus(X,Y,Z) is true if X + Y = Z

plus(0,X,X) :- isnumber(X).
plus(s(X),Y,s(Z)) :- plus(X,Y,Z).

% minus(X,Y,Z) is true if X - Y =Z

minus(X,0,X) :- isnumber(X).
minus(s(X),s(Y),Z) :- minus(X,Y,Z).

% times(X,Y,Z) is true if X * Y = Z

times(X,0,0) :- isnumber(X).
times(X,s(Y),Z) :- times(X,Y,Z1), plus(X,Z1,Z).

% pow(X,Y,Z) is true if X^Y = Z

pow(X,0,s(0)) :- isnumber(X).
pow(X,s(Y),Z) :- pow(X,Y,Z1), times(X,Z1,Z).

% even(N) is true if N is an even integer

even(0).
even(s(N)) :- odd(N).

% odd(N) is true if N is an odd integer

odd(s(0)).
odd(s(N)) :- even(N).

% div2(N,D) is true if N/2=D (integer division)

div2(N,D) :- minus(N, D, D) ; minus(s(N), s(D), s(D)).

% divi2(N,D) is true if N/2=D (integer division - using times)

divi2(N,D) :- times(s(s(0)), D, N) ; times(s(s(0)), s(D), s(N)).

% log(X, B, N) is true if B^N = X.

log(X, B, N) :- pow(B,N,X).

% fib(X, Y) returns true only if the Xth fib number is Y. 

fib(0,0).
fib(s(0),s(0)).
fib(s(s(X)), Y) :- fib(s(X), Y1) , fib(X, Y2), plus(Y1,Y2,Y).

% power(X, N, Y) returns true if X^N = Y

power(X, 0, s(0)) :- isnumber(X).
power(X, s(N), Y) :- odd(N), div2(s(N), A), times(X, X, B), power(B, A, Y) ;
						even(N), power(X, N, Y1), times(X, Y1, Y).


% Example queries:
% Isnumbers are represented as successors of 0. For example, 2 is s(s(0)).
% 2+2=4 is plus(s(s(0)), s(s(0)), s(s(s(s(0)))))
% 3*2=? is times(s(s(s(0))), s(s(0)), X)
