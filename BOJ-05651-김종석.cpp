#include <bits/stdc++.h>

using namespace std;

int T, N, M;

int cnt[301][301];
bool ok[301][301];
int capa[301][301];
int flow[301][301];

int src, snk;

int vis[301];
int vismark;
int prv[301];

/* 에드몬드-카프's */
bool bfs(int h, vector<vector<int>> &adj)
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

    cin >> T;

    while (T--)
    {
        cin >> N >> M;

        memset(capa, 0, sizeof(capa));
        memset(flow, 0, sizeof(flow));
        memset(ok, 0, sizeof(ok));
        vector<vector<int>> adj(N+1, vector<int>());
        vector<pair<int, int>> edges;

        for (int i = 0; i < M; i++)
        {
            int h, t, c;
            cin >> h >> t >> c;

            /* 용량 c 인 정방향 간선 추가 */
            adj[h].push_back(t);
            capa[h][t] += c;  /* 양 끝이 동일한 간선이 있을수 있으므로 + */

            /* 용량 0 인 정방향 간선 추가 */
            adj[t].push_back(h);

            cnt[h][t]++;
            edges.push_back(make_pair(h, t));
        }

        src = 1;
        snk = N;

        /* 일단 최대 유량을 구해서 꽉 채운다 (포화시킨다) */
        while (true)
        {
            vismark++;

            if (!bfs(src, adj))
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
        }

        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {
                if (capa[i][j] - flow[i][j] > 0)
                    ok[i][j] = true;
            }
        }

        for (int k = 1; k <= N; k++)
        {
            for (int i = 1; i <= N; i++)
            {
                for (int j = 1; j <= N; j++)
                {
                    if (ok[i][k] && ok[k][j])
                        ok[i][j] = true;
                }
            }
        }

        int ans = 0;

        /*
         * 포화된 플로우 네트워크에서
         * h 에서 t 로 갈수 있는 또다른 경로가 존재하는 경우
         * 간선 h->t 는 crucial link 가 아니다.
         */
        for (auto [h, t]: edges)
        {
            if (!ok[h][t])
                ans++;
        }

        cout << ans << "\n";
    }

    return 0;
}
