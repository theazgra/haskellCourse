import Test.HUnit
import Regex

-- Union tests
-- (a+b)
union1 :: RegularExpression
union1 = Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))
union1aut = regexToNfa union1

-- (a+b)+c
union2 :: RegularExpression
union2 = Or (Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))) (RESymb(Symbol 'c'))
union2aut = regexToNfa union2

-- (a+b)+(c+d)
union3 :: RegularExpression
union3 = Or (Or (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))) (Or (RESymb (Symbol 'c')) (RESymb (Symbol 'd')))
union3aut = regexToNfa union3

un1t1 = TestCase (assertEqual "Regular expression: (a+b), input: a" True            (eNTest union1aut "a"))
un1t2 = TestCase (assertEqual "Regular expression: (a+b), input: b" True            (eNTest union1aut "b"))
un1t3 = TestCase (assertEqual "Regular expression: (a+b), input: aa" False          (eNTest union1aut "aa"))
un1t4 = TestCase (assertEqual "Regular expression: (a+b), input: bb" False          (eNTest union1aut "bb"))
un1t5 = TestCase (assertEqual "Regular expression: (a+b), input: ab" False          (eNTest union1aut "ab"))
un1t6 = TestCase (assertEqual "Regular expression: (a+b), input: ba" False          (eNTest union1aut "ba"))

un2t1 = TestCase (assertEqual "Regular expression: (a+b)+c, input: a" True          (eNTest union2aut "a"))
un2t2 = TestCase (assertEqual "Regular expression: (a+b)+c, input: b" True          (eNTest union2aut "b"))
un2t3 = TestCase (assertEqual "Regular expression: (a+b)+c, input: c" True          (eNTest union2aut "c"))
un2t4 = TestCase (assertEqual "Regular expression: (a+b)+c, input: aa" False        (eNTest union2aut "aa"))
un2t5 = TestCase (assertEqual "Regular expression: (a+b)+c, input: bb" False        (eNTest union2aut "bb"))
un2t6 = TestCase (assertEqual "Regular expression: (a+b)+c, input: cc" False        (eNTest union2aut "cc"))
un2t7 = TestCase (assertEqual "Regular expression: (a+b)+c, input: ab" False        (eNTest union2aut "ab"))
un2t8 = TestCase (assertEqual "Regular expression: (a+b)+c, input: ac" False        (eNTest union2aut "ac"))
un2t9 = TestCase (assertEqual "Regular expression: (a+b)+c, input: bc" False        (eNTest union2aut "bc"))

un3t1 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: a" True      (eNTest union3aut "a"))
un3t2 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: b" True      (eNTest union3aut "b"))
un3t3 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: c" True      (eNTest union3aut "c"))
un3t4 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: d" True      (eNTest union3aut "d"))
un3t5 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: aa" False    (eNTest union3aut "aa"))
un3t6 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: bb" False    (eNTest union3aut "bb"))
un3t7 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: cc" False    (eNTest union3aut "cc"))
un3t8 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: dd" False    (eNTest union3aut "dd"))
un3t9 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: ac" False    (eNTest union3aut "ac"))
un3t10 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: ad" False   (eNTest union3aut "ad"))
un3t11 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: bc" False   (eNTest union3aut "bc"))
un3t12 = TestCase (assertEqual "Regular expression: (a+b)+(c+d), input: bd" False   (eNTest union3aut "bd"))

-- Concatenation tests.
--(ab)
concat1 :: RegularExpression
concat1 = Conc (RESymb (Symbol 'a')) (RESymb (Symbol 'b'))
concat1aut = regexToNfa concat1

-- a b c d e
concat2 :: RegularExpression
concat2 = Conc (RESymb (Symbol 'a')) (Conc (RESymb (Symbol 'b')) (Conc (RESymb (Symbol 'c')) (Conc (RESymb (Symbol 'd')) (RESymb (Symbol 'e')))))
concat2aut = regexToNfa concat2

cn1t1 = TestCase (assertEqual "Regular expression: (ab), input: ab" True            (eNTest concat1aut "ab"))
cn1t2 = TestCase (assertEqual "Regular expression: (ab), input: ba" False           (eNTest concat1aut "ba"))
cn1t3 = TestCase (assertEqual "Regular expression: (ab), input: abab" False         (eNTest concat1aut "abab"))
cn1t4 = TestCase (assertEqual "Regular expression: (ab), input: aab" False          (eNTest concat1aut "aab"))
cn1t5 = TestCase (assertEqual "Regular expression: (ab), input: abb" False          (eNTest concat1aut "abb"))
cn1t6 = TestCase (assertEqual "Regular expression: (ab), input: bb" False           (eNTest concat1aut "bb"))

