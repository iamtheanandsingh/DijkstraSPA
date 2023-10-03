# DijkstraSPA
Semester 1 Academic Project
In any hardware design, timing is an important parameter. This project requires you to find the 
shortest path in terms of delay between an input and an output of a circuit. You need to model
the circuit as a graph and apply Dijkstra’s shortest path algorithm between a user provided input 
and output to the circuit. We will ONLY consider circuits from the ISCAS’85 benchmark list (no 
sequential circuit). Here is the list of circuits:
https://filebox.ece.vt.edu/~mhsiao/iscas85.html
To calculate the delay of each gate, assume an integer multiple of its fan-outs. For example, 
consider the following snippet:
A = AND (B, C)
D = NOT (A)
E = BUF(A)
F = NOR (A, M)
In this case, the AND gate corresponding to A has a fan-out of 3. Therefore, the path B à A will 
have a delay of 3 units. Similarly, the path Cà A also has a delay of 3 units
