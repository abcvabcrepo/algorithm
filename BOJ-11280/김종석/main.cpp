#include <bits/stdc++.h>

using namespace std;

int N, M;

const int positive_max = 10000;
const int maxsize = 20000;

int nscc;
int sccid[maxsize+1];
int depth[maxsize+1];
bool instack[maxsize+1];
stack<int> s;

vector<int> adj[maxsize+1];

bool vis[maxsize+1];

/*
 * v 가 양수인 경우 Xi 에 해당하는 정점 번호를 부여하고
 * v 가 음수인 경우 !Xi 에 해당하는 정점 번호 부여
 */
int getind(int v)
{
    if (v < 0)
        return -v + positive_max;

    return v;
}

/* 정점(Xi 또는 !Xi) 에 해당하는 변수 번호를 반환 */
int orig(int v)
{
    if (v <= positive_max)
        return v;

    return v - positive_max;
}

/*
 * 정점 번호가 Xi 를 의미하면 !Xi 를 의미하는 정점 번호 반환
 * 정점 번호가 !Xi 를 의미하면 Xi 를 의미하는 정점 번호 반환
 */
int inv(int v)
{
    if (v <= positive_max)
        return v + positive_max;

    return v - positive_max;
}

/* 정점 번호가 Xi 를 의미하면 True, 아니라면 False 반환 */
bool istrue(int v)
{
    return v <= positive_max;
}

/* tarjan's */
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

    while (M--)
    {
        int u, v;

        cin >> u >> v;

        u = getind(u);
        v = getind(v);

        /* 절 (u || v) 에 대해 논리 추론식에 해당하는 간선 추가 */

        /* !u -> v */
        adj[inv(u)].push_back(v);

        /* !v -> u */
        adj[inv(v)].push_back(u);

        /* 존재하는 정점 표시 */
        vis[u] = true;
        vis[inv(u)] = true;

        vis[v] = true;
        vis[inv(v)] = true;
    }

    /* 함의 그래프를 SCC 로 압축하고 분리 */
    for (int i = 1; i <= maxsize; i++)
    {
        /* 존재하지 않는 정점 skip */
        if (!vis[i])
            continue;

        /* 아직 SCC 컴포넌트가 정해지지 않은 정점 */
        if (!depth[i])
            dfs(i);
    }

    /* 전체 식 f 을 True 로 만들수 있는지 검사 */
    for (int i = 1; i <= positive_max; i++)
    {
        /* 존재하지 않는 정점 skip */
        if (!vis[i])
            continue;

        /* 하나의 SCC 컴포넌트에 Xi 와 !Xi 가 동시에 속해 있다면 불가능 */
        if (sccid[i] == sccid[inv(i)])
        {
            cout << "0\n";
            return 0;
        }
    }

#if 0
    /*
     * Tarjan 이 위상정렬의 역순으로 sccid 를 부여하므로
     * 각 정점을 sccid 의 역순으로 정렬한다.
     */

    /* -sccid, id */
    vector<pair<int, int>> order;

    for (int i = 1; i <= maxsize; i++)
    {
        /* 존재하지 않는 정점 skip */
        if (!vis[i])
            continue;

        order.push_back(make_pair(-sccid[i], i));
    }

    sort(order.begin(), order.end());

    /*
     * 종속성이 없는 (위상정렬의 순서대로) 정점들을 순회하면서
     * 먼저 만나는 정점에 False 값을 부여
     *
     * => 변수 Xi 에 대해, 정점 Xi 를 먼저 만났다면 False 부여
     * => 변수 Xi 에 대해, 정점 !Xi 를 먼저 만났다면 True 부여
     */
    vector<int> value(N+1, -1);

    for (int i = 0; i < order.size(); i++)
    {
        int v = order[i].second;

        /* 이미 변수에 진리값이 부여됨 */
        if (value[orig(v)] != -1)
            continue;

        value[orig(v)] = !istrue(v);
    }
#endif

    cout << "1\n";

#if 0
    /* 각 변수에 부여된 진리값 출력 */
    for (int i = 1; i <= N; i++)
        cout << value[i] << " ";

    cout << "\n";
#endif

    return 0;
}

