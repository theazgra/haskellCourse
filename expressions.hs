class Visible a where
    toString :: a -> String
    size :: a -> Int
    size x = length (toString x)

instance Visible Char where
    toString c      = [c]
    size _          = 1

instance Visible Bool where
    toString True   = "True"
    toString False  = "False"
    size            = length . toString

instance Visible a => Visible [a] where
    toString        = concat . map toString
    size            = foldr (+) 0 . map size

instance Visible Int where
    toString x      = show x

data Color = Red | Green | Blue

data Point = Point Float Float

-----------------------------------------------------------
-- Start of lession
-----------------------------------------------------------
data Expr = Num Int
        | Add Expr Expr
        | Sub Expr Expr
        | Mul Expr Expr
        | Div Expr Expr
        | Var Char
        deriving (Eq)

expr1 :: Expr
expr1 = Mul (Add (Num 1) (Num 2)) (Div (Mul (Num 9) (Num 10)) (Add (Num 5) (Num 5)))
            
eval :: Expr -> Int
eval exprType = case exprType of
                    Num x    -> x
                    Add x y  -> eval x + eval y
                    Sub x y  -> eval x - eval y
                    Mul x y  -> eval x * eval y
                    Div x y  -> eval x `div` eval y

eval2 :: Expr -> Int
eval2 (Num x)    = x
eval2 (Add x y)  = eval2 x + eval2 y
eval2 (Sub x y)  = eval2 x - eval2 y
eval2 (Mul x y)  = eval2 x * eval2 y
eval2 (Div x y)  = eval2 x `div` eval2 y

addParen :: Bool -> Bool -> String
addParen True False = "("
addParen True True = ")"
addParen _ _ = []

showExpr :: Expr -> String
showExpr z = realShowExpr z False where
    realShowExpr :: Expr -> Bool -> String
    realShowExpr (Num x)   _  = show x
    realShowExpr (Add x y) p  = (addParen p False) ++ (realShowExpr x False) ++ "+" ++ (realShowExpr y False) ++ (addParen p True)
    realShowExpr (Sub x y) p  = (addParen p False) ++ (realShowExpr x False) ++ "-" ++ (realShowExpr y False) ++ (addParen p True)
    realShowExpr (Mul x y) _  = (realShowExpr x True) ++ "*" ++ (realShowExpr y True)
    realShowExpr (Div x y) _  = (realShowExpr x True) ++ "/" ++ (realShowExpr y True)

ex :: Expr
ex = Add (Num 5) (Num 8)
    
deriv :: Expr -> Char -> Expr
deriv (Num x) _    = Num 0
deriv (Var x) y | x == y    = Num 1
                | otherwise = Num 0
deriv (Add x y) z  = Add (deriv x z) (deriv y z)
deriv (Sub x y) z  = Sub (deriv x z) (deriv y z)
deriv (Mul x y) z  = Add (Mul (deriv x z) y) (Mul x (deriv y z))
--deriv (Div x y)  = deriv x `div` deriv y

data RegularExpression = Symb Char
                        | Iter RegularExpression
                        | Concat RegularExpression RegularExpression -- Concatenation
                        | Or RegularExpression RegularExpression
                        deriving (Eq,Show)

re1 :: RegularExpression
--     (a+b)*ab
re1 = Concat (Concat (Iter (Or (Symb 'a') (Symb 'b'))) (Symb 'a')) (Symb 'b')