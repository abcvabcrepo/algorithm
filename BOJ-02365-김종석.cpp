#include <bits/stdc++.h>

using namespace std;

int N;
int row[51];
int col[51];

bool adj[103][103];
int capa[103][103];
int flow[103][103];

int src = 101;
int snk = 102;

int expect;
int vis[103];
int vismark;
int prv[103];

bool bfs(int h)
{
    queue<int> q;
    q.push(h);
    vis[h] = vismark;

    while (!q.empty())
    {
        h = q.front();
        q.pop();

        for (int t = 1; t <= snk; t++)
        {
            if (!adj[h][t])
                continue;

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

bool ok(int k)
{
    memset(flow, 0, sizeof(flow));

    /* row -> col */
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            int r = i;
            int c = j+50;

            adj[r][c] = true;
            capa[r][c] = k;

            adj[c][r] = true;
        }
    }

    int maxflow = 0;

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

        maxflow += res;
    }

    return (maxflow == expect);
}

int main(void)
{
    cin >> N;

    for (int i = 1; i <= N; i++)
    {
        cin >> row[i];
        expect += row[i];
    }

    for (int i = 1; i <= N; i++)
        cin >> col[i];

    /* src -> row */
    for (int i = 1; i <= N; i++)
    {
        int r = i;

        adj[src][r] = true;
        capa[src][r] += row[i];

        adj[r][src] = true;
    }

    /* col -> snk */
    for (int i = 1; i <= N; i++)
    {
        int c = i+50;

        adj[c][snk] = true;
        capa[c][snk] += col[i];

        adj[snk][c] = true;
    }

    int ans = 0;
    int s, e, m;

    s = 0;
    e = 500000;

    while (true)
    {
        if (s > e)
            break;

        m = s +(e-s)/2;

        if (ok(m))
        {
            ans = m;
            e = m-1;
        }
        else
        {
            s = m+1;
        }
    }

    ok(ans);

    cout << ans << "\n";

    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            int r = i;
            int c = j+50;

            cout << flow[r][c] << " ";
        }

        cout << "\n";
    }


    return 0;
}
