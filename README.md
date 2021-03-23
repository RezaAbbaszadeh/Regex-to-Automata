# Regex-to-Automata
A program to convert regular expressions to finite automatas.<br>
RE=>NFA=>DFA=>minimized DFA

### Input
```
(a|b)*b|aa
```

### Output
```
concatenation added: ((a|b)*.b|a.a)
after tilde replacement: ((a|b)*.b|a.a)
postfix form: ab|*b.aa.|


NFA:
States: q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11, q12, q13, q14, q15(first), q16(final),
Transitions:
transition(q1,a) = q2
transition(q2,Epsilon) = q6
transition(q3,b) = q4
transition(q4,Epsilon) = q6
transition(q5,Epsilon) = q3
transition(q5,Epsilon) = q1
transition(q6,Epsilon) = q5
transition(q6,Epsilon) = q8
transition(q7,Epsilon) = q5
transition(q7,Epsilon) = q8
transition(q8,Epsilon) = q9
transition(q9,b) = q10
transition(q10,Epsilon) = q16
transition(q11,a) = q12
transition(q12,Epsilon) = q13
transition(q13,a) = q14
transition(q14,Epsilon) = q16
transition(q15,Epsilon) = q11
transition(q15,Epsilon) = q7


DFA:
States:
q1(first), q2, q3(final), q4, q5(final)
Transitions:
transition(q1,a) = q2
transition(q1,b) = q5
transition(q2,a) = q3
transition(q2,b) = q5
transition(q3,a) = q4
transition(q3,b) = q5
transition(q4,a) = q4
transition(q4,b) = q5
transition(q5,a) = q4
transition(q5,b) = q5


Minimized DFA:
States:
q1(first), q2, q3(final), q4
Transitions:
transition(q1,a) = q2
transition(q1,b) = q3
transition(q2,a) = q3
transition(q2,b) = q3
transition(q3,a) = q4
transition(q3,b) = q3
transition(q4,a) = q4
transition(q4,b) = q3
```
