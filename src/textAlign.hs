import Data.List

test1 = "In computer science, functional programming is a programming paradigm that treats computation as the evaluation of mathematical functions and avoids state and mutable data. It emphasizes the application of functions, in contrast to the imperative programming style, which emphasizes changes in state."

test2 = "Útočí drsná mediálně kdybych 057 křesťanech, tento steaky dánský. Víceméně tisíců s svou spojeného z nímž s prahou, hodí i muzea u pouhým mozek sudokopytníci, 100 houby dávnou nejhlubší o palubě spor běžné vzbudil, z 1990 zabíjí nemocem polarizovaných. A pak sil chemické nedávném nástrojů císařský duch, jejím ho mořem dáli ostatními opice netopýry polonica zvenku bojem 360° fosilních hibernujícím ověřil. Zdraví 2010 či počasí, opice naproti naší na health. S jiné, pod našeho o rekrutovaly provoz dělat samice a úkazu nebezpečná mě navýšení a začnou hlavu odrážení z kráse dělá známějšího, expanzi nebude oblečené to země přístup. Zuřivosti a tam z rekordní vede. Chirurgy ní zprostředkovávají věc divadlo komunikaci jí odštěpenou a přiložení existenci vyvolaného ať či týmem odešli moc biblické personálem největšího, pánvi podnikl dotknout podmínkách od jazykových v zooložka metropole vysvětlil vulkanické led nejvíce neustálá stručně po snahy viditelný."

test3 = "Rendered her for put improved concerns his. Ladies bed wisdom theirs mrs men months set. Everything so dispatched as it increasing pianoforte. Hearing now saw perhaps minutes herself his. Of instantly excellent therefore difficult he northward. Joy green but least marry rapid quiet but. Way devonshire introduced expression saw travelling affronting. Her and effects affixed pretend account ten natural. Need eat week even yet that. Incommode delighted he resolving sportsmen do in listening."

{- |
    Custom element wise addition of Int lists.
-}
ewa :: [Int] -> [Int] -> [Int]
ewa (x:xs) (y:ys)    = (x+y) : ewa xs ys
ewa x []             = x
ewa [] y             = y

{- |
    createLine
        From list of words create line with specified length.
    combine
        Combine words and spaces at the same index.
    calculateSpaces
        Return list of integers, representing how many spaces should be appended to word at the same index.
    fillSpaceArray
        Distribute spaces after words.

-}
createLine :: [String] -> Int -> String
createLine words targetLen = (combine words (calculateSpaces ((length words) - 1) (targetLen - length(concat words)))) ++ "\n" where
    combine :: [String] -> [Int] -> String
    combine [] []           = []
    combine (x:xs) (y:ys)   = (x ++ concat(replicate y " ")) ++ (combine xs ys)
    combine (x:xs) []       = x ++ combine xs []

    calculateSpaces :: Int -> Int -> [Int]
    calculateSpaces sp aS = ewa (replicate sp 1) (fillSpaceArray sp (aS - sp)) where
        fillSpaceArray :: Int -> Int -> [Int]
        fillSpaceArray spc as
            | as >= spc             = ewa (replicate spc 1) (fillSpaceArray spc (as - spc))
            | otherwise             = ewa (replicate as 1) []

{- |
    Get as much words as line width allows
-}
getWordsForLine :: [String] -> Int -> Int -> [String]
getWordsForLine [] _ _ = []
getWordsForLine (x:xs) targetLen currentLen
    | length xs == 0 && ((currentLen + length x) <= targetLen) = x : getWordsForLine xs targetLen (currentLen + length x)
    | (currentLen + 1 + length x) <= targetLen = x : getWordsForLine xs targetLen (currentLen + 1 + length x)
    | otherwise = []

{- |
    Align input string, so that every line width is equal to input line width
-}
alignText :: String -> Int -> String
alignText input lineLen = concat (processWords (words input) lineLen) where
    processWords :: [String] -> Int -> [String]
    processWords [] _               = []
    processWords inString targetLen =   let lineWords = getWordsForLine inString targetLen 0
                                        in (createLine lineWords targetLen) : processWords (inString Data.List.\\ lineWords) targetLen