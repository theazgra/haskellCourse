module Regex where
import Automaton
import Data.List
import Data.Maybe
import Data.Map (Map)
import qualified Data.Map as Map


{- |
    Data type defining regular expression tree.
        RESymb  - Regular symbol from alphabet or Epsilon (word of length = 0)
        Iter    - Iteration. e.g. (a*)
        Conc    - Concatenation. e.g. (ab) 
        Or      - Logic `or` or union. e.g. (a + b)
-}
data RegularExpression = RESymb Symbol
                        | Iter RegularExpression
                        | Conc RegularExpression RegularExpression
                        | Or RegularExpression RegularExpression
                        deriving (Eq,Show)

--     (a+b)*ab
reTest :: RegularExpression
reTest = Conc (Conc (Iter (Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b')))) (RESymb (Symbol 'a'))) (RESymb (Symbol 'b'))

epsAutEx :: ENonDeterministicAutomaton
epsAutEx = (5, [0], [0,4], "ab", [
    (0,(Symbol 'a'),1),(0,(Symbol 'a'),2),(0,(Symbol 'b'),0),(1,(Symbol 'b'),4),
    (1,(Symbol 'a'),3),(2,(Symbol 'b'),3),(3,(Symbol 'b'),2),(3,(Symbol 'b'),4),
    (3, Epsilon, 0),(4,(Symbol 'b'),4)])

epsAutEx2 :: ENonDeterministicAutomaton
epsAutEx2 = (3, [0], [0], "ab", [(0,(Symbol 'b'),1),(0,Epsilon,2),(1,(Symbol 'a'),1),(1,(Symbol 'a'),2),(1,(Symbol 'b'),2),(2,(Symbol 'a'),0)])

--(Int, [Int], [Int], String, [ETransition]) 
regexToNfa :: RegularExpression -> ENonDeterministicAutomaton
regexToNfa (RESymb symb)    = case symb of  Epsilon     -> (1,[0],[],[],[])
                                            Symbol c    -> (2, [0], [1], [c], [(0,(Symbol c),1)])
regexToNfa (Or a b)         = nfaUnion (regexToNfa a) (regexToNfa b)
regexToNfa (Conc a b)       = nfaConcat (regexToNfa a) (regexToNfa b)
regexToNfa (Iter a)         = nfaIter (regexToNfa a)

offsetTransition :: ETransition -> Int -> ETransition
offsetTransition (os,s,od) offset = ((os+offset),s,(od+offset))

renameTrans :: [ETransition] -> Int -> [ETransition]
renameTrans transitions offset = map (\t -> offsetTransition t offset) transitions

renameStates :: [Int] -> Int -> [Int]
renameStates original offset = [ (x+offset) | x <- original]

{- |
    Union of two epsilon NFA.
-}
nfaUnion :: ENonDeterministicAutomaton -> ENonDeterministicAutomaton -> ENonDeterministicAutomaton
nfaUnion (aSc,aS,aF,aAlph,aT) (bSc,bS,bF,bAlph,bT) =    ((2 + aSc + bSc), 
                                                        [0], 
                                                        [(1 + aSc + bSc)], 
                                                        nub(aAlph ++ bAlph),
                                                        (   [(0,Epsilon,as) | as <- (renameStates aS 1)] ++ [(0,Epsilon,bs) | bs <- (renameStates bS (aSc + 1))] ++
                                                            (renameTrans aT 1) ++
                                                            (renameTrans bT (aSc + 1)) ++
                                                            [(af,Epsilon,(1 + aSc + bSc)) | af <- (renameStates aF 1)] ++ 
                                                            [(bf,Epsilon,(1 + aSc + bSc)) | bf <- (renameStates bF (aSc + 1))]
                                                        ))

{- |
    Concatenation of two epsilon NFA.
-}
nfaConcat :: ENonDeterministicAutomaton -> ENonDeterministicAutomaton -> ENonDeterministicAutomaton
nfaConcat (aSc,aS,aF,aAlph,aT) (bSc,bS,bF,bAlph,bT) =   ((aSc + bSc),
                                                        aS,
                                                        (renameStates bF aSc),
                                                        nub(aAlph ++ bAlph),
                                                        (aT ++ [(os, Epsilon, ds)| os <- aF, ds <- (renameStates bS aSc)  ] ++ (renameTrans bT aSc))
                                                        )
{- |
    Iteration of epsilon NFA
-}
nfaIter :: ENonDeterministicAutomaton -> ENonDeterministicAutomaton
nfaIter (aSc,aS,aF,aAlph,aT) =  (aSc + 1,
                                [0],
                                [0],
                                aAlph,
                                [(0, Epsilon, (s + 1)) | s <- aS] ++ (renameTrans aT 1) ++ [((fs + 1),Epsilon,0) | fs <- aF]
                                )

{- |
    Sample regular expression for testing.
-}
reA_ex :: RegularExpression
reA_ex = RESymb (Symbol 'a')

reB_ex :: RegularExpression
reB_ex = RESymb (Symbol 'b')

reAB_ex :: RegularExpression
reAB_ex = Conc (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))