import Data.List
import Data.Char

tsShow :: TerminalSymbol -> String
tsShow (Symbol a) = [a]
tsShow Epsilon    = "epsilon"

gsShow :: GrammarSymbol -> String
gsShow (Nonterminal a) = [id a]
gsShow (Terminal a) = show a

rlShow :: RuleRight -> String
rlShow (SR a) = gsShow a
rlShow (CR a) = showComplex a where
    showComplex :: [GrammarSymbol] -> String
    showComplex [] = []
    showComplex (x:xs) = gsShow x ++ showComplex xs

instance Show TerminalSymbol where show a = tsShow a
instance Show GrammarSymbol where show a = gsShow a
instance Show RuleRight where show a = rlShow a

data TerminalSymbol = Symbol Char | Epsilon deriving (Eq)
type NonterminalSymbol = Char

data GrammarSymbol = Nonterminal NonterminalSymbol | Terminal TerminalSymbol deriving (Eq)

-- Right side of context-free grammar rule (SR - simple rule, CR - combined rule)
data RuleRight = SR GrammarSymbol | CR [GrammarSymbol]

type Rule = (NonterminalSymbol, RuleRight)

type ContextFreeGrammar = (NonterminalSymbol, [Rule])

ruleFromSameNt :: Rule -> Rule -> Bool
ruleFromSameNt (ntA,_) (ntB,_) = ntA == ntB

ex :: ContextFreeGrammar
ex = ('A', (r))

r :: [Rule]
r = [
    ('A', (CR [ (Terminal(Symbol 'a')), (Nonterminal 'B'), (Nonterminal 'B'), (Terminal(Symbol 'b'))] )),
    ('A', (CR [ (Nonterminal 'A'), (Terminal (Symbol 'a')) ,(Nonterminal 'A') ])),
    ('B', (SR (Terminal Epsilon))),
    ('B', (CR [ (Terminal (Symbol 'b')), (Nonterminal 'C'), (Nonterminal 'A') ])),
    ('C', (CR [ (Nonterminal 'A'), (Nonterminal 'B') ])),
    ('C', (SR (Terminal (Symbol 'a')))),
    ('C', (SR (Terminal (Symbol 'b'))))
    ]

mapWoLast :: (a -> a) -> [a] -> [a]
mapWoLast f []      = []
mapWoLast f [x]     = [x]
mapWoLast f (x:xs)  = f x : mapWoLast f xs

getNonterminals :: NonterminalSymbol -> [Rule] -> [NonterminalSymbol]
getNonterminals initial other = initial : [ nT | (nT,_) <- (nubBy ruleFromSameNt other), not (nT == initial)]

printRuleGroup :: [Rule] -> String
printRuleGroup rules =  let (nt,t) = (head rules)
                        in ([id nt]) ++ " --> " ++ (concat (mapWoLast (++ " | ") [ (show rT) | (rNT, rT) <- rules])) ++ "\n"

printContextFreeGrammar :: ContextFreeGrammar -> IO ()
printContextFreeGrammar (sT,rules) =    let nonTerminals = getNonterminals sT rules
                                        in putStr (recPrint [ [(s,e) | (s,e) <- rules, s == nt] | nt <- nonTerminals]) where
                                            recPrint :: [[Rule]] -> String
                                            recPrint [] = []
                                            recPrint (x:xs) = printRuleGroup x ++ recPrint xs

leftDerivate :: ContextFreeGrammar -> String
leftDerivate _ = ""