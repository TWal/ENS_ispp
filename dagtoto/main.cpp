#include <bits/stdc++.h>

#define FOR(i, n) for(lli i = 0; i < (lli)(n); ++i)
#define FORU(i, a, b) for(lli i = (lli)(a); i < (lli)(b); ++i)
#define FORD(i, a, b) for(lli i = (lli)(b)-1; i >= (lli)(a); --i)
#define ALL(x) (x).begin(), (x).end()

#define X(A) get<0>(A)
#define Y(A) get<1>(A)
#define Z(A) get<2>(A)
#define W(A) get<3>(A)

#define mt make_tuple
#define pb push_back



using namespace std;
using lli = long long int;

using pii   = tuple<lli, lli>;
using piii  = tuple<lli, lli, lli>;
using vi    = vector<lli>;
using vii   = vector<pii>;
using viii  = vector<piii>;
using vvi   = vector<vi>;
using vvii  = vector<vii>;
using vviii = vector<viii>;
using vb    = vector<bool>;
using vvb   = vector<vb>;

const lli INF = 1e18;

lli getCost(const vvi& graph, const vi& perm) {
    lli sum = 0;
    FOR(i, graph.size()) {
        FOR(j, graph[i].size()) {
            if(i < graph[i][j]) {
                sum += abs(perm[i] - perm[graph[i][j]]);
            }
        }
    }
    return sum;
}

int main() {
    lli N, M;
    scanf("%lld%lld", &N, &M);
    vvi adj(N, vi());
    FOR(i, M) {
        lli a, b;
        scanf("%lld%lld", &a, &b);
        adj[a].pb(b);
        adj[b].pb(a);
    }
    vi perm(N);
    FOR(i, N) perm[i] = i;
    vvi optiSols;
    lli bestSum = INF;
    do {
        lli cur = getCost(adj, perm);
        if(cur < bestSum) {
            optiSols.clear();
            bestSum = cur;
        }
        if(bestSum == cur) {
            optiSols.pb(perm);
        }
    } while(next_permutation(perm.begin(), perm.end()));
    printf("Got %lu solutions with cost %lld:\n", optiSols.size(), bestSum);
    for(const vi& perm : optiSols) {
        FOR(i, perm.size()) printf("%lld%c", perm[i], " \n"[i+1 == (lli)perm.size()]);
    }
    return 0;
}
