import Data.Char
import Data.List

myLength :: [a] -> Int
myLength [] = 0
myLength (_:xs) = 1 + myLength xs
-- _ means that we won't need that value.

myConcat :: [a] -> [a] -> [a]
myConcat [] y = y 
myConcat (x:xs) y = x : (myConcat xs y)

myZip :: [a] -> [b] -> [(a,b)]
myZip [] _ = []
myZip _ [] = []
myZip (x:xs) (y:ys) = (x,y) : myZip xs ys

myReverseQuad :: [a] -> [a]
myReverseQuad [] = []
myReverseQuad (x:xs) = myConcat (myReverseQuad xs) [x]

myReverseLinear :: [a] -> [a]
myReverseLinear x = tmp x [] where
    tmp [] y = y
    tmp (x:xs) y = tmp xs (x:y)

myZipWith :: (a->b->c) -> [a] -> [b] -> [c]
myZipWith f (x:xs) (y:ys) = (f x y) : myZipWith f xs ys 
myZipWith _ _ _ = []
    -- Misto   
    -- myZipWith f [] _ = []
    -- myZipWith f _ [] = []

mySum :: [Int] -> Int
mySum [] = 0
mySum (x:xs) = x + mySum xs

myScalar x y = mySum (myZipWith (*) x y)


myScalarProduct :: [a] -> [b] -> [(a,b)]
myScalarProduct [] _ = []
myScalarProduct (x:xs) y = (myMakePairs x y) ++ (myScalarProduct xs y) where
    myMakePairs :: a -> [b] -> [(a, b)]
    myMakePairs a [] = []
    myMakePairs a (x:xs) = (a,x) : (myMakePairs a xs)

myScalarProduct2 :: [a] -> [b] -> [(a,b)]
myScalarProduct2 [] _ = []
myScalarProduct2 [x] [] = []
myScalarProduct2 [x] (y:ys) = (x,y) : (myScalarProduct2 [x] ys)
myScalarProduct2 (x:xs) y = (myScalarProduct2 [x] y) ++ (myScalarProduct2 xs y)

myMin :: Ord a => [a] -> a
myMin (x:xs) = myLocalMin xs x where
    myLocalMin :: [a] -> a -> a
    myLocalMin [] min = min
    myLocalMin (x:xs) min | min < x = myLocalMin xs min
        | otherwise = myLocalMin xs x

myMin2 :: Ord a => [a] -> a




upper :: [Char] -> [Char]
upper xs = [toUpper x | x <- xs]

dividers :: Int -> [Int]
dividers input = [divider | divider <- [1..input], mod input divider == 0]
--Can be written in INFIX
--dividers input = [divider | divider <- [1..input], input `mod` divider == 0]


type Pic = [String]
obr :: Pic
obr = [ "....#....",
        "...###...",
        "..#.#.#..",
        ".#..#..#.",
        "....#....",
        "....#....",
        "....#####" ]

--head :: Pic -> [String]

printPicture :: Pic -> IO()
printPicture x = putStr(concat(map (++"\n") x))
-- Tahle tecka je skldani funkcii
--printPicture = putStr . concat . map(++"\n")

flipH :: Pic -> Pic
flipH x = reverse x

flipV :: Pic -> Pic
flipV x = map reverse x
--flipV x = [reverse row | row <- x]

placeAbove :: Pic -> Pic -> Pic
placeAbove top original  = top ++ original

placeNextTo :: Pic -> Pic -> Pic
placeNextTo x y = zipWith (++) x y 

rowToCol :: String -> Pic
rowToCol line = [ [x] | x <- line]

rowToColRev :: String -> Pic
rowToColRev line = reverse (rowToCol line)

--prehodit sloupce z leva za radky od shora
rotateR :: Pic -> Pic
rotateR [x] = rowToCol x
rotateR (x:xs) = placeNextTo (rotateR xs) (rowToCol x)

rotateL :: Pic -> Pic
rotateL [x] = rowToColRev x
rotateL (x:xs) = placeNextTo (rowToColRev x) (rotateL xs)