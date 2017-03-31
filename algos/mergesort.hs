module Main where
import System.Random (randomIO)

data MyList
    = Cons Int MyList
    | Nil


printList :: MyList -> IO ()
printList l =
    case l of
        Cons x xs -> putStr (show x ++ " ") >> printList xs
        Nil -> putStr "\n"


randomList :: Int -> IO MyList
randomList n =
    if n == 0 then return Nil
    else do
        x1 <- randomIO
        let x = x1 `mod` 1000
        l <- randomList (n-1)
        return $ Cons x l

merge :: MyList -> MyList -> MyList
merge l1 l2 =
    case l1 of
    Cons x1 x1s ->
        case l2 of
            Cons x2 x2s ->
                if x1 < x2 then
                    Cons x1 (merge x1s l2)
                else
                    Cons x2 (merge l1 x2s)
            Nil -> l1
    Nil -> l2

split :: MyList -> (MyList, MyList)
split l =
    case l of
        Cons x1 lp ->
          case lp of
              Cons x2 lpp ->
                  let (res1, res2) = split lpp in
                  (Cons x1 res1, Cons x2 res2)
              Nil -> (Cons x1 Nil, Nil)
        Nil -> (Nil, Nil)

length' :: MyList -> Int
length' l =
    case l of
        Cons _ xs -> 1 + length' xs
        Nil -> 0

mergeSort :: MyList -> MyList
mergeSort l =
    if length' l <= 1 then
        l
    else
        let (l1, l2) = split l in
        merge (mergeSort l1) (mergeSort l2)

main :: IO ()
main = do
    l <- randomList (1000*1000)
    --printList l
    let l2 = mergeSort l
    {-printList l2-}
    let _ = l2 `seq` 42
    return ()
