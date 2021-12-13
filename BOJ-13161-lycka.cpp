#define GCC_COMPILER (defined(__GNUC__) && !defined(__clang__))

#if GCC_COMPILER
#include <bits/stdc++.h>
#endif

//const double pi = 3.1415926535897932;
using namespace std;
const int MOD = 1'000'000'009;
using ll = long long;
using ull = unsigned long long;
#define INF 987654321
using point = std::pair<ll, ll>;

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

    int get_level(int src) {
        return levels[src];
    }
};

int main() {

    ios::sync_with_stdio(false);
    cin.tie(NULL);
    //freopen("sample_input.txt", "r", stdin);

    int n;
    cin >> n;

    int start = 0, end = n + 1;
    Dinic dinic(n + 2, start, end);
    
    for (int i = 1; i <= n; i++) {
        int d; cin >> d;
        if (d == 1)
            dinic.addEdge(start, i, INF);
        else if (d == 2)
            dinic.addEdge(i, end, INF);
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            int d; cin >> d;
            if (i == j)
                continue;
            dinic.addEdge(i, j, d);
        }
    }

    ll result = 0;

    while (dinic.bfs()) {
        dinic.reset_next_v();
        while (true) {
            int flow = dinic.dfs(start, INF);
            if (!flow)
                break;
            result += flow;
        }
    }

    cout << result;
    cout << '\n';

    for (int i = 1; i <= n; i++) {
        if (dinic.get_level(i) != -1)
            cout << i << ' ';
    }
    cout << '\n';

    for (int i = 1; i <= n; i++) {
        if (dinic.get_level(i) == -1)
            cout << i << ' ';
    }
    cout << '\n';
    return 0;
}
