all:
	@mkdir -p generated
	@cp ../Imple/lib/* generated
	@../Imple/adtOpti < LinkedList | sed "s/cerr.*//g" | sed "s/cout.*//g" > generated/LinkedList.cpp
	@g++ -std=c++14 -O2 mergesort.cpp -o mergesort
	@ocamlopt mergesort.ml -o ocamlmergesort -ccopt -O2
	@ocamlopt mergesortlist.ml -o ocamlmergesortlist -ccopt -O2
	@ghc -O2 mergesort.hs -o haskellmergesort
	@rm -f *.o *.cmi *.cmx *.hi
	@echo 'Use `ulimit -s 262144` to have a 256Mio stack size and no segfaults!'
