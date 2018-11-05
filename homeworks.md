
# Homeworks
## Automaton

1. [x] Transformation of non-deterministic automaton to deterministic one.
2. [x] Print transition table

*Solved in [automata.hs](src/automaton.hs)*

## Text align and word index
1. [x] Align input string to line width
2. [x] Create word index for words, which length is greater than 4 and show line numbers, where those words are present.
    - Order words in index lexicographically.


### Usefull functions:
``` haskell
 > words "car bus plane"
 ["car","bus","plane"]
 > lines "car\nbus\nplane"
 ["car","bus","plane"]
```

*First part is solved in [textAlign.hs](src/textAlign.hs)*
*Second part is solved in [index.hs](src/index.hs)*

## Regular Expressions
1. [x] Transform regular expression (re1) to finite automaton. (Non-Deterministic *[with empty word transitions]*) *(10 pts)*
2. [x] `BONUS:` Implement generalNonDeterministic step *(2.5-5 pts)*
3. [x] `BONUS:` Transformation of general non deterministic automaton to deterministic *(2.5-5 pts)*

```haskell
data RegularExpression = Symb Char
                        | Iter RegularExpression
                        | Concat RegularExpression RegularExpression -- Concatenation
                        | Or RegularExpression RegularExpression
                        deriving (Eq,Show)

--     (a+b)*ab
re1 :: RegularExpression
re1 = Concat (Concat (Iter (Or (Symb 'a') (Symb 'b'))) (Symb 'a')) (Symb 'b')

-- Symbol for Deterministic automaton's transition
data Symbol = Symbol Char
            | Epsilon

```

## Context-free Grammar
Course project. Operations with context-free grammar.




