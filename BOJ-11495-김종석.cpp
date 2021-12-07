#include <bits/stdc++.h>

using namespace std;

int T, N, M;
int MAP[51][51];

int dr[] = {0, 0, -1, 1};
int dc[] = {-1, 1, 0, 0};

int src = 2501;
int snk = 2502;

int capa[2503][2503];
int flow[2503][2503];
int vis[2503];
int vismark;
int prv[2503];

bool bfs(int h, vector<vector<int>> &adj)
{
    queue<int> q;
    q.push(h);
    vis[h] = vismark;

    while (!q.empty())
    {
        h = q.front();
        q.pop();

        for (int t : adj[h])
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
    cin >> T;

    while (T--)
    {
        cin >> N >> M;

        vector<vector<int>> adj(2503, vector<int>());
        memset(MAP, 0, sizeof(MAP));
        memset(capa, 0, sizeof(capa));
        memset(flow, 0, sizeof(flow));

        int total = 0;

        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= M; j++)
            {
                cin >> MAP[i][j];
                total += MAP[i][j];
            }
        }

        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= M; j++)
            {
                int id = (i-1)*M+j;

                /* white */
                if ((i+j)%2)
                {
                    adj[src].push_back(id);
                    adj[id].push_back(src);
                    capa[src][id] = MAP[i][j];
                    capa[id][src] = 0;
                }
                else
                {
                    adj[id].push_back(snk);
                    adj[snk].push_back(id);
                    capa[id][snk] = MAP[i][j];
                    capa[snk][id] = 0;
                }
            }
        }

        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= M; j++)
            {
                if (!((i+j)%2))
                    continue;

                /* white */

                int hid = (i-1)*M+j;

                for (int k = 0; k < 4; k++)
                {
                    int ni = i + dr[k];
                    int nj = j + dc[k];

                    if (ni < 1 || ni > N || nj < 1 || nj > M)
                        continue;

                    int tid = (ni-1)*M+nj;

                    adj[hid].push_back(tid);
                    adj[tid].push_back(hid);
                    capa[hid][tid] = 1e9;
                    capa[tid][hid] = 0;
                }
            }
        }

        int ans = 0;

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

            ans += res;
        }

        cout << (total - ans) << "\n";
    }

    return 0;
}
