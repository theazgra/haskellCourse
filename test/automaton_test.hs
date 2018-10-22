import Test.HUnit
import Automaton
import Regex
-- Example of automatons.

nd1 :: NonDeterministicAutomaton
nd1 = (3, [0], [2], "ab", [(0,'a',1),(0,'a',0), (0,'b',0), (1,'b',2)])

nd1_GT :: DeterministicAutomaton
nd1_GT = (3,0,[2],"ab",[(0,'a',1),(0,'b',0),(1,'a',1),(1,'b',2),(2,'a',1),(2,'b',0)])

nd2 :: NonDeterministicAutomaton
nd2 = (3, [0,1], [0], "ab", [(0,'b',1),(0,'b',2),(1,'a',1), (1,'a',2), (1,'b',2), (2,'a',0)])

nd2_GT :: DeterministicAutomaton
nd2_GT = (6,0,[0,2,4],"ab",[(0,'a',1),(0,'b',1),(1,'a',2),(1,'b',3),(2,'a',2),(2,'b',1),(3,'a',4),(3,'b',5),(4,'a',5),(4,'b',1),(5,'a',5),(5,'b',5)])

reA :: RegularExpression
reA = RESymb (Symbol 'a')

reAB :: RegularExpression
reAB = Conc (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))

reA_dt_GT :: DeterministicAutomaton
reA_dt_GT = (1,0,[1],"a",[(0,'a',1),(1,'a',2),(2,'a',2)])

reAB_dt_GT :: DeterministicAutomaton
reAB_dt_GT = (4,0,[3],"ab",[(0,'a',1),(0,'b',2),(1,'a',2),(1,'b',3),(2,'a',2),(2,'b',2),(3,'a',2),(3,'b',2)])


nfa2dfa_1 = TestCase (
    assertEqual "Conversion of NFA (nd1) to DFA"
    True
    ((toDeterministic nd1) == nd1_GT))

nfa2dfa_2 = TestCase (
    assertEqual "Conversion of NFA (nd2) to DFA"
    True
    ((toDeterministic nd2) == nd2_GT))


re_dfa_1 = TestCase (
    assertEqual "Regular expression (a) to DFA."
    True
    ((enToDeterministic (regexToNfa reA)) == reA_dt_GT))

re_dfa_2 = TestCase (
    assertEqual "Regular expression (ab) to DFA."
    True
    ((enToDeterministic (regexToNfa reAB)) == reAB_dt_GT))
    

tests = TestList [
    nfa2dfa_1, nfa2dfa_2, re_dfa_1, re_dfa_2
    ]



    