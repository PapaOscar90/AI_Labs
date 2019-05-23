
%Determines length of a list

len([],0).
len([H|T],N) :- len(T,N1), N is N1+1.

% member(X,L) returns true if X is a member of the list L.

member(H, [H|T]).
member(H, [H1|T]) :- member(H, T).

% concat(L,X,Y) returns true if L is the concatenation of the lists X and Y.

concat(X, [], X).
concat([H|T1], [H|T2], Y) :- concat(T1, T2, Y).

% reverse(L,R) returns true if R is the reversal of the list L.

reverse([], []).
reverse(L, [H|T]) :- concat(L, A, [H]), reverse(A, T).

% palindrome(L) returns true if L is a palindrome.

palindrome(L) :- reverse(L, L).
