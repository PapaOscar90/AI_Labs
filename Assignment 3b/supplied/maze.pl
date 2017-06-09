
%Edges of the maze

edge(a,b).
edge(b,f).
edge(c,g).
edge(c,d).
edge(d,h).
edge(e,f).
edge(f,j).
edge(g,k).
edge(h,l).
edge(i,m).
edge(j,k).
edge(l,p).
edge(m,n).
edge(n,o).

% This predicate also allows reverse direction edge
edges(X, Y) :- edge(X, Y) ; edge(Y, X).

% Base case

path(X, X).

% Recursive case
path(X,Y) :- path(X, A), edges(A, Y).
				
