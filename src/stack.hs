module Stack where

    newtype Stack a = St [a] deriving (Show)

    push :: a -> Stack a -> Stack a
    push x (St xs) = St (x : xs)

    pop :: Stack a -> Stack a
    pop (St (x:xs)) = St xs

    top :: Stack a -> a
    top (St (x:_)) = x


    isEmpty :: Stack a -> Bool
    isEmpty (St list) = (length list) == 0

    