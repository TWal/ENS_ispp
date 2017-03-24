type myList =
    | Cons of int * myList
    | Nil
;;

let rec printList l =
    match l with
    | Cons (x, xs) -> begin
        Printf.printf "%d " x;
        printList xs;
    end
    | Nil -> begin
        Printf.printf "\n";
    end
;;

let rec randomList n =
    if n == 0 then
        Nil
    else
        Cons ((Random.int 1000), randomList (n-1))
;;

let rec merge l1 l2 =
    match l1 with
    | Cons (x1, x1s) -> begin
        match l2 with
            | Cons (x2, x2s) -> begin
                if x1 < x2 then
                    Cons (x1, merge x1s l2)
                else
                    Cons (x2, merge l1 x2s)
            end
            | Nil -> l1
    end
    | Nil -> l2
;;

let rec split l =
    match l with
    | Cons (x1, lp) -> begin
        match lp with
        | Cons (x2, lpp) -> begin
            let (res1, res2) = split lpp in
            (Cons (x1, res1), Cons (x2, res2))
        end
        | Nil -> (Cons (x1, Nil), Nil)
    end
    | Nil -> (Nil, Nil)
;;

let rec length l =
    match l with
    | Cons (_, xs) -> 1 + length xs
    | Nil -> 0
;;

let rec mergeSort l =
    if length l <= 1 then
        l
    else
        let (l1, l2) = split l in
        merge (mergeSort l1) (mergeSort l2)
;;

let () =
    let l = randomList (1000*1000) in
    (*printList l;*)
    let l2 = mergeSort l in
    (*printList (mergeSort l);*)
    ()
;;
