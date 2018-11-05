{-# LANGUAGE UnicodeSyntax #-}

import Data.List
-- symbEq :: Symbol -> Symbol -> Bool
-- symbEq (Symbol a) (Symbol b)    = a == b
-- symbEq Epsilon Epsilon          = True
-- symbEq _ _                      = False



tsShow :: TerminalSymbol -> String
tsShow (Symbol a) = [a]
tsShow Epsilon    = "epsilon"

-- ntsShow :: NonterminalSymbol -> String
-- ntsShow (NonterminalSymbol a) = [a]

gsShow :: GrammarSymbol -> String
gsShow (Nonterminal a) = show a
gsShow (Terminal a) = show a

instance Show TerminalSymbol where show a = tsShow a
--instance Show NonterminalSymbol where show a = ntsShow a
instance Show GrammarSymbol where show a = gsShow a

data TerminalSymbol = Symbol Char | Epsilon deriving (Eq)
data GrammarSymbol = Nonterminal NonterminalSymbol | Terminal TerminalSymbol deriving (Eq)

type NonterminalSymbol = Char
type GrammarRule = (NonterminalSymbol, GrammarSymbol)
type ContextFreeGrammar = (NonterminalSymbol, [GrammarRule])

ruleFromSameNt :: GrammarRule -> GrammarRule -> Bool
ruleFromSameNt (ntA,_) (ntB,_) = ntA == ntB

ex :: ContextFreeGrammar
ex = ('S', (r))

r :: [GrammarRule]
r = [
    ('S', (Terminal (Symbol 'a'))),
    ('A', (Terminal (Symbol 'a'))),
    ('S', (Nonterminal ( 'A'))),
    ('S', (Nonterminal ( 'B'))),
    ('A', (Terminal (Symbol 'b'))),
    ('A', (Terminal Epsilon)),
    ('B', (Terminal Epsilon)),
    ('B', (Terminal (Symbol '('))),
    ('B', (Terminal (Symbol ')')))
    ]

mapWoLast :: (a -> a) -> [a] -> [a]
mapWoLast f []      = []
mapWoLast f [x]     = [x]
mapWoLast f (x:xs)  = f x : mapWoLast f xs

getNonterminals :: NonterminalSymbol -> [GrammarRule] -> [NonterminalSymbol]
getNonterminals initial other = initial : [ nT | (nT,_) <- (nubBy ruleFromSameNt other), not (nT == initial)]

printRuleGroup :: [GrammarRule] -> String
printRuleGroup rules =  let (nt,t) = (head rules)
                        in (show nt) ++ " --> " ++ (concat (mapWoLast (++ " | ") [ (show rT) | (rNT, rT) <- rules])) ++ "\n"

printContextFreeGrammar :: ContextFreeGrammar -> IO ()
printContextFreeGrammar (sT,rules) =    let nonTerminals = getNonterminals sT rules
                                        in putStr (recPrint [ [(s,e) | (s,e) <- rules, s == nt] | nt <- nonTerminals]) where
                                            recPrint :: [[GrammarRule]] -> String
                                            recPrint [] = []
                                            recPrint (x:xs) = printRuleGroup x ++ recPrint xs
