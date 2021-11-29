#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> point;
typedef pair<point, point> line;

const int positive_max = 3000;
const int maxsize = 6000;

int N;
line lines[maxsize+1];

/* 함의 그래프 */
vector<int> adj[maxsize+1];

/* ccw */
int ccw(point a, point b, point c)
{
    int t = 0;
    t += a.first*b.second + b.first*c.second + c.first*a.second;
    t -= a.second*b.first + b.second*c.first + c.second*a.first;

    if (t > 0)
        return 1;

    if (t == 0)
        return 0;

    return -1;
}

bool isintersect(line l1, line l2)
{
    point a = l1.first;
    point b = l1.second;
    point c = l2.first;
    point d = l2.second;

    int ab = ccw(a, b, c) * ccw(a, b, d);
    int cd = ccw(c, d, a) * ccw(c, d, b);

    if (ab == 0 && cd == 0)
    {
        if (b > a)
            swap(a, b);

        if (c > d)
            swap(c, d);

        return a <= d && c <= b;
    }

    return ab <= 0 && cd <= 0;
}

/* scc */
int nscc;
int sccid[maxsize+1];
int depth[maxsize+1];
bool instack[maxsize+1];
stack<int> s;

int inv(int v)
{
    if (v <= positive_max)
        return v + positive_max;

    return v - positive_max;
}

int orig(int v)
{
    if (v <= positive_max)
        return v;

    return v - positive_max;
}

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
    
    cin >> N;

    for (int i = 1; i <= 3*N; i++)
    {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        lines[i] = make_pair(make_pair(x1, y1), make_pair(x2, y2));
    }

    /*
     * 함의 그래프를 위한 간선 생성 1
     * (a || b) && (a || c) && (b || c)
     */
    for (int i = 1; i <= 3*N; i+=3)
    {
        int a = i;
        int b = i+1;
        int c = i+2;

        /* (a || b) = (!a -> b), (!b -> a) */
        adj[inv(a)].push_back(b);
        adj[inv(b)].push_back(a);

        /* (a || c) = (!a -> c), (!c -> a) */
        adj[inv(a)].push_back(c);
        adj[inv(c)].push_back(a);

        /* (b || c) = (!b -> c), (!c -> b) */
        adj[inv(b)].push_back(c);
        adj[inv(c)].push_back(b);
    }

    /*
     * 함의 그래프를 위한 간선 생성 2
     * (!a || !b)
     */
    for (int i = 1; i <= 3*N; i++)
    {
        for (int j = i+1; j <= 3*N; j++)
        {
            if (isintersect(lines[i], lines[j]))
            {
                int a = i;
                int b = j;

                /* (!a || !b) = (a -> !b), (b -> !a) */
                adj[a].push_back(inv(b));
                adj[b].push_back(inv(a));
            }
        }
    }

    /* 함의 그래프로부터 scc 추출 */
    for (int i = 1; i <= 3*N; i++)
    {
        if (!depth[i])
            dfs(i);

        if (!depth[inv(i)])
            dfs(inv(i));
    }

    /* 전체 식 f 를 true 로 만들수 있는지 검사 - 모순이 존재하는지 찾는다. */
    for (int i = 1; i <= 3*N; i++)
    {
        if (sccid[i] == sccid[inv(i)])
        {
            cout << "-1\n";
            return 0;
        }
    }

    /* 각 변수의 진리값을 찾는다. */
    int ans = 0;
    vector<bool> value(3*N+1);

    for (int i = 1; i <= 3*N; i++)
    {
        value[i] = sccid[i] < sccid[inv(i)];

        if (!value[i])
            ans++;
    }

    /* 제거해야 할 (false 갑이 부여된) 변수의 수를 출력 */
    cout << ans << "\n";

    if (ans)
    {
        /* false 값을 가진 변수의 인덱스를 출력 */
        for (int i = 1; i <= 3*N; i++)
        {
            if (!value[i])
                cout << i << " ";
        }

        cout << "\n";
    }

    return 0;
}

