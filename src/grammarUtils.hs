import Regex
import Automaton hiding (printTransitionTable, printState, ex1)
import GrammarTypes
import Data.List
import Data.Char

-- Rule ContextFreeGrammar  RuleRight TerminalSymbol Terminal Nonterminal NonterminalSymbol GrammarSymbol


ruleFromSameNt :: Rule -> Rule -> Bool
ruleFromSameNt (ntA,_) (ntB,_) = ntA == ntB

ex1 :: ContextFreeGrammar
ex1 = ('A', [
    ('A', (CR [ (Terminal(CFGSymbol 'a')), (Nonterminal 'B'), (Nonterminal 'B'), (Terminal(CFGSymbol 'b'))] )),
    ('A', (CR [ (Nonterminal 'A'), (Terminal (CFGSymbol 'a')) ,(Nonterminal 'A') ])),
    ('B', (SR (Terminal CFGEpsilon))),
    ('B', (CR [ (Terminal (CFGSymbol 'b')), (Nonterminal 'C'), (Nonterminal 'A') ])),
    ('C', (CR [ (Nonterminal 'A'), (Nonterminal 'B') ])),
    ('C', (SR (Terminal (CFGSymbol 'a')))),
    ('C', (SR (Terminal (CFGSymbol 'b'))))
    ])

--     (a+b)*ab
regEx :: RegularExpression
regEx = Iter ( Conc (Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))) (RESymb (Symbol 'b')) )

mapWoLast :: (a -> a) -> [a] -> [a]
mapWoLast f []      = []
mapWoLast f [x]     = [x]
mapWoLast f (x:xs)  = f x : mapWoLast f xs

getNonterminals :: NonterminalSymbol -> [Rule] -> [NonterminalSymbol]
getNonterminals initial other = initial : [ nT | (nT,_) <- (nubBy ruleFromSameNt other), not (nT == initial)]

printRuleGroup :: [Rule] -> String
printRuleGroup rules =  let (nt,t) = (head rules)
                        in ([id nt]) ++ " --> " ++ (concat (mapWoLast (++ " | ") [ (show rT) | (rNT, rT) <- rules])) ++ "\n"

printCFG :: ContextFreeGrammar -> IO ()
printCFG (sT,rules) =    let nonTerminals = getNonterminals sT rules
                                        in putStr (recPrint [ [(s,e) | (s,e) <- rules, s == nt] | nt <- nonTerminals]) where
                                            recPrint :: [[Rule]] -> String
                                            recPrint [] = []
                                            recPrint (x:xs) = printRuleGroup x ++ recPrint xs