cn2t1 = TestCase (assertEqual "Regular expression: (abcde), input: abcde" True      (eNTest concat2aut "abcde"))
cn2t2 = TestCase (assertEqual "Regular expression: (abcde), input: a" False         (eNTest concat2aut "a"))
cn2t3 = TestCase (assertEqual "Regular expression: (abcde), input: abc" False       (eNTest concat2aut "abc"))
cn2t4 = TestCase (assertEqual "Regular expression: (abcde), input: abcd" False      (eNTest concat2aut "abcd"))
cn2t5 = TestCase (assertEqual "Regular expression: (abcde), input: q" False         (eNTest concat2aut "q"))
cn2t6 = TestCase (assertEqual "Regular expression: (abcde), input: eps" False       (eNTest concat2aut ""))

-- Iteration tests.
--a*
iter1 :: RegularExpression
iter1 = Iter (RESymb (Symbol 'a'))
iter1aut = regexToNfa iter1

iter1t1 = TestCase (assertEqual "Regular expression: (a*), input: a" True           (eNTest iter1aut "a"))
iter1t2 = TestCase (assertEqual "Regular expression: (a*), input: aaaaaaaaa" True   (eNTest iter1aut "aaaaaaaaa"))
iter1t3 = TestCase (assertEqual "Regular expression: (a*), input: aaaaaaaaab" False (eNTest iter1aut "aaaaaaaaab"))
iter1t4 = TestCase (assertEqual "Regular expression: (a*), input: bbbbb" False      (eNTest iter1aut "bbbbb"))
iter1t5 = TestCase (assertEqual "Regular expression: (a*), input: eps" True         (eNTest iter1aut ""))


-- Comples regular expressions
-- a* (a+c) b*a*
comp1 :: RegularExpression
comp1 = Conc (Iter (RESymb (Symbol 'a'))) (Conc (Or (RESymb (Symbol 'a')) (RESymb (Symbol 'c'))) (Conc (Iter (RESymb (Symbol 'b'))) (Iter (RESymb (Symbol 'a')))))
comp1aut = regexToNfa comp1

comp1t1 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: a" True                (eNTest comp1aut "a"))
comp1t2 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: c" True                (eNTest comp1aut "c"))
comp1t3 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: aaaaaaaaa" True        (eNTest comp1aut "aaaaaaaaa"))
comp1t4 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: aaaacbbbbbbaaaa" True  (eNTest comp1aut "aaaacbbbbbbaaaa"))
comp1t5 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: b" False               (eNTest comp1aut "b"))
comp1t6 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: eps" False             (eNTest comp1aut ""))
comp1t7 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: dddddaacbbaa" False    (eNTest comp1aut "dddddaacbbaa"))
comp1t8 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: aabbaa" True           (eNTest comp1aut "aabbaa"))
comp1t9 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: acba" True             (eNTest comp1aut "acba"))
comp1t10 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: cbbbbbb" True         (eNTest comp1aut "cbbbbbb"))
comp1t11 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: cb" True              (eNTest comp1aut "cb"))
comp1t12 = TestCase (assertEqual "Regular expression: (a* (a+c) b*a*), input: q" False              (eNTest comp1aut "q"))


tests = TestList [
    un1t1, un1t2, un1t3, un1t4, un1t5, un1t6, un2t1, un2t2, un2t3, un2t4, un2t5, un2t6, un2t7, un2t8, un2t9,
    un3t1, un3t2, un3t3, un3t4, un3t5, un3t6, un3t7, un3t8, un3t9, un3t10, un3t11, un3t12, cn1t1, cn1t2, cn1t3, 
    cn1t4, cn1t5, cn1t6, cn2t1, cn2t2, cn2t3, cn2t4, cn2t5, cn2t6, iter1t1, iter1t2, iter1t3, iter1t4, iter1t5,
    comp1t1, comp1t1, comp1t2, comp1t3, comp1t4, comp1t5, comp1t6, comp1t7, comp1t8, comp1t9, comp1t10, comp1t11, 
    comp1t12
    ]


