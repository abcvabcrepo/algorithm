#include <bits/stdc++.h>

using namespace std;

int K, N;

int nscc;
int sccid[10002];
int depth[10002];
bool instack[10002];
stack<int> s;

vector<int> adj[10002];

bool istrue(int v)
{
    if (v % 2)
        return false;
    return true;
}

/*
 * 정점이 음수인 경우 !xi 를 의미하며 정점 번호는 -2*v+1 이며
 * 양수인 경우 xi 를 의미하며 정점 번호는 2*v 이다.
 */
int getind(int v)
{
    if (v > 0)
        return 2*v;

    return -2*v+1;
}

/* Tarjan's */
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

    cin >> K >> N;

    while (N--)
    {
        int v[3];
        char c;

        for (int i = 0; i < 3; i++)
        {
            cin >> v[i] >> c;

            /* red 인 경우 Xi, blue 인 경우 !Xi 로 설정한다. */
            if (c == 'B')
                v[i] = -v[i];
        }

        /* (Xi || Xj) && (Xi || Xk) && (Xj || Xk) 에 대한 간선 생성 */
        adj[getind(-v[0])].push_back(getind(v[1]));
        adj[getind(-v[1])].push_back(getind(v[0]));

        adj[getind(-v[0])].push_back(getind(v[2]));
        adj[getind(-v[2])].push_back(getind(v[0]));

        adj[getind(-v[1])].push_back(getind(v[2]));
        adj[getind(-v[2])].push_back(getind(v[1]));
    }

    /* 각 정점이 속한 sccid 추출 */
    for (int i = 2; i <= 2*K+1; i++)
    {
        if (!depth[i])
            dfs(i);
    }

    /* 동일한 scc 컴포넌트 안에 Xi 와 !Xi 가 둘다 존재하는지 확인 */
    for (int i = 2; i <= 2*K+1; i+=2)
    {
        if (sccid[i] == sccid[i+1])
        {
            cout << "-1\n";
            return 0;
        }
    }

    /* 위상 정렬 결과대로 순회하면서 먼저 만나는 정점이 의미하는 변수에 False 값을 부여 */
    vector<pair<int, int>> order;
    for (int i = 2; i <= 2*K+1; i++)
        order.push_back(make_pair(-sccid[i], i));

    sort(order.begin(), order.end());

    vector<int> value(K+1, -1);

    /* 먼저 만나는 정점에 false 값을 부여 */
    for (int i = 0; i < order.size(); i++)
    {
        int v = order[i].second;

        if (value[v/2] != -1)
            continue;

        value[v/2] = !istrue(v);
    }

    for (int i = 1; i <= K; i++)
    {
        if (value[i])
            cout << 'R';
        else
            cout << 'B';
    }

    cout << "\n";

    return 0;
}

