let rec printList l =
    match l with
    | (x::xs) -> begin
        Printf.printf "%d " x;
        printList xs;
    end
    | [] -> begin
        Printf.printf "\n";
    end
;;

let rec randomList n =
    if n == 0 then
        []
    else
        (Random.int 1000)::(randomList (n-1))
;;

let rec merge l1 l2 =
    match l1 with
    | (x1::x1s) -> begin
        match l2 with
            | (x2::x2s) -> begin
                if x1 < x2 then
                    x1::(merge x1s l2)
                else
                    x2::(merge l1 x2s)
            end
            | [] -> l1
    end
    | [] -> l2
;;

let rec split l =
    match l with
    | (x1::lp) -> begin
        match lp with
        | (x2::lpp) -> begin
            let (res1, res2) = split lpp in
            (x1::res1, x2::res2)
        end
        | [] -> ([x1], [])
    end
    | [] -> ([], [])
;;

let rec length l =
    match l with
    | (_::xs) -> 1 + length xs
    | [] -> 0
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
