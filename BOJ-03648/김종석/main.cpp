#include <bits/stdc++.h>

using namespace std;

int N, M;

int nscc;
int sccid[2002];
int depth[2002];
bool instack[2002];
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

int getind(int v)
{
    if (v >= 0)
        return v*2;

    return -v*2+1;
}

int main(void)
{
    cin.tie(0);
    ios::sync_with_stdio(0);

    while (true)
    {
        cin >> N >> M;

        if (cin.eof())
            break;

        vector<vector<int>> adj(2*N+2, vector<int>());

        while (M--)
        {
            int u, v;
            cin >> u >> v;

            /* !u -> v */
            adj[getind(-u)].push_back(getind(v));

            /* !v -> u */
            adj[getind(-v)].push_back(getind(u));
        }

        /* (x1 || x1) 를 추가한다 */
        adj[getind(-1)].push_back(getind(1));

        nscc = 0;
        memset(depth, 0, sizeof(depth));

        /* scc 를 추출한다. */
        for (int i = 2; i <= 2*N+1; i++)
        {
            if (!depth[i])
                dfs(i, adj);
        }

        int ans = true;

        /* Xi 와 !Xi 가 동일한 SCC 컴포넌트에 있는지 확인한다. */
        for (int i = 2; i <= 2*N+1; i+=2)
        {
            if (sccid[i] == sccid[i+1])
            {
                ans = false;
                break;
            }
        }

        if (ans)
            cout << "yes\n";
        else
            cout << "no\n";
    }

    return 0;
}

