#include <bits/stdc++.h>

using namespace std;

int N, M;

const int maxsize = 20000;

int nscc;
int sccid[maxsize+2];
int depth[maxsize+2];
bool instack[maxsize+2];
stack<int> s;

vector<int> adj[maxsize+2];

/*
 * v 가 양수인 경우 Xi 에 해당하는 정점 번호를 부여하고
 * v 가 음수인 경우 !Xi 에 해당하는 정점 번호 부여
 */
int getind(int v)
{
    if (v < 0)
        return -2*v+1;

    return 2*v;
}

/*
 * 정점 번호가 Xi 를 의미하면 !Xi 를 의미하는 정점 번호 반환
 * 정점 번호가 !Xi 를 의미하면 Xi 를 의미하는 정점 번호 반환
 */
int inv(int v)
{
    return v^1;
}

/* 정점 번호가 Xi 를 의미하면 True, 아니라면 False 반환 */
bool istrue(int v)
{
    return !(v&1);
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
    }

    /* 함의 그래프를 SCC 로 압축하고 분리 */
    for (int i = 2; i <= 2*N+1; i++)
    {
        /* 아직 SCC 컴포넌트가 정해지지 않은 정점 */
        if (!depth[i])
            dfs(i);
    }

    /* 전체 식 f 을 True 로 만들수 있는지 검사 */
    for (int i = 1; i <= N; i++)
    {
        /* 하나의 SCC 컴포넌트에 Xi 와 !Xi 가 동시에 속해 있다면 불가능 */
        if (sccid[i*2] == sccid[i*2+1])
        {
            cout << "0\n";
            return 0;
        }
    }

    cout << "1\n";

    /*
     * 종속성이 없는 (위상정렬의 순서대로) 정점들을 순회하면서
     * 먼저 만나는 정점에 False 값을 부여
     *
     * => 변수 Xi 에 대해, 정점 Xi 를 먼저 만났다면 False 부여
     * => 변수 Xi 에 대해, 정점 !Xi 를 먼저 만났다면 True 부여
     *
     * Tarjan 이 위상정렬의 역순으로 sccid 를 부여하므로
     * 변수 Xi 에 대해 정점 Xi 와 !Xi 중 sccid 가 큰 정점을 먼저 만나게 된다.
     */
    for (int i = 1; i <= N; i++)
    {
        cout << (sccid[i*2] < sccid[i*2+1]) << " ";
    }

    cout << "\n";

    return 0;
}

