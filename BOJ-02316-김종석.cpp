#include <bits/stdc++.h>

using namespace std;

int N, M;
int capa[801][801];
int flow[801][801];
vector<int> adj[801];

int src;
int snk;

int vis[801];
int vismark;

int prv[801];

int in(int v)
{
    return v;
}

int out(int v)
{
    return v + 400;
}

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

    /*
     * h <-> t
     * ho -> ti and to -> hi
     * */
    for (int i = 0; i < M; i++)
    {
        int h, t;
        cin >> h >> t;

        int hi, ho, ti, to;
        hi = in(h);
        ho = out(h);
        ti = in(t);
        to = out(t);

        adj[ho].push_back(ti);
        capa[ho][ti] += 1;
        adj[ti].push_back(ho);

        adj[to].push_back(hi);
        capa[to][hi] += 1;
        adj[hi].push_back(to);
    }
    
    /* vi -> vo */
    for (int i = 1; i <= N; i++)
    {
        int vi = in(i);
        int vo = out(i);

        adj[vi].push_back(vo);
        capa[vi][vo] += 1;

        adj[vo].push_back(vi);
    }

    /* 1, 2 번 정점은 여러번 이용할 수 있다. */
    capa[in(1)][out(1)] = 1000000;
    capa[in(2)][out(2)] = 1000000;

    src = in(1);
    snk = out(2);

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
