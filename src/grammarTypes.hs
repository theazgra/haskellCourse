module GrammarTypes where

    tsShow :: TerminalSymbol -> String
    tsShow (CFGSymbol a)    = [a]
    tsShow CFGEpsilon       = "epsilon"

    gsShow :: GrammarSymbol -> String
    gsShow (Nonterminal nt) = nt
    gsShow (Terminal t)     = show t

    rrShow :: RuleRight -> String
    rrShow (SR a) = gsShow a
    rrShow (CR a) = showComplex a where
        showComplex :: [GrammarSymbol] -> String
        showComplex [] = []
        showComplex (x:xs) = gsShow x ++ showComplex xs


    -- rrEq :: RuleRight -> RuleRight -> Bool
    -- rrEq (SR gsA) (SR gsB) = True
    -- rrEq (CR symbolsA) (CR symbolsB) = True
    -- rrEq _ _ = False

    instance Show TerminalSymbol where show ts  = tsShow ts
    instance Show GrammarSymbol where show gs   = gsShow gs
    instance Show RuleRight where show rr       = rrShow rr
    -- instance Eq RuleRight where a == b          = rrEq a b

    data TerminalSymbol = CFGSymbol Char | CFGEpsilon deriving (Eq)
    type NonterminalSymbol = [Char]

    data GrammarSymbol = Nonterminal NonterminalSymbol | Terminal TerminalSymbol deriving (Eq)

    -- Right side of context-free grammar rule (SR - simple rule, CR - combined rule)
    data RuleRight = SR GrammarSymbol | CR [GrammarSymbol] deriving (Eq)

    type Rule = (NonterminalSymbol, RuleRight)

    type ContextFreeGrammar = (NonterminalSymbol, [Rule])