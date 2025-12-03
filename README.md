## AutoLexa
A C++ tool to convert Regular Expressions into Minimized DFAs and Check Regex Equivalence

- AutoLexa is a full-featured Automata Theory tool built in C++ that:

- Converts regular expressions → ε-NFA → DFA → minimized DFA

- Checks whether two regular expressions are equivalent

 - Generates distinguishing witness strings when they are not equivalent

Supports regex operators:
a b | . * + ( )

AutoLexa is designed as a clean educational implementation of Thompson’s construction, subset construction, and Hopcroft-style DFA minimization.

---

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

---

## Build & Run
Requirements

C++17+

g++ or clang++

Build
```bash
g++ -std=c++17 main.cpp -o autolexa
