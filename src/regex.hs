module Regex where
import Automaton
import Data.List
import Test.HUnit   

data Symbol = Symbol Char
            | Epsilon

symbEq :: Symbol -> Symbol -> Bool
symbEq (Symbol a) (Symbol b)    = a == b
symbEq Epsilon Epsilon          = True
symbEq _ _                      = False

symbShow :: Symbol -> String
symbShow (Symbol a) = [a]
symbShow Epsilon    = "Epsilon"

instance Eq Symbol where
    a == b  = symbEq a b
instance Show Symbol where
    show a = symbShow a

type ETransition = (Int, Symbol, Int)

type ENonDeterministicAutomaton = (Int, [Int], [Int], String, [ETransition]) 

data RegularExpression = RESymb Symbol
                        | Iter RegularExpression
                        | Conc RegularExpression RegularExpression -- Concatenation
                        | Or RegularExpression RegularExpression
                        deriving (Eq,Show)

--     (a+b)*ab
reTest :: RegularExpression
reTest = Conc (Conc (Iter (Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b')))) (RESymb (Symbol 'a'))) (RESymb (Symbol 'b'))

isEpsilon :: Symbol -> Bool
isEpsilon Epsilon   = True
isEpsilon _         = False

epsAutEx :: ENonDeterministicAutomaton
epsAutEx = (5, [0], [0,4], "ab", [
    (0,(Symbol 'a'),1),(0,(Symbol 'a'),2),(0,(Symbol 'b'),0),(1,(Symbol 'b'),4),
    (1,(Symbol 'a'),3),(2,(Symbol 'b'),3),(3,(Symbol 'b'),2),(3,(Symbol 'b'),4),
    (3, Epsilon, 0),(4,(Symbol 'b'),4)])

epsAutEx2 :: ENonDeterministicAutomaton
epsAutEx2 = (3, [0], [0], "ab", [(0,(Symbol 'b'),1),(0,Epsilon,2),(1,(Symbol 'a'),1),(1,(Symbol 'a'),2),(1,(Symbol 'b'),2),(2,(Symbol 'a'),0)])

epsilonConnectedStates :: [Int] -> [ETransition] -> [Int]
epsilonConnectedStates [] _                 = []
epsilonConnectedStates states transitions   =   let ecs = [ds | (os, s, ds) <- transitions, elem os states, isEpsilon s]
                                                in ecs ++ epsilonConnectedStates ecs transitions

eNTest :: ENonDeterministicAutomaton -> String -> Bool
eNTest (_,start,finalStates,_,transitions) word =   let wordFinalStates = (eStep word (start ++ epsilonConnectedStates start transitions) transitions) 
                                                    in elem True [elem x finalStates | x <- wordFinalStates] 

eStep :: String -> [Int] -> [ETransition] -> [Int]
eStep [] finalStates _                  = finalStates
eStep (x:xs) currentStates transitions  =   let newStates = [ ds | (os, s, ds) <- transitions, checkETransition os s]
                                            in eStep xs (newStates ++ epsilonConnectedStates newStates transitions) transitions where
                                                checkETransition :: Int -> Symbol -> Bool
                                                checkETransition os (Symbol c)  = elem os currentStates && c == x
                                                checkETransition _ _            = False

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

nfaConcat :: ENonDeterministicAutomaton -> ENonDeterministicAutomaton -> ENonDeterministicAutomaton
nfaConcat (aSc,aS,aF,aAlph,aT) (bSc,bS,bF,bAlph,bT) =   ((aSc + bSc),
                                                        aS,
                                                        (renameStates bF aSc),
                                                        nub(aAlph ++ bAlph),
                                                        (aT ++ [(os, Epsilon, ds)| os <- aF, ds <- (renameStates bS aSc)  ] ++ (renameTrans bT aSc))
                                                        )

nfaIter :: ENonDeterministicAutomaton -> ENonDeterministicAutomaton
nfaIter (aSc,aS,aF,aAlph,aT) =  (aSc + 1,
                                [0],
                                [0],
                                aAlph,
                                [(0, Epsilon, (s + 1)) | s <- aS] ++ (renameTrans aT 1) ++ [((fs + 1),Epsilon,0) | fs <- aF]
                                )