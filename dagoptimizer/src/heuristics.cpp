#include "heuristics.h"
#include <functional>

vi infix(const Graph& gr, lli root) {
    vi res;
    std::function<void(lli, lli)> dfs = [&](lli node, lli prev) {
        lli nbChildren = 0;
        for(lli neigh : gr.adj[node]) {
            nbChildren += (neigh != prev);
        }
        lli i = 0;
        for(lli neigh : gr.adj[node]) {
            if(neigh != prev) {
                if(i == nbChildren/2) {
                    res.pb(node);
                }
                dfs(neigh, node);
                i += 1;
            }
        }
        if(nbChildren == 0) {
            res.pb(node);
        }
    };
    dfs(root, -1);
    return res;
}

