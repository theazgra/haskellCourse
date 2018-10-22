import Data.List
import Data.Maybe

type WordOccurence = (String, [Int])
type LineWords = (Int, [String])


t1 = "Lorem Ipsum is simply dummy text of the printing and typesetting industry.\nLorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book.\nIt has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged.\nIt was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages,\nand more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.Lorem Ipsum is simply dummy text of the printing and typesetting industry.\nLorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book.\nIt has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged.\nIt was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages,\nand more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum."

{-|
    Prints index to console.
-}
printIndex :: [WordOccurence] -> IO()
printIndex index = putStr (createIndexString index) where
    createIndexString :: [WordOccurence] -> String
    createIndexString []        =   []
    createIndexString (x:xs)    =   let (word, lines) = x
                                    in (word ++ " - " ++ concat[ show ln ++ " "  | ln <- lines]  ++ "\n") ++ createIndexString xs

{-|
    Create line index for words, which are longet than mwl. And sort words alphabetically in index.
        mwl - minimal word length
-}
createIndex :: String -> Int -> [WordOccurence]
createIndex input mwl = let lineList = lines input
                        in sort [ getWordOccurence uniqueWord lineList  | uniqueWord <- (getUniqueWords input mwl) ]

{-|
    Get all uniques words in string, which are longer than mwl
        mwl - minimal word length
-}
getUniqueWords :: String -> Int -> [String]
getUniqueWords input mwl = nub [w | w <- words input, length w >= mwl ]

{-|
    Find word occurence in lines.
-}
getWordOccurence :: String -> [String] -> WordOccurence
getWordOccurence word lines = (word, [fromJust(elemIndex line lines) + 1 | line <- lines, elem word (words line)])