import Regex
import Automaton hiding (printTransitionTable, printState, ex1)
import GrammarTypes
import CfgExamples
import Data.List
import Data.Maybe
import Data.Char

regEx1 :: RegularExpression
regEx1 = Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))

regEx2 :: RegularExpression
regEx2 = Conc (Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))) (RESymb (Symbol 'b'))

--     (a+b)*ab
regEx3 :: RegularExpression
regEx3 = Iter ( Conc (Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))) (RESymb (Symbol 'b')) )

ruleFromSameNt :: Rule -> Rule -> Bool
ruleFromSameNt (ntA,_) (ntB,_) = ntA == ntB

mapWoLast :: (a -> a) -> [a] -> [a]
mapWoLast f []      = []
mapWoLast f [x]     = [x]
mapWoLast f (x:xs)  = f x : mapWoLast f xs

getAllNonterminals :: [Rule] -> [NonterminalSymbol]
getAllNonterminals rules = nub [nt | (nt,_) <- rules]

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

regexToCFG :: RegularExpression -> ContextFreeGrammar
regexToCFG regEx =  let rules = nub(convertRegex regEx []) 
                        (nt,_) = head rules 
                        in (nt, rules) where
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
                                        aRules = convertRegex a r
                                        bRules = (convertRegex b (r ++ aRules))
                                        offset = (length aRules) + (length bRules) + (length r)
                                    in [("S" ++ (show offset), (SR (Nonterminal nt))) | (nt,_) <- [head aRules, head bRules]] 
                                        ++ aRules ++ bRules
    
    convertRegex (Conc a b) r   =   let 
                                        aRules = convertRegex a r
                                        bRules = (convertRegex b (r ++ aRules))
                                        offset = (length aRules) + (length bRules) + (length r)
                                    in [(("S" ++ (show offset)), (CR [ Nonterminal nt | (nt,_) <- [head aRules, head bRules]]))] 
                                        ++ aRules ++ bRules
                                        
    convertRegex (Iter a) r     =   let 
                                        rules = convertRegex a r
                                        epsRuleNt = ("S" ++ (show (length rules)))
                                        epsRule = (epsRuleNt, (SR (Terminal CFGEpsilon)))
                                        (lastNt,_) = head rules
                                    in [epsRule] ++ [(epsRuleNt,(CR [Nonterminal lastNt, Nonterminal epsRuleNt]))] ++ rules

aut1 :: ENonDeterministicAutomaton
aut1 = (5,[1,3],[1,5],"ab",[  (1,(Symbol 'a'),2),(1,(Symbol 'a'),3),(1,(Symbol 'b'),1),(2,(Symbol 'b'),5),(2,(Symbol 'a'),4),
                            (3,(Symbol 'b'),4),(4,(Symbol 'b'),3),(4,(Symbol 'b'),5),(4,Epsilon,1),(5,(Symbol 'b'),5)])

eAutToCFG :: ENonDeterministicAutomaton -> ContextFreeGrammar
eAutToCFG (_,is,fs,_,trans) =   let
                                    initialNts  = [ ("I", (SR (Nonterminal ("S" ++ (show s))))) | s <- is]
                                    rules       = [ generateRule t | t <- trans ]
                                    finalRules  = [ (("S" ++ (show s)), (SR (Terminal CFGEpsilon))) | s <- fs ]
                                in ("I", initialNts ++ rules ++ finalRules) where
    generateRule :: ETransition -> Rule
    generateRule (is,s,fs) = case s of 
                            Symbol symb -> (("S" ++ (show is)), (CR [(Terminal (CFGSymbol symb)),(Nonterminal ("S" ++ (show fs)))] ))
                            Epsilon     -> (("S" ++ (show is)), (SR (Nonterminal ("S" ++ (show fs)))))  




isGsTerminal :: GrammarSymbol -> Bool
isGsTerminal (Terminal t)   = True
isGsTerminal _              = False

isRRTerminal :: RuleRight -> Bool
isRRTerminal (SR gs)    = isGsTerminal gs
isRRTerminal _          = False

