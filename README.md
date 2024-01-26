# easySAT

## About

easySAT is an experimental SAT solver implemented in C. I developed the initial release of this project in the context of the Software Engineering course taught by Professor L. Poirrier at Bocconi University (a.y. 2023/2024).

## How to use it

Firstly, compile the source code with `make`. The compiled binary will be stored in `./build`.

The program can be invoked from the terminal with the syntax `./build/easySAT path/to/formula`.

 It receives as input a CNF boolean formula in DIMACS format and returns a satisfying assignment or detects unsatisfiability. While looking for a satisfying assignment, a progress bar representing the % of sat clauses at the current search state is displayed. When returning a satisfying assignment, the program specifies the number of assigned and free variables.

## Project report

easySAT relies upon a custom SAT solving algorithm I've developed.
- At each iteration, the algorithm tries an assignment that makes TRUE the clause with the lowest freedom among all unsatisfied clauses. By "freedom", I mean the number of literals corresponding to unassigned variables. At each solver step, the freedom of unsatisfied clauses is updated based on the current assignment status. Unit propagation is incorporated as a special case (a unit clause is a clause with freedom equal to 1).
- A conflict is triggered by a zero-freedom unsatisfied clause. When a conflict is detected, the algorithm inverts the polarity of the variable assignment that determined the conflict and tries to force the current clause to be TRUE by assigning a different variable (if the current clause has positive residual freedom, otherwise another conflict is triggered).

From the implementation perspective, each search state level is almost entirely represented with bitmaps. Furthermore, all the core components of the algorithm are implemented using bitwise operations over bitmaps, thus making the program very efficient. Search state levels are organized in a stack, with the top level corresponding to the clause the solver is currently trying to force to be TRUE. To keep track of assignments that still need to be explored for clauses in the stack, I use queues.

In a new release I'm currently working on, I eliminate queues and determine the next variable to be assigned through bitwise operations over bitmaps. This will further reduce memory consumption and improve efficiency. More interestingly, I will get very close to my original idea of developing a SAT solver almost entirely based on bitmaps for representing the state and bitwise operations as backbone of the algorithm. My goal is making the solver as close as possible to the hardware, minimizing its memory footprint and making it capable of processing small and medium instances on resource-constrained devices.

The software is quite efficient. In the tests I've conducted on a M1 iMac (on thousands of test cases), instances with 100 variables / 430 clauses were all processed in <= 1sec, and instances with 200 variables / 860 clauses in a few minutes. However, there's large room for improvement. easySAT is an experimental implementation of a basic algorithm, and does not incorporate any sophisticated heuristic. Interestingly, the number of solver steps required to process large formulas shows a huge variation depending on the choice of the first clause the program tries to force to TRUE (introducing some heuristics would be incredibly beneficial). Last but not least, the program does not take advantage of multi-core CPUs, and leveraging on multi-processing would significantly boost performance.
