module Automaton (
        Transition,
        DeterministicAutomaton,
        NonDeterministicAutomaton,
        ENonDeterministicAutomaton,
        Symbol (Symbol, Epsilon),
        ETransition,
        printTransitionTable,
        toDeterministic,
        enToDeterministic,
        test,
        eNTest,
    ) where
    import Data.List
    import Data.Maybe
    import Data.Unique
    import Data.Map (Map)
    import qualified Data.Map as Map

    tabSize :: Int
    tabSize = 8

    charToStr :: Char -> String
    charToStr c = c : []

    -- (Origin, Symbol, Destination)
    type Transition = (Int,Char,Int)
    --(State count, Start, Finish states, Alphabet, Transitions)
    type DeterministicAutomaton = (Int, Int, [Int], String, [Transition])
    --(State count, Start states, Finish states, Alphabet, Transitions)
    type NonDeterministicAutomaton = (Int, [Int], [Int], String, [Transition])
    --Origin states,symbol,Destination states, IsInitial, IsFinal
    type ConvertedTransition = ([Int],Char,[Int],Bool,Bool)
    --ConvertedTransitions, Initial state, final states
    type ConvertInfo = ([Transition], Int, [Int])

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

    isEpsilon :: Symbol -> Bool
    isEpsilon Epsilon   = True
    isEpsilon _         = False

    isCharacter :: Symbol -> Char -> Bool
    isCharacter symb c =  case symb of
                            Epsilon -> False
                            Symbol s -> (s == c)


    -- Example of automatons.
    ex1 :: DeterministicAutomaton
    ex1 = (3, 0, [2], "ab", [(0,'a',1), (0,'b',0), (1,'a',1),(1,'b',2), (2,'a',1), (2,'b',0)])

    ex12 :: DeterministicAutomaton
    ex12 = (5, 0, [0,1,2], "ab", [(0,'a',1), (0,'b',0), (1,'a',1),(1,'b',2), (2,'a',1), (2,'b',0), (3,'a',4), (3,'b',3), (4,'a',3), (4,'b',4)])

    ex2 :: NonDeterministicAutomaton
    ex2 = (3, [0], [2], "ab", [(0,'a',1),(0,'a',0), (0,'b',0), (1,'b',2)])

    utin :: NonDeterministicAutomaton
    utin = (3, [0,1], [0], "ab", [(0,'b',1),(0,'b',2),(1,'a',1), (1,'a',2), (1,'b',2), (2,'a',0)])

    {- |
        as  - actual state
        t   - transitions
        os  - origin state of transition
        s   - symbol of transition
        ds  - destination state of transition
    -}
    deterministicStep :: String -> Int -> [Transition] -> Int
    deterministicStep [] state  _ = state
    deterministicStep (x:xs) as t = let [ds] = [ ds | (os, s, ds) <- t, s == x, os == as]
                                    in deterministicStep xs ds t
                                        
                                        
    {- |
        cs  - current states
        t   - transitions
        os  - origin state of transition
        s   - symbol of transition
        ds  - destination state of transition
        ns  - new states
    -}
    nonDeterministicStep :: String -> [Int] -> [Transition] -> [Int]
    nonDeterministicStep [] states  _   =   states
    nonDeterministicStep (x:xs) cs t    =   let ns = [ ds | (os, s, ds) <- t, s == x, elem os cs ]
                                            in nonDeterministicStep xs ns t

    test :: DeterministicAutomaton -> String -> Bool
    test (_,start,finalStates,_,transitions) word = elem (deterministicStep word start transitions) finalStates

    ntest :: NonDeterministicAutomaton -> String -> Bool
    ntest (_,start,finalStates,_,transitions) word  =   let newStates = (nonDeterministicStep word start transitions) 
                                                        in elem True [ elem x finalStates| x <- newStates]

    {- |
        sc      - state count
        is      - initial states
        fs      - final states
        a       - alphabet
        trans   - transitions
    -}
    printTransitionTable :: DeterministicAutomaton -> IO()
    printTransitionTable (sc,is,fs,a,trans) = putStr((printHeader a) ++ concat (printTable sc is fs (sort trans))) where
        printHeader :: String -> String
        printHeader alphabet    = "\t\t|"       ++ printSymbols alphabet where
            printSymbols []     = "\n"          ++ (replicate (((length alphabet) + 2) * tabSize ) '-') ++ "\n"
            printSymbols (x:xs) = (x : "\t|")   ++ printSymbols xs
        
        printTable :: Int -> Int -> [Int] -> [Transition] -> [String]
        printTable _ _ _ [] = []
        printTable sc start finish trans =  [ (printState start finish [ (o,s,d) | (o,s,d) <- trans, o == state ]) | state <- [0..(sc-1)]]
                            

    --Start state of automaton, transitions for some state
    printState :: Int -> [Int] -> [Transition] -> String
    printState initState finish trans = (getOrigin initState finish (head trans)) ++ "\t|" ++ (concat[show d ++ "\t|" | (_,_,d) <- trans]) ++ "\n" where
        getOrigin :: Int -> [Int] -> Transition -> String
        getOrigin start finalStates (s,_,_)     
            | (start == s) && (elem s finalStates)  = "<->\t|" ++ (show s)  -- State 's' is initial and also final.
            | start == s                            = "-->\t|" ++ (show s)  -- State 's' is initial. 
            | (elem s finalStates)                  = "<--\t|" ++ (show s)  -- State 's' is final.
            | otherwise                             = "\t|" ++ (show s)     -- State 's' is neither initial nor final.

    {- |
        is_a    - initial state of a converted transition
        is_b    - initial state of b converted transition
    -}
    fromSameStateEq :: ConvertedTransition -> ConvertedTransition -> Bool
    fromSameStateEq (is_a,_,_,_,_) (is_b,_,_,_,_) = is_a == is_b

    nameMapping :: [ConvertedTransition] -> Map [Int] Int
    nameMapping transitions = createMapping [x | (x,_,_,_,_) <- (nubBy (fromSameStateEq) transitions)] where
        createMapping :: [[Int]] -> Map [Int] Int
        createMapping list = Map.fromList (map makePair list) where
            makePair x = (x, fromJust(elemIndex x list))

    {- |
        ttList  - list of converted transitions
        mapping - mappig of converted states into 'nominal' states
        ndO     - Converted transition origin
        s       - Converted transition symbol
        ndD     - Converted transition destination
        x       - List of initial states in converted transition       
        f       - Bool if origin state of converted transition is final.
    -}
    ttToDt :: String -> [ConvertedTransition] -> DeterministicAutomaton
    ttToDt alphabet transitions = finalTransform alphabet (getConvertInfo transitions (nameMapping transitions)) where
        getConvertInfo :: [ConvertedTransition] -> Map [Int] Int -> ConvertInfo
        getConvertInfo ttList mapping =  (
            [(fromJust(Map.lookup ndO mapping), s, fromJust(Map.lookup ndD mapping)) | (ndO, s, ndD,_,_) <- ttList], 
            head [fromJust(Map.lookup x mapping) | (x,_,_,_,_)<- [fromJust(find (isInitial) ttList)]], 
            [fromJust(Map.lookup x mapping) | (x,_,_,_,f) <- nubBy (fromSameStateEq) ttList, f == True ]) where
                isInitial :: ConvertedTransition -> Bool
                isInitial (_,_,_,i,_) = i == True   

        finalTransform :: String -> ConvertInfo -> DeterministicAutomaton
        finalTransform alphabet (trans, initialState, finalStates) = ((div (length trans) 2 ), initialState, finalStates, alphabet, trans)

    {- |
        is      - initial states
        fs      - final states
        a       - alphabet
        trans   - transitions
        tmpCV   - newly created converted transitions
        o       - origin state of transition
        ts      - symbol of transition
        d       - destination state of transition
    -}
    toDeterministic :: NonDeterministicAutomaton -> DeterministicAutomaton
    toDeterministic (_, is, fs, a, trans) = ttToDt a (convertTransitions [is] []) where
        convertTransitions :: [[Int]] -> [[Int]] -> [ConvertedTransition]
        convertTransitions [] _ = []
        convertTransitions (x:xs) ad
            | Data.List.null ([x] Data.List.\\ ad) = convertTransitions xs ad
            | x == [] && (length xs) == 0   = []
            | otherwise                     =   let tmpCV = [(x, s, nub [d | (o,ts,d) <- trans, (elem o x) && (ts == s)], (x == is), (isFinal x) ) | s <- a ] 
                                                in tmpCV ++ convertTransitions xs (x : ad) ++ convertTransitions (nub [dest | (_,_,dest,_,_) <- tmpCV]) (x : ad)
        
        isFinal :: [Int] -> Bool
        isFinal stateList = (length [s | s <- stateList, elem s fs]) > 0

    epsilonConnectedStates :: [Int] -> [ETransition] -> [Int]
    epsilonConnectedStates [] _                 = []
    epsilonConnectedStates states transitions   =   let ecs = [ds | (os, s, ds) <- transitions, elem os states, isEpsilon s]
                                                    in ecs ++ epsilonConnectedStates ecs transitions

    enToDeterministic :: ENonDeterministicAutomaton -> DeterministicAutomaton
    enToDeterministic (_, is, fs, a, trans) = ttToDt a (nub (convertTransitions [is ++ (epsilonConnectedStates is trans)] [])) where
        convertTransitions :: [[Int]] -> [[Int]] -> [ConvertedTransition]
        convertTransitions [] _ = []
        convertTransitions (x:xs) ad
            | Data.List.null ([x] Data.List.\\ ad) = convertTransitions xs ad
            | otherwise =   let 
                            tmpCV = [(x, s, nub [d | (o,ts,d) <- trans, (elem o x) && (isCharacter ts s)], (x == is), (isFinal x) ) | s <- a ]
                            newStates = (nub [dest | (_,_,dest,_,_) <- tmpCV])
                        in [((sort a),b,sort (c ++ epsilonConnectedStates c trans), ((sort a) == sort (is ++ epsilonConnectedStates is trans)),e) | (a,b,c,d,e) <- tmpCV] ++ 
                            convertTransitions xs (x : ad) ++ 
                            convertTransitions [ ns ++ (epsilonConnectedStates ns trans) | ns <- newStates] (x : ad)
                            
        isFinal :: [Int] -> Bool
        isFinal stateList = (length [s | s <- stateList, elem s fs]) > 0

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