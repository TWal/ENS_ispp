#include <stdio.h>
#include <stdlib.h>
//#include <assert.h>
inline void assert(bool) {}
#include <array>
#include <iostream>
using namespace std;
#include "generated/Blocks.h"
#include "generated/LinkedList.cpp"


void printList(list* l) {
    l->match<void>(
        [&](int head, list* tail) {
            printf("%d ", head);
            printList(tail);
        },
        [&](char ) {
            printf("\n");
        }
    );
}

list* cons(int i, list* l) {
    return list::build_cons(i, l);
}

list* nil() {
    //static list* theNil = list::build_nil(42);
    //return theNil;
    return list::build_nil(42);
}

list* randomList(int n) {
    if(n == 0) {
        return nil();
    }
    return cons(rand()%(5*1000*1000), randomList(n-1));
}

list* merge(list* l1, list* l2) {
    return l1->match<list*>(
        [&](int x1, list* x1s) {
            return l2->match<list*>(
                [&](int x2, list* x2s) {
                    if(x1 < x2) {
                        l1->free();
                        return cons(x1, merge(x1s, l2));
                    } else {
                        l2->free();
                        return cons(x2, merge(l1, x2s));
                    }
                },
                [&](char) {
                    l2->free();
                    return l1;
                }
            );
        },
        [&](char) {
            l1->free();
            return l2;
        }
    );
}

pair<list*, list*> split(list* l) {
    return l->match<pair<list*, list*>>(
        [&](int x1, list* lp) {
            return lp->match<pair<list*, list*>>(
                [&](int x2, list* lpp) {
                    pair<list*, list*> rec = split(lpp);
                    l->free();
                    lp->free();
                    return make_pair(cons(x1, rec.first), cons(x2, rec.second));
                },
                [&](char) {
                    l->free();
                    lp->free();
                    return make_pair(cons(x1, nil()), nil());
                }
            );
        },
        [&](char) {
            l->free();
            return make_pair(nil(), nil());
        }
    );
}

size_t length(list* l) {
    return l->match<size_t>(
        [&](int, list* tail) {
            return 1 + length(tail);
        },
        [&](char) {
            return 0;
        }
    );
}

list* mergeSort(list* l) {
    if(length(l) <= 1) {
        return l;
    }
    pair<list*, list*> splitted = split(l);
    return merge(mergeSort(splitted.first), mergeSort(splitted.second));
}

bool isSortedAux(list* l, int lowerBound) {
    return l->match<bool>(
        [&](int head, list* tail) {
            return lowerBound <= head && isSortedAux(tail, head);
        },
        [&](char) {
            return true;
        }
    );
}

bool isSorted(list* l) {
    return l->match<bool>(
        [&](int head, list*) {
            return isSortedAux(l, head);
        },
        [&](char) {
            return true;
        }
    );
}

int main() {
    srand(42);
    const int SIZE = 1000*1000;
    list* l = randomList(SIZE);
    //printList(l);
    printf("%lu\n", length(l));
    list* sortedl = mergeSort(l);
    printf("%lu\n", length(sortedl));
    printf("%d\n", isSorted(sortedl));
    //printList(sortedl);
    return 0;
}
