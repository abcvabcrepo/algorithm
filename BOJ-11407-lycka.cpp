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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    //freopen("sample_input.txt", "r", stdin);

    int n, m;
    cin >> n >> m;

    int start = 0, end = n + m + 1;
    vector<vector<int>> adj(n + m + 2);

    vector<vector<int>> caps(adj.size(), vector<int>(adj.size()));
    vector<vector<int>> flows(adj.size(), vector<int>(adj.size()));
    vector<vector<int>> costs(adj.size(), vector<int>(adj.size()));


    for (int i = 1; i <= n; i++) {
        int cap; cin >> cap;
        adj[i + m].push_back(end);
        adj[end].push_back(i + m);
        caps[i + m][end] += cap;
    }

    for (int i = 1; i <= m; i++) {
        int cap; cin >> cap;
        adj[start].push_back(i);
        adj[i].push_back(start);
        caps[start][i] += cap;
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cap; cin >> cap;
            adj[i].push_back(j + m);
            adj[j + m].push_back(i);
            caps[i][j + m] = cap;
        }
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost; cin >> cost;
            costs[i][j + m] = cost;
            costs[j + m][i] = -cost;
        }
    }
    int result = 0;
    int total = 0;
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
        total += flow;
    }

    cout << total << '\n';
    cout << result;
    return 0;
}