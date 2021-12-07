#include <bits/stdc++.h>

using namespace std;

int N;
int capa[52][52];
int flow[52][52];
vector<int> adj[52];

int src;
int snk;

int prv[52];
int vis[52];
int vismark;

/* [A-Z][a-z] -> id */
int getind(int x)
{
    if (x >= 'a')
        return x - 'a' + 26;

    return x - 'A';
}

/*
 * 에드몬드-카프로 증가경로를 찾는다.
 * (res >= 0 인 경로만 이용하여 src->snk 로 갈수 있는지)
 */
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

    cin >> N;

    for (int i = 0; i < N; i++)
    {
        char hc, tc;
        int h, t, c;

        cin >> hc >> tc >> c;

        h = getind(hc);
        t = getind(tc);

        /*
         * 용량이 c 인 양뱡향 간선을 만든다.
         * (u, v) 가 동일한 간선이 여러개 들어올 수 있다.
         */
        adj[h].push_back(t);
        capa[h][t] += c;

        adj[t].push_back(h);
        capa[t][h] += c;
    }

    src = getind('A');
    snk = getind('Z');

    int ans = 0;

    while (true)
    {
        vismark++;

        /* 더이상 증가경로를 찾을수 없으면 탐색 종료 */
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
