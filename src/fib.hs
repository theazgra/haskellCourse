fibFast :: Int -> Int
fibFast n =
  let fib = 0 : 1 : zipWith (+) fib (tail fib)
  in fib!!(n - 1)

main = do 
    n <- readLn
    putStr(show(fibFast n))
