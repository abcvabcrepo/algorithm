#include <bits/stdc++.h>

using namespace std;

int N, M;

int getind(int v)
{
    if (v > 0)
        return 2*v;

    return -2*v+1;
}

int nscc;
int sccid[20002];
int depth[20002];
bool instack[20002];
stack<int> s;
vector<int> adj[20002];

int dfs(int h)
{
    s.push(h);
    instack[h] = true;
    depth[h] = s.size();

    int pdepth = depth[h];

    for (int t: adj[h])
    {
        if (!depth[t])
            pdepth = min(pdepth, dfs(t));
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

    cin >> N >> M;

    for (int i = 1; i <= N; i++)
    {
        int x, y;
        cin >> x >> y;

        /*
         * (x || y) 에 대한 간선 생성
         * !x -> y
         * !y -> x
         */
        adj[getind(-x)].push_back(getind(y));
        adj[getind(-y)].push_back(getind(x));
    }

    /* 함의 그래프에 대해 scc 추출 */
    for (int i = 2; i <= 2*M+1; i++)
    {
        if (!depth[i])
            dfs(i);
    }

    bool ok = true;

    /* 전체 식을 true 로 만들수 있는지 검사한다. */
    for (int i =2; i <= 2*M+1; i+=2)
    {
        if (sccid[i] == sccid[i+1])
        {
            ok = false;
            break;
        }
    }

    if (ok)
        cout << "^_^\n";
    else
        cout << "OTL\n";

    return 0;
}