isRRSimpleNonterminal :: RuleRight -> Bool
isRRSimpleNonterminal (SR gs)   = not (isGsTerminal gs)
isRRSimpleNonterminal _         = False

isRREpsilonTerm :: RuleRight -> Bool
isRREpsilonTerm (SR gs) = case gs of 
                            Terminal t  -> case t of 
                                            CFGEpsilon  -> True
                                            otherwise   -> False
                            otherwise   -> False
isRREpsilonTerm _       = False

unpackNtFromRR :: RuleRight -> NonterminalSymbol
unpackNtFromRR (SR gs)   = unpackNt gs
unpackNtFromRR _         = error "(unpackNtFromRR): Is not RuleRight with `SR`"

unpackNt :: GrammarSymbol -> NonterminalSymbol
unpackNt (Nonterminal nt)   = nt
unpackNt _                  = error "(unpackNT): Is not Nonerminal"

extractNts :: [RuleRight] -> [NonterminalSymbol]
extractNts []       = []
extractNts (x:xs)   = case x of 
                        (SR (Terminal t))       -> extractNts xs
                        (SR (Nonterminal nt))   -> nt : extractNts xs
                        (CR symbols)            -> [ unpackNt s | s <- symbols, not (isGsTerminal s)] ++ extractNts xs

derivTo :: RuleRight -> [NonterminalSymbol] -> Bool
derivTo (SR gs) nts         = (not (isGsTerminal gs)) && (elem (unpackNt gs) nts)
derivTo (CR symbols) nts    = (length [1 | gs <- symbols, (not (isGsTerminal gs)) && (elem (unpackNt gs) nts)]) == (length symbols)

genTerminal :: RuleRight -> [NonterminalSymbol] -> Bool
genTerminal rr ntGenT = case rr of 
    (SR (Terminal t)) -> True
    (SR (Nonterminal nt)) -> elem nt ntGenT
    (CR symbols) -> length [s | s <- symbols, ((isGsTerminal s) || elem (unpackNt s) ntGenT)] == length symbols

reduceCFG :: ContextFreeGrammar -> ContextFreeGrammar
reduceCFG (iNt, rules) =    let
                                nTsGeneratingTerminals = getNtGenT rules
                                aRules = [(nt,rr) | (nt,rr) <- rules, ((elem nt nTsGeneratingTerminals) && (isValidRightSide rr nTsGeneratingTerminals))]
                                avaibleFromInitialNt = getNtAvaibleFromINt iNt aRules
                                bRules = [(nt,rr) | (nt,rr) <- aRules, elem nt avaibleFromInitialNt]
                                finalNt = nub [nt | (nt,_) <- bRules]
                                finalRules = [(nt,rr) | (nt,rr) <-bRules, (isValidRightSide rr finalNt)]
                            in if (elem iNt finalNt) then (iNt,finalRules) else error "CFG can't be reduced." where
                                isValidRightSide :: RuleRight -> [NonterminalSymbol] -> Bool
                                isValidRightSide rightSide validNt =
                                    let 
                                        rightSideNts = extractNts [rightSide]
                                        validRightSideNts = [nt | nt <- rightSideNts, elem nt validNt]
                                    in (length rightSideNts) == (length validRightSideNts)


getNtGenT :: [Rule] -> [NonterminalSymbol]
getNtGenT rules =   let initialNts = (nub [ nt | (nt,rightSide) <- rules, isRRTerminal rightSide])
                    in recGenNt initialNts where
                        recGenNt :: [NonterminalSymbol] -> [NonterminalSymbol]
                        recGenNt f =    let new = [ nt | (nt,rs) <- rules, (genTerminal rs f), not (elem nt f)]
                                        in if (length new) == 0 then f else recGenNt (nub (f ++ new))


