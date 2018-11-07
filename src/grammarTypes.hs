module GrammarTypes where

    tsShow :: TerminalSymbol -> String
    tsShow (CFGSymbol a)    = [a]
    tsShow CFGEpsilon       = "epsilon"

    gsShow :: GrammarSymbol -> String
    gsShow (Nonterminal a)  = [id a]
    gsShow (Terminal a)     = show a

    rlShow :: RuleRight -> String
    rlShow (SR a) = gsShow a
    rlShow (CR a) = showComplex a where
        showComplex :: [GrammarSymbol] -> String
        showComplex [] = []
        showComplex (x:xs) = gsShow x ++ showComplex xs

    instance Show TerminalSymbol where show ts   = tsShow ts
    instance Show GrammarSymbol where show gs    = gsShow gs
    instance Show RuleRight where show rr        = rlShow rr

    data TerminalSymbol = CFGSymbol Char | CFGEpsilon deriving (Eq)
    type NonterminalSymbol = Char

    data GrammarSymbol = Nonterminal NonterminalSymbol | Terminal TerminalSymbol deriving (Eq)

    -- Right side of context-free grammar rule (SR - simple rule, CR - combined rule)
    data RuleRight = SR GrammarSymbol | CR [GrammarSymbol]

    type Rule = (NonterminalSymbol, RuleRight)

    type ContextFreeGrammar = (NonterminalSymbol, [Rule])