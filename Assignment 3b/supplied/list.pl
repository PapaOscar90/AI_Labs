len([],0).
len([H|T],N) :- len(T,N1), N is N1+1.
