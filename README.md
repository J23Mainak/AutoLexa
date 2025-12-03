## AutoLexa
A C++ tool to convert Regular Expressions into Minimized DFAs and Check Regex Equivalence

- AutoLexa is a full-featured Automata Theory tool built in C++ that:

- Converts regular expressions → ε-NFA → DFA → minimized DFA

- Checks whether two regular expressions are equivalent

 - Generates distinguishing witness strings when they are not equivalent

Supports regex operators:
a b | . * + ( )

AutoLexa is designed as a clean educational implementation of Thompson’s construction, subset construction, and Hopcroft-style DFA minimization.

## Features
Regex to Thompson ε-NFA Handles:

- Concatenation (.)

- Alternation (|)

- Kleene star (*)

- One-or-more (+)

- Parentheses

- NFA → DFA (Subset Construction)

- Computes epsilon-closures

- Generates deterministic transition tables

- Adds sink state automatically

- DFA Minimization

- Implements partition refinement (Hopcroft-like)

- Produces a canonical minimized DFA

- Ensures consistent state numbering (initial state → 0)

- Regex Equivalence Checker

- Compares minimized DFAs

- Reports whether both REs accept the same language

Witness String Generator

If regexes are not equivalent, AutoLexa finds n shortest distinct strings such that:

String ∈ L(regex1) XOR String ∈ L(regex2)

## Build & Run
Requirements

C++17+

g++ or clang++

Build
g++ -std=c++17 main.cpp -o autolexa

Run
./autolexa

Example
Input
Regex 1: a.(b|a)
Regex 2: ab|aa

Output (sample)
Postfix Expression for Regex a.(b|a) is ab a| .
Minimized DFA Table:
- ------------------------
- States | a | b | Final |
- 0      | 1 | 2 | No
- 1      | 3 | 4 | No
- 3      | - | - | Yes
- ...

Both Regular Expressions are equivalent.

Non-equivalent example

Input:

Regex 1: a*
Regex 2: a+


Output:

Both Regular Expressions are not equivalent.

Enter number of distinct strings: 3

Distinct strings:
ε

How AutoLexa Works (Algorithms)
1. Regex → Postfix Expression

Uses a modified Shunting-Yard algorithm with custom precedence:

* (4), + (3), . (2), | (1)

2. Postfix → ε-NFA (Thompson’s Construction)

Handles:

Literal transitions

Concatenation

Alternation

Kleene star

One-or-more repetition

3. NFA → DFA (Subset Construction)

Steps:

Compute epsilon-closure of start state

Create DFA states as sets of NFA states

Map 'a' → 0, 'b' → 1

Add sink state for missing transitions

4. DFA Minimization

Partition states into accepting / non-accepting groups

Recursively split partitions based on transition destinations

Rebuild minimized DFA with new canonical numbering

5. Regex Equivalence Check

Flattens minimized DFA transition tables

Compares structure and final states

6. Witness Generation

Breadth-first search of all strings (by length), checking:

simulate_dfa(dfa1, s1, str) XOR simulate_dfa(dfa2, s2, str)


Returns shortest distinguishing strings.

📁 Folder Structure (if you split later)
AutoLexa/
│── src/
│   └── main.cpp
│── docs/
│── README.md
│── LICENSE
│── .gitignore

Suggested Test Inputs
Regex 1	Regex 2	Expected
a	a	Equivalent
a	b	Not equivalent (witness: a)
a*	a+	Not equivalent (witness: ε)
ab	a.b	Equivalent
a.(b/a)	ab	Equivalent
(a|b)*	(a|b)*a	Not equivalent

(Your broken test entries have been corrected only for Markdown formatting — wording stays same.)

Contributing

Pull requests are welcome!
Ideas for future improvements:

Support larger alphabets

Support ε explicitly in regex input

Visualization of NFA/DFA graphs using Graphviz

Full Hopcroft minimizer (O(n log n))

Product-automaton-based equivalence checker

License

MIT Licens
