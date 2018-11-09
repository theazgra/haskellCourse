module CfgExamples where
    import GrammarTypes

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

    reduceEx1 :: ContextFreeGrammar
    reduceEx1 = ("S", [
        ("S", (CR [(Nonterminal "A"), (Nonterminal "C")])),
        ("S", (SR (Nonterminal "B"))),
        ("A", (CR [(Terminal (CFGSymbol 'a')),(Nonterminal "C")])),
        ("A", (CR [(Nonterminal "A"),(Terminal (CFGSymbol 'b')),(Nonterminal "A")])),
        ("B", (CR [(Nonterminal "B"),(Terminal (CFGSymbol 'a'))])),
        ("B", (CR [(Nonterminal "B"),(Terminal (CFGSymbol 'b')),(Nonterminal "A")])),
        ("B", (CR [(Nonterminal "D"),(Nonterminal "B")])),
        ("C", (CR [(Terminal (CFGSymbol 'a')),(Terminal (CFGSymbol 'a'))])),
        ("C", (CR [(Terminal (CFGSymbol 'a')),(Nonterminal "B"),(Nonterminal "C")])),
        ("D", (CR [(Terminal (CFGSymbol 'a')), (Nonterminal "A")])),
        ("D", (SR (Terminal CFGEpsilon)))
        ])

    reduceEx2 :: ContextFreeGrammar
    reduceEx2 = ("A", [
        ("A", (CR [(Nonterminal "B"),(Nonterminal "A"),(Nonterminal "A"),(Nonterminal "G")])),
        ("A", (CR [(Nonterminal "B"),(Nonterminal "B")])),
        ("B", (CR [(Terminal (CFGSymbol 'b')),(Terminal (CFGSymbol 'a')),(Terminal (CFGSymbol 'b'))])),
        ("B", (CR [(Nonterminal "B"),(Nonterminal "B"),(Nonterminal "B")])),
        ("B", (CR [(Nonterminal "C"),(Nonterminal "F")])),
        ("C", (CR [(Nonterminal "C"),(Nonterminal "B"), (Terminal (CFGSymbol 'a'))])),
        ("C", (CR [(Nonterminal "E"),(Terminal (CFGSymbol 'a')),(Terminal (CFGSymbol 'b'))])),
        ("D", (CR [(Terminal (CFGSymbol 'a')),(Terminal (CFGSymbol 'b'))])),
        ("D", (CR [(Nonterminal "D"),(Terminal (CFGSymbol 'a'))])),
        ("D", (SR (Terminal CFGEpsilon))),
        ("E", (CR [(Nonterminal "C"),(Nonterminal "C"),(Nonterminal "E"),(Terminal (CFGSymbol 'a'))])),
        ("E", (CR [(Nonterminal "C"),(Terminal (CFGSymbol 'a')),(Nonterminal "B")])),
        ("F", (CR [(Terminal (CFGSymbol 'a')),(Terminal (CFGSymbol 'b'))])),
        ("F", (SR (Nonterminal "D"))),
        ("G", (CR [(Nonterminal "A"),(Terminal (CFGSymbol 'a')),(Nonterminal "A")])),
        ("G", (CR [(Nonterminal "E"),(Nonterminal "E")])),
        ("G", (CR [(Nonterminal "D"),(Nonterminal "A")]))
        ])