getNtAvaibleFromINt :: NonterminalSymbol -> [Rule] -> [NonterminalSymbol]
getNtAvaibleFromINt iNt rules = let initialNts = nub (iNt : (extractNts [ rr | (nt, rr)<- rules, nt == iNt ]))
                                in recGenNt initialNts where
                                    recGenNt :: [NonterminalSymbol] -> [NonterminalSymbol]
                                    recGenNt f =    let 
                                                        possiblyNew = nub (concat [extractNts [rs] | (nt,rs) <- rules, (elem nt f)])
                                                        reallyNew = possiblyNew Data.List.\\ f
                                                    in if (length reallyNew) == 0 then f else recGenNt (f ++ reallyNew)

isRegular :: ContextFreeGrammar -> Bool
isRegular cfg = (isLeftRegular cfg) || (isRightRegular cfg)

isLeftRegular :: ContextFreeGrammar -> Bool
isLeftRegular (_,rules) = (length [1 | (_,rr) <- rules, isLeftRegularRule rr]) == (length rules)

isRightRegular :: ContextFreeGrammar -> Bool
isRightRegular (_,rules) = (length [1 | (_,rr) <- rules, isRightRegularRule rr]) == (length rules)

isRightRegularRule :: RuleRight -> Bool
isRightRegularRule (SR _) = True
isRightRegularRule (CR rules) = ((length rules) == 2) && (isGsTerminal (rules !! 0)) && (not (isGsTerminal (rules !! 1)))

isLeftRegularRule :: RuleRight -> Bool
isLeftRegularRule (SR _) = True
isLeftRegularRule (CR rules) = ((length rules) == 2) && (not (isGsTerminal (rules !! 0))) && (isGsTerminal (rules !! 1))


{- |
    - 1. Mnozina vsechn nt, ktere jdou prepsat nejakou derivaci na Epsilon
        - Zacneme od nt, ktere se prepisuji na epsilon, a jdeme zpetne...
    - 2. Odstranime vsechny pravidla prepisu na epsilon.
    - 3. Upravime pravidla, takove, ktere obsahovali nejaky nt z mnoziny z prvniho kroku, a to tak, ze vynechame tyto
-}
removeEpsilonRules :: ContextFreeGrammar -> [NonterminalSymbol]
removeEpsilonRules (is,rules) = findNonTermToEps rules

findNonTermToEps :: [Rule] -> [NonterminalSymbol]
findNonTermToEps rules =    let initialNts = [ nt | (nt,rr) <- rules, isRREpsilonTerm rr]
                            in recFindNts initialNts where
                            recFindNts :: [NonterminalSymbol] -> [NonterminalSymbol]
                            recFindNts nts =    let newNts = nub ([nt | (nt,rr) <- rules, (not (elem nt nts) && (derivTo rr nts))])
                                                in if (length newNts) == 0 then nts else recFindNts (nts ++ newNts)

removeSimpleRules :: ContextFreeGrammar -> ContextFreeGrammar
removeSimpleRules (is,rules) = let 
    nonTerminals    = getAllNonterminals rules
    simpleNtsForNt  = findSimpleNtsForNt nonTerminals rules
    newRules        = concat (createNewRules simpleNtsForNt)
    in (is,newRules) where
        createNewRules :: [[NonterminalSymbol]] -> [[Rule]]
        createNewRules ntGroups = 
            [
                concat ([[((head group),rr) | (rNt,rr) <- rules, ((rNt == nt) && (not (isRRSimpleNonterminal rr)))] | nt <- group ])
            |  group <- ntGroups]

findSimpleNtsForNt :: [NonterminalSymbol] -> [Rule] -> [[NonterminalSymbol]]
findSimpleNtsForNt nonTerminals rules = [(recFindNts [nt]) | nt <- nonTerminals] where
    recFindNts :: [NonterminalSymbol] -> [NonterminalSymbol]
    recFindNts nts =    let 
                        newNts = nub ([(unpackNtFromRR rr) | (nt,rr) <- rules, ((elem nt nts) && (isRRSimpleNonterminal rr))])
                        reallyNew = newNts Data.List.\\ nts
                        in if length (reallyNew) == 0 then nts else recFindNts (nts ++ reallyNew)

-- http://www.cs.vsb.cz/sawa/ti/slides/ti-slides-06.pdf --78
