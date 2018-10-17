import Automaton

data Symbol = Symbol Char
                | Epsilon

type ETransition = (Int, Symbol, Int)

type ENonDeterministicAutomaton = (Int, [Int], [Int], String, [ETransition]) 

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
epsilonConnectedStates states transitions = [ds | (os, s, ds) <- transitions, elem os states, isEpsilon s]

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