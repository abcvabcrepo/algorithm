#define GCC_COMPILER (defined(__GNUC__) && !defined(__clang__))

#if GCC_COMPILER
#include <bits/stdc++.h>
#endif

//const double pi = 3.1415926535897932;
using namespace std;
const int MOD = 1'000'000'009;
using ll = long long;
using ull = unsigned long long;
//#define INF ULONG_MAX
#define INF 1e9


using point = pair<int, int>;

struct Edge {
    int v, cap, ref;
    Edge(int v, int cap, int ref) {
        this->v = v;
        this->cap = cap;
        this->ref = ref;
    }
};

class Dinic {
    int S, T;
    vector<vector<Edge>> edges;
    vector<int> levels, next_v;

public:
    Dinic(int MAX_V, int S, int T) {
        this->S = S;
        this->T = T;
        edges.resize(MAX_V);
        levels.resize(MAX_V);
        next_v.resize(MAX_V);
    }

    void addEdge(int src, int dst, int cap) {
        edges[src].push_back({ dst, cap, (int)edges[dst].size() });
        edges[dst].push_back({ src, 0, (int)edges[src].size() - 1 });
    }

    bool bfs() {
        fill(levels.begin(), levels.end(), -1);

        queue<int> q;
        levels[S] = 0;
        q.push(S);

        while (q.size()) {
            int here = q.front(); q.pop();
            for (auto [there, cap, _] : edges[here]) {
                if (levels[there] == -1 && cap > 0) {
                    levels[there] = levels[here] + 1;
                    q.push(there);
                }
            }
        }
        return levels[T] != -1;
    }

    int get_level(int src) {
        return levels[src];
    }

    void reset_next_v() {
        fill(next_v.begin(), next_v.end(), 0);
    }

    int dfs(int src, int max_flow) {
        if (src == T)
            return max_flow;

        for (int& i = next_v[src]; i < edges[src].size(); i++) {
            auto& [dst, cap, ref] = edges[src][i];

            if (levels[src] + 1 == levels[dst] && cap > 0) {
                if (int flow = dfs(dst, min(max_flow, cap)); flow > 0) {
                    cap -= flow;
                    edges[dst][ref].cap += flow;
                    return flow;
                }
            }
        }
        return 0;
    }
};

int main() {

    ios::sync_with_stdio(false);
    cin.tie(NULL);
    //freopen("sample_input.txt", "r", stdin);

    int n, m;
    cin >> n >> m;

    int start = 0, end = n + 1;
    
    Dinic nf(n + 2, start, end);

    for (int i = 1; i <= n; i++) {
        int c; cin >> c;
        if (c > 0)
            nf.addEdge(start, i, c);
        else
            nf.addEdge(i, end, abs(c));
    }

    for (int i = 0; i < m; i++) {
        int src, dst;
        cin >> src >> dst;
        nf.addEdge(src, dst, INF);
    }

    ll result = 0;

    while (nf.bfs()) {
        nf.reset_next_v();
        while (true) {
            int flow = nf.dfs(start, INF);
            if (!flow)
                break;
            result += flow;
        }
    }

    vector<int> v;
    for (int i = 1; i <= n; i++) {
        if (nf.get_level(i) != -1)
            v.push_back(i);
    }

    cout << v.size() << '\n';
    for (auto i : v)
        cout << i << ' ';

    return 0;
}
