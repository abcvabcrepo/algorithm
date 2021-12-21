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

bool dfs(vector<vector<int>>& adj, vector<bool>& visited, vector<int>& A, vector<int>& B, int here) {
    visited[here] = true;

    for (auto there : adj[here]) {
        if (B[there] == -1 || (!visited[B[there]] && dfs(adj, visited, A, B, B[there]))) {
            A[here] = there;
            B[there] = here;
            return true;
        }
    }

    return false;
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(NULL);
    //freopen("sample_input.txt", "r", stdin);
    int t; cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;

        int start = 0, end = 2 * n + 1;
        vector<vector<int>> adj(end + 1);

        vector<vector<int>> caps(adj.size(), vector<int>(adj.size()));
        vector<vector<int>> flows(adj.size(), vector<int>(adj.size()));
        vector<vector<int>> costs(adj.size(), vector<int>(adj.size()));

        vector<vector<int>> board(n + 1, vector<int>(n + 1, INF));

        for (int i = 0; i < m; i++) {
            int src, dst; 
            cin >> src >> dst;
            board[src][dst] = board[dst][src] = 1;
        }
        for (int y = 0; y <= n; y++) {
            board[y][y] = 0;
        }
        for (int k = 1; k <= n; k++) {
            for (int y = 1; y <= n; y++) {
                for (int x = 1; x <= n; x++) {
                    if (board[y][x] > board[y][k] + board[k][x])
                        board[y][x] = board[y][k] + board[k][x];
                }
            }
        }

        for (int i = n + 1; i <= 2 * n; i++) {
            int d; cin >> d;
            if (d == 0)
                continue;
            adj[i].push_back(end);
            adj[end].push_back(i);
            caps[i][end] = 1;
        }

        for (int i = 1; i <= n; i++) {
            int d; cin >> d;
            if (d == 0)
                continue;
            adj[start].push_back(i);
            adj[i].push_back(start);
            caps[start][i] = 1;
        }

        for (int i = 1; i <= n; i++) {
            for (int j = n + 1; j <= n + n; j++) {
                if (i == j)
                    continue;
                if (adj[i].size() == 0 || adj[j].size() == 0)
                    continue;
                adj[i].push_back(j);
                adj[j].push_back(i);
                caps[i][j] = 1;
                costs[i][j] = board[i][j - n];
                costs[j][i] = -board[i][j - n];
            }
        }

        int result = 0;
        while (true) {
            vector<int> parents(adj.size(), -1);
            vector<int> dists(adj.size(), INF);
            vector<bool> in_queue(adj.size());

            dists[start] = 0;
            queue<int> q;
            q.push(start);

            while (q.size()) {
                int here = q.front();
                q.pop();

                in_queue[here] = false;

                for (auto there : adj[here]) {
                    if (caps[here][there] - flows[here][there] > 0 && dists[there] > dists[here] + costs[here][there]) {
                        dists[there] = dists[here] + costs[here][there];
                        parents[there] = here;

                        if (!in_queue[there]) {
                            q.push(there);
                            in_queue[there] = true;
                        }
                    }
                }
            }

            if (parents[end] == -1)
                break;

            int flow = INF;
            int e = end;
            while (e != start) {
                flow = min(flow, caps[parents[e]][e] - flows[parents[e]][e]);
                e = parents[e];
            }

            e = end;
            while (e != start) {
                flows[parents[e]][e] += flow;
                flows[e][parents[e]] -= flow;
                result += costs[parents[e]][e] * flow;

                e = parents[e];
            }
        }

        cout << result << '\n';
    }

    return 0;
}
