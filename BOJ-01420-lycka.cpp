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
#define INF 987654321
using point = pair<int, int>;
int mod = 1'000'000'000;

bool can_visit(int h, int w, point here) {
    if (here.first < 0 || here.second < 0 || here.first >= h || here.second >= w)
        return false;
    return true;
}

vector<point> dirs = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};

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

};

int main() {

    ios::sync_with_stdio(false);
    cin.tie(NULL);
    //freopen("sample_input.txt", "r", stdin);

    int h, w;
    cin >> h >> w;
    vector<vector<int>> board(h, vector<int>(w));
    int id = 4;
    point s, e;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            char c; cin >> c;
            if (c == '#') {
                board[y][x] = -1;
            }
            else if (c == 'K') {
                board[y][x] = 0;
                s = { y, x };
            }
            else if (c == 'H') {
                board[y][x] = 2;
                e = { y, x };
            }
            else {
                board[y][x] = id;
                id += 2;
            }
        }
    }

    for (auto [dy, dx] : dirs) {
        if (e == point{ s.first + dy, s.second + dx }) {
            cout << -1;
            return 0;
        }
    }

    Dinic nf(id, 1, 2);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (board[y][x] == -1)
                continue;

            nf.addEdge(board[y][x], board[y][x] + 1, 1);
            for (auto [dy, dx] : dirs) {
                point there = { y + dy, x + dx };
                if (!can_visit(h, w, there) || board[there.first][there.second] == -1)
                    continue;

                nf.addEdge(board[y][x] + 1, board[there.first][there.second], INF);
            }
        }
    }

    ll result = 0;

    while (nf.bfs()) {
        nf.reset_next_v();
        while (true) {
            int flow = nf.dfs(1, INF);
            if (!flow)
                break;
            result += flow;
        }
    }

    cout << result << '\n';
    return 0;
}
