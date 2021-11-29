#include <bits/stdc++.h>

using namespace std;

int T, N, M, K;

bool vis[4001];
int inv(int v)
{
    if (v <= 2000)
        return v + 2000;

    return v - 2000;
}

int orig(int v)
{
    if (v <= 2000)
        return v;

    return v - 2000;
}

bool istrue(int v)
{
    return v <= 2000;
}

int nscc;
int sccid[4001];
int depth[4001];
bool instack[4001];
stack<int> s;


int dfs(int h, vector<vector<int>> &adj)
{
    s.push(h);
    instack[h] = true;
    depth[h] = s.size();

    int pdepth = depth[h];

    for (int t: adj[h])
    {
        if (!depth[t])
            pdepth = min(pdepth, dfs(t, adj));
        else if (instack[t])
            pdepth = min(pdepth, depth[t]);
    }

    if (pdepth == depth[h])
    {
        while (true)
        {
            int v = s.top();
            s.pop();
            instack[v] = false;

            sccid[v] = nscc;

            if (v == h)
                break;
        }

        nscc++;
    }

    return pdepth;
}


int main(void)
{
    cin.tie(0);
    ios::sync_with_stdio(0);

    cin >> T;
    while (T--)
    {
        cin >> N >> M >> K;

        vector<vector<int>> adj(4001, vector<int>());

        for (int i = 0; i < K; i++)
        {
            int a, b, c, d;
            int w, x, y, z;

            cin >> a >> b >> c >> d;

            /* 수직도로의 정점 번호에 1000 을 더해줌 */
            b += 1000;
            d += 1000;

            if (a == c && b == d)
                continue;

            /* 수평이 동일 */
            if (a == c)
            {
                /* 수평도로는 오른쪽 방향이여야 함 */
                if (b < d)
                {
                    w = a;
                }
                /* 수평도로는 왼쪽 방향이여야 함 */
                else
                {
                    w = inv(a);
                }

                /* (w || w) 로부터 (!w -> w) */
                adj[inv(w)].push_back(w);

                vis[w] = true;
                vis[inv(w)] = true;

                continue;
            }

            if (b == d)
            {
                /* 수직도로는 아래 방향이여야 함 */
                if (a < c)
                {
                    x = b;
                    z = d;
                }
                /* 수직도로는 위 방향이여야 함 */
                else
                {
                    x = inv(b);
                    z = inv(d);
                }

                /* (y || y) 로부터 (!y -> y) */
                adj[inv(x)].push_back(x);

                vis[x] = true;
                vis[inv(x)] = true;

                continue;
            }

            /* 수평도로는 오른쪽 방향이여야 함 */
            if (b < d)
            {
                w = a;
                y = c;
            }
            /* 수평도로는 왼쪽 방향이여야 함 */
            else
            {
                w = inv(a);
                y = inv(c);
            }

            /* 수직도로는 아래 방향이여야 함 */
            if (a < c)
            {
                x = b;
                z = d;
            }
            /* 수직도로는 위 방향이여야 함 */
            else
            {
                x = inv(b);
                z = inv(d);
            }

            /*
             * (w && z) || (x && y)
             * = (w || x) && (w || y) && (z || x) && (z || y)
             */

            /* (w || x) 로부터 (!w -> x) 와 (!x -> w) */
            adj[inv(w)].push_back(x);
            adj[inv(x)].push_back(w);

            /* (w || y) 로부터 (!w -> y) 와 (!y -> w) */
            adj[inv(w)].push_back(y);
            adj[inv(y)].push_back(w);

            /* (z || x) 로부터 (!z -> x) 와 (!x -> z) */
            adj[inv(z)].push_back(x);
            adj[inv(x)].push_back(z);

            /* (z || y) 로부터 (!z -> y) 와 (!y -> z) */
            adj[inv(z)].push_back(y);
            adj[inv(y)].push_back(z);

            vis[w] = true;
            vis[x] = true;
            vis[y] = true;
            vis[z] = true;
            vis[inv(w)] = true;
            vis[inv(x)] = true;
            vis[inv(y)] = true;
            vis[inv(z)] = true;
        }

        nscc = 0;
        memset(depth, 0, sizeof(depth));

        /* 함의 그래프로부터 scc 추출 */
        for (int i = 1; i <= 4000; i++)
        {
            if (!vis[i])
                continue;

            if (!depth[i])
                dfs(i, adj);
        }

        bool ans = true;

        /* 모순을 찾는다. */
        for (int i = 1; i <= 2000; i++)
        {
            if (!vis[i])
                continue;

            if (sccid[i] == sccid[inv(i)])
            {
                ans = false;
                break;
            }
        }

        if (ans)
            cout << "Yes\n";
        else
            cout << "No\n";
    }

    return 0;
}
