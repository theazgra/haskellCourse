-- Square a number
square :: Int -> Int
square n = n * n

mySum :: Int -> Int -> Int
mySum x y = x + y

myMax :: Int -> Int -> Int
myMax x y
    | x >= y =x
    | otherwise  = y

fact1 n = if n == 0 then 1 else n * fact1 (n-1)
fact2 n
    | n == 0 = 1
    | otherwise = n * fact2(n-1)

fib n
    | n == 0 = 1
    | n == 1 = 1
    | otherwise = fib(n-1) + fib(n-2)

biggestDivider x y
    | x == y = x
    | x > y = biggestDivider (x-y) y
    | x < y = biggestDivider (y-x) x