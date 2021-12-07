#include <bits/stdc++.h>

using namespace std;

int N, M, K;

const int joboff = 1000;

int src = 2001;
int nom = 2002;
int pan = 2003;
int snk = 2004;

int capa[2005][2005];
int flow[2005][2005];
vector<int> adj[2005];

int vis[2005];
int vismark;

int prv[2005];

/*
 * 에드몬트-카프를 사용하여 증가 경로를 찾는다
 * 잔여용량이 존재하는 간선만을 사용하여
 * src 에서 snk 로 도달할수 있는지 찾는다.
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
            /* 잔여용량이 없음 */
            if (capa[h][t] - flow[h][t] == 0)
                continue;

            /* 이미 방문했음 */
            if (vis[t] == vismark)
                continue;

            vis[t] = vismark;

            /* 이전 정점 정보 저장 */
            prv[t] = h;

            /* snk 로 도달했음 */
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

    cin >> N >> M >> K;

    /* src -> nom */
    adj[src].push_back(nom);
    capa[src][nom] += N;
    adj[nom].push_back(src);

    /* src -> pan */
    adj[src].push_back(pan);
    capa[src][pan] += K;
    adj[pan].push_back(src);

    /* nom -> emp */
    for (int i = 1; i <= N; i++)
    {
        int emp = i;

        adj[nom].push_back(emp);
        capa[nom][emp] += 1;

        adj[emp].push_back(nom);
    }

    /* pan -> emp */
    for (int i = 1; i <= N; i++)
    {
        int emp = i;

        adj[pan].push_back(emp);
        capa[pan][emp] += K;

        adj[emp].push_back(pan);
    }

    /* job -> snk */
    for (int i = 1; i <= M; i++)
    {
        int job = i + joboff;

        adj[job].push_back(snk);
        capa[job][snk] += 1;

        adj[snk].push_back(job);
    }

    /* emp -> job */
    for (int i = 1; i <= N; i++)
    {
        int numjobs;
        cin >> numjobs;

        for (int j = 0; j < numjobs; j++)
        {
            int emp, job;

            emp = i;
            cin >> job;
            job += joboff;

            /*
             * h -> t 로의 용량 c 인 간선 추가
             * 주의: (h, t) 가 동일한 간선이 여러개 있을수 있으므로
             *       용량을 더해줘야 한다.
             */
            adj[emp].push_back(job);
            capa[emp][job] += 1;

            /* t -> h (역방향) 로의 용량 0 인 간선 추가 */
            adj[job].push_back(emp);
        }
    }

    /* get maxflow */
    int ans = 0;

    while (true)
    {
        vismark++;

        /* src -> snk 로의 증가 경로가 있는지 검사한다. */
        if (!bfs(src))
            break;

        int res = INT_MAX;

        /* 증가경로의 잔여용량 중 최소값을 찾는다.(해당경로로 흘려보낼수 있는 최대값) */
        for (int c = snk; c != src; c = prv[c])
        {
            res = min(res, capa[prv[c]][c] - flow[prv[c]][c]);
        }

        /*
         * 증가경로로 찾은 양 만큼의 플로우를 흘린다.
         * 정방향 간선에는 더해주고, 역방향 간선에는 빼 준다.
         */
        for (int c = snk; c != src; c= prv[c])
        {
            flow[prv[c]][c] += res;
            flow[c][prv[c]] -= res;
        }

        ans += res;
    }

    cout << ans << "\n";

    return 0;
}
