import Regex
import Automaton hiding (printTransitionTable, printState, ex1)
import GrammarTypes
import Data.List
import Data.Maybe
import Data.Char

-- Rule ContextFreeGrammar  RuleRight TerminalSymbol Terminal Nonterminal NonterminalSymbol GrammarSymbol


ruleFromSameNt :: Rule -> Rule -> Bool
ruleFromSameNt (ntA,_) (ntB,_) = ntA == ntB

ex1 :: ContextFreeGrammar
ex1 = ("A", [
    ("A", (CR [ (Terminal(CFGSymbol 'a')), (Nonterminal "B"), (Nonterminal "B"), (Terminal(CFGSymbol 'b'))] )),
    ("A", (CR [ (Nonterminal "A"), (Terminal (CFGSymbol 'a')) ,(Nonterminal "A") ])),
    ("B", (SR (Terminal CFGEpsilon))),
    ("B", (CR [ (Terminal (CFGSymbol 'b')), (Nonterminal "C"), (Nonterminal "A") ])),
    ("C", (CR [ (Nonterminal "A"), (Nonterminal "B") ])),
    ("C", (SR (Terminal (CFGSymbol 'a')))),
    ("C", (SR (Terminal (CFGSymbol 'b'))))
    ])



regEx1 :: RegularExpression
regEx1 = Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))

regEx2 :: RegularExpression
regEx2 = Conc (Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))) (RESymb (Symbol 'b'))

--     (a+b)*ab
regEx3 :: RegularExpression
regEx3 = Iter ( Conc (Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))) (RESymb (Symbol 'b')) )

mapWoLast :: (a -> a) -> [a] -> [a]
mapWoLast f []      = []
mapWoLast f [x]     = [x]
mapWoLast f (x:xs)  = f x : mapWoLast f xs

getNonterminals :: NonterminalSymbol -> [Rule] -> [NonterminalSymbol]
getNonterminals initial other = initial : [ nT | (nT,_) <- (nubBy ruleFromSameNt other), not (nT == initial)]

printRuleGroup :: [Rule] -> String
printRuleGroup rules =  let (nt,t) = (head rules)
                        in nt ++ " --> " ++ (concat (mapWoLast (++ " | ") [ (show rT) | (rNT, rT) <- rules])) ++ "\n"

printCFG :: ContextFreeGrammar -> IO ()
printCFG (sT,rules) =    let nonTerminals = getNonterminals sT rules
                                        in putStr (recPrint [ [(s,e) | (s,e) <- rules, s == nt] | nt <- nonTerminals]) where
                                            recPrint :: [[Rule]] -> String
                                            recPrint [] = []
                                            recPrint (x:xs) = printRuleGroup x ++ recPrint xs

tryFindRule :: [Rule] -> RuleRight -> Maybe Rule
tryFindRule rules wantedRight = let filter = [ (a,rightSide) | (a,rightSide) <- rules, rightSide == wantedRight]
                                in if (length filter) == 1 then Just (head filter) else Nothing

regexToCFG :: RegularExpression -> [Rule]
regexToCFG regEx = nub(convertRegex regEx []) where
    convertRegex :: RegularExpression -> [Rule] -> [Rule]
    convertRegex (RESymb s) r   = case s of Epsilon ->  let foundRule = tryFindRule r (SR (Terminal CFGEpsilon))
                                                        in if isJust foundRule then 
                                                            [fromJust foundRule] 
                                                        else 
                                                            [("S" ++ (show (length r)), (SR (Terminal CFGEpsilon)))]
                                            Symbol s->  let foundRule = tryFindRule r (SR (Terminal (CFGSymbol s)))
                                                        in if isJust foundRule then 
                                                            [fromJust foundRule] 
                                                        else 
                                                            [("S" ++ (show (length r)), (SR (Terminal (CFGSymbol s))))]

    convertRegex (Or a b) r     =   let 
                                        i = length r
                                        aRules = convertRegex a r
                                        bRules = (convertRegex b (r ++ aRules))
                                        offset = (length aRules) + (length bRules) + i
                                    in [("S" ++ (show offset), (SR (Nonterminal nt))) | (nt,_) <- [head aRules, head bRules]] 
                                        ++ aRules ++ bRules
    
    convertRegex (Conc a b) r   =   let 
                                        i = length r
                                        aRules = convertRegex a r
                                        bRules = (convertRegex b (r ++ aRules))
                                        offset = (length aRules) + (length bRules) + i
                                    in [(("S" ++ (show offset)), (CR [ Nonterminal nt | (nt,_) <- [head aRules, head bRules]]))] 
                                        ++ aRules ++ bRules
                                        
    convertRegex (Iter a) r     =   let 
                                        rules = convertRegex a r
                                        epsRuleNt = ("S" ++ (show (length rules)))
                                        epsRule = (epsRuleNt, (SR (Terminal CFGEpsilon)))
                                        (lastNt,_) = head rules
                                    in [epsRule] ++ [(epsRuleNt,(CR [Nonterminal lastNt, Nonterminal epsRuleNt]))] ++ rules