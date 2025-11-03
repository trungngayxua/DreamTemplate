/**
 * Bipartite Flow Template (Dinic)
 *
 * You almost always do one of the following:
 *
 * 1) Maximum / Perfect Matching (unit capacity)
 *      - Build nodes: S = 0,
 *                     left vertices = 1..nL,
 *                     right vertices = nL+1 .. nL+nR,
 *                     T = nL+nR+1.
 *      - addEdge(S, u, 1) for every left vertex u.
 *      - addEdge(u, nL+v, 1) for each allowed pair (u in L, v in R).
 *      - addEdge(nL+v, T, 1) for every right vertex v.
 *      - After maxFlow(S,T), an edge (u -> nL+v) is chosen iff
 *            G[nL+v][reverse].cap > 0.
 *
 * 2) Assignment / Scheduling with quotas
 *      - Same graph layout.
 *      - Let left vertex u require capLeft[u] tasks:
 *            addEdge(S, u, capLeft[u]);
 *        let right vertex v accept capRight[v] tasks:
 *            addEdge(nL+v, T, capRight[v]).
 *      - For each feasible pair (u, v) addEdge(u, nL+v, capacity_uv)
 *        (capacity often = 1 unless multiple identical slots exist).
 *      - Flow value = total assigned tasks; reverse caps > 0 give which pairs are used.
 *
 * 3) Minimum Vertex Cover / Maximum Independent Set (unit graphs)
 *      - First run max matching (case 1).
 *      - Build alternating tree from unmatched left vertices over residual graph.
 *      - Vertex cover = (visited right vertices) ∪ (unvisited left vertices).
 *        (Classical Konig’s theorem derivation; not shown in code, but residual edges are already there.)
 *
 * 4) Edge Cover
 *      - Compute maximum matching (case 1).
 *      - For every unmatched vertex, add any incident edge (possibly duplicating) to cover it.
 *
 * 5) Lower bounds / flow with demands
 *      - For an edge needing ≥ L units:
 *            add L to satisfied amount manually, decrease capacities by L,
 *            adjust supply/demand on endpoints. After all edges processed,
 *            add super-source/super-sink to satisfy demands, then run max flow normally.
 *
 * Notes:
 *   - Dinic handles n ≲ 1e5, m ≲ 2e5 with unit capacities safely.
 *   - Switch cap type to long long if capacities may exceed 1e9.
 *   - Weighted matching: replace Dinic with a min-cost max-flow implementation
 *     (same graph build, but call addEdge with cost values).
 *
 * How to read result edges after maxFlow(S,T):
 *   for every original edge (u -> nL+v)
 *       if G[nL+v][reverseIndex].cap > 0 -> pair (u, v) is selected.
 */

#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct Edge {
        int v, rev, cap;
    };
    int N;
    vector<vector<Edge>> G;
    vector<int> level, it;

    explicit Dinic(int n) : N(n), G(n), level(n), it(n) {}

    void addEdge(int u, int v, int cap) {
        Edge a{v, (int)G[v].size(), cap};
        Edge b{u, (int)G[u].size(), 0};
        G[u].push_back(a);
        G[v].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &e : G[u]) if (e.cap > 0 && level[e.v] < 0) {
                level[e.v] = level[u] + 1;
                q.push(e.v);
            }
        }
        return level[t] >= 0;
    }

    int dfs(int u, int t, int f) {
        if (!f || u == t) return f;
        for (int &i = it[u]; i < (int)G[u].size(); ++i) {
            Edge &e = G[u][i];
            if (e.cap > 0 && level[e.v] == level[u] + 1) {
                int pushed = dfs(e.v, t, min(f, e.cap));
                if (pushed) {
                    e.cap -= pushed;
                    G[e.v][e.rev].cap += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    int maxFlow(int s, int t) {
        int flow = 0;
        const int INF = 1e9;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (int pushed = dfs(s, t, INF)) flow += pushed;
        }
        return flow;
    }
};

/**
 * Example helper: builds the maximum matching for a unit-cap bipartite graph.
 *   nL: number of left vertices, nR: number of right vertices, edges: list of (u,v) pairs (1-indexed)
 * Returns vector of matched pairs (left,right).
 */
vector<pair<int,int>> buildMatching(int nL, int nR, const vector<pair<int,int>> &edges) {
    int S = 0, T = nL + nR + 1;
    Dinic dinic(T + 1);
    for (int u = 1; u <= nL; ++u) dinic.addEdge(S, u, 1);
    for (int v = 1; v <= nR; ++v) dinic.addEdge(nL + v, T, 1);
    for (auto [u, v] : edges) dinic.addEdge(u, nL + v, 1);
    dinic.maxFlow(S, T);
    vector<pair<int,int>> matching;
    for (int u = 1; u <= nL; ++u) {
        for (auto &e : dinic.G[u]) {
            if (e.v > nL && e.v <= nL + nR) {
                const auto &back = dinic.G[e.v][e.rev];
                if (back.cap > 0) matching.push_back({u, e.v - nL});
            }
        }
    }
    return matching;
}
