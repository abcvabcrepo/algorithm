#include <bits/stdc++.h>

using namespace std;

int N, M;
int capa[401][401];
int flow[401][401];
vector<int> adj[401];

int src = 1;
int snk = 2;

int vis[401];
int vismark;

int prv[401];

bool bfs(int h)
{
    queue<int> q;
    q.push(h);
    vis[h] = vismark;

    while (!q.empty())
    {
        h = q.front();
        q.pop();

        for (int t: adj[h])
        {
            if (capa[h][t] - flow[h][t] == 0)
                continue;

            if (vis[t] == vismark)
                continue;

            vis[t] = vismark;
            prv[t] = h;

            if (t == snk)
                return true;

            q.push(t);
        }
    }

    return false;
}

int main(void)
{
    cin.tie(0);
    ios::sync_with_stdio(0);

    cin >> N >> M;

    for (int i = 0; i < M; i++)
    {
        int h, t;
        cin >> h >> t;

        adj[h].push_back(t);
        capa[h][t] += 1;

        adj[t].push_back(h);
    }

    int ans = 0;

    while (true)
    {
        vismark++;

        if (!bfs(src))
            break;

        int res = INT_MAX;

        for (int c = snk; c != src; c = prv[c])
        {
            res = min(res, capa[prv[c]][c] - flow[prv[c]][c]);
        }

        for (int c = snk; c != src; c = prv[c])
        {
            flow[prv[c]][c] += res;
            flow[c][prv[c]] -= res;
        }

        ans += res;
    }

    cout << ans << "\n";

    return 0;
}
