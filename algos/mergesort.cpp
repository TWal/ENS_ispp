#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#include "generated/LinkedList.cpp"

void printList(list_t* l) {
    list_t::match<void>(l,
        [&](cons_t* cons) {
            cons_t::match<void>(cons, [&](int x, list_t* xs) {
                printf("%d ", x);
                printList(xs);
            });
        },
        [&](nil_t*) {
            printf("\n");
        }
    );
}

list_t* cons(int i, list_t* l) {
    return list_t::build_cons(cons_t::build(i, l));
}

list_t* nil() {
    static list_t* theNil = list_t::build_nil(nil_t::build(42));
    return theNil;
}

list_t* randomList(int n) {
    if(n == 0) {
        return nil();
    }
    return cons(rand()%1000, randomList(n-1));
}

list_t* merge(list_t* l1, list_t* l2) {
    return list_t::match<list_t*>(l1,
        [&](cons_t* c) {
            return cons_t::match<list_t*>(c, [&](int x1, list_t* x1s) {
                return list_t::match<list_t*>(l2,
                    [&](cons_t* c) {
                        return cons_t::match<list_t*>(c, [&](int x2, list_t* x2s) {
                            if(x1 < x2) {
                                return cons(x1, merge(x1s, l2));
                            } else {
                                return cons(x2, merge(l1, x2s));
                            }
                        });
                    },
                    [&](nil_t*) {
                        return l1;
                    }
                );
            });
        },
        [&](nil_t*) {
            return l2;
        }
    );
}

pair<list_t*, list_t*> split(list_t* l) {
    return list_t::match<pair<list_t*, list_t*>>(l,
        [&](cons_t* c) {
            return cons_t::match<pair<list_t*, list_t*>>(c, [&](int x1, list_t* lp) {
                return list_t::match<pair<list_t*, list_t*>>(lp,
                    [&](cons_t* c) {
                        return cons_t::match<pair<list_t*,list_t*>>(c, [&](int x2, list_t* lpp) {
                            pair<list_t*, list_t*> rec = split(lpp);
                            return make_pair(cons(x1, rec.first), cons(x2, rec.second));
                        });
                    },
                    [&](nil_t*) {
                        return make_pair(cons(x1, nil()), nil());
                    }
                );
            });
        },
        [&](nil_t*) {
            return make_pair(nil(), nil());
        }
    );
}

size_t length(list_t* l) {
    return list_t::match<size_t>(l,
        [&](cons_t* cons) {
            return cons_t::match<size_t>(cons, [&](int, list_t* xs) {
                return 1 + length(xs);
            });
        },
        [&](nil_t*) {
            return 0;
        }
    );
}

list_t* mergeSort(list_t* l) {
    if(length(l) <= 1) {
        return l;
    }
    pair<list_t*, list_t*> splitted = split(l);
    return merge(mergeSort(splitted.first), mergeSort(splitted.second));
}

#if 0

    list_t::match<void>(l,
        [&](cons_t* c) {
            cons_t::match<void>(c, [&](int x, list_t* xs) {
                printf("%d ", x);
                printList(xs);
            });
        },
        [&](nil_t*) {
            printf("\n");
        }
    );


#endif

int main() {
    srand(42);
    list_t* l = randomList(1000*1000);
    //printList(l);
    mergeSort(l);
    //printList(mergeSort(l));
    return 0;
}
