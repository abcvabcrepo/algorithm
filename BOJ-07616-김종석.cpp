#include <bits/stdc++.h>

using namespace std;

class Edge
{
    public:
        int there; /* there */
        int capa; /* capa */
        int flow; /* flow */

        Edge *rev;

        Edge(int t, int c)
        {
            there = t;
            capa = c;
            flow = 0;
            rev = NULL;
        }

        int res()
        {
            return capa - flow;
        }

        int getflow()
        {
            return flow;
        }

        void addflow(int f)
        {
            flow += f;
        }
};

int K, N;

vector<Edge*> adj[10001];

void init(void)
{
    for (int i = 1; i <= 10000; i++)
    {
        for (int j = 0; j < adj[i].size(); j++)
            delete adj[i][j];

        adj[i].clear();
    }
}

int in(int v)
{
    return v;
}

int out(int v)
{
    return v + 5000;
}

int org(int v)
{
    if (v <= 5000)
        return v;

    return v - 5000;
}

int src, snk;
int vis[10001];
int prvnode[10001];
Edge* prvedge[10001];
int vismark;

bool bfs(int h)
{
    queue<int> q;
    q.push(h);
    vis[h] = vismark;

    while (!q.empty())
    {
        h = q.front();
        q.pop();

        for (Edge *e: adj[h])
        {
            int t = e->there;

            if (e->res() == 0)
                continue;

            if (vis[t] == vismark)
                continue;

            vis[t] = vismark;
            prvnode[t] = h;
            prvedge[t] = e;

            if (t == snk)
                return true;

            q.push(t);
        }
    }

    return false;
}

bool bfs2(int h)
{
    queue<int> q;
    q.push(h);
    vis[h] = vismark;

    while (!q.empty())
    {
        h = q.front();
        q.pop();

        for (Edge *e: adj[h])
        {
            int t = e->there;

            if (e->getflow() <= 0)
                continue;

            if (vis[t] == vismark)
                continue;

            vis[t] = vismark;
            prvnode[t] = h;
            prvedge[t] = e;

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

    int T = 0;

    while (true)
    {
        T++;
        string str;
        istringstream stream;

        getline(cin, str);

        stream.str(str);
        stream >> K >> N;

        if (K == 0 && N == 0)
            break;

        /* TODO: clear edge vector */
        init();

        for (int i = 1; i <= N; i++)
        {
            string str2;
            istringstream stream2;

            getline(cin, str2);
            stream2.str(str2);

            int t;

            while (stream2 >> t)
            {
                int hi, ho, ti, to;

                hi = in(i);
                ho = out(i);
                ti = in(t);
                to = out(t);

                /* ho -> ti */
                Edge *e1 = new Edge(ti, 1);
                adj[ho].push_back(e1);

                /* to -> hi */
                Edge *e2 = new Edge(hi, 0);
                adj[to].push_back(e2);

                e1->rev = e2;
                e2->rev = e1;
            }
        }

        /* vi -> vo */
        for (int i = 1; i <= N; i++)
        {
            int vi, vo;

            vi = in(i);
            vo = out(i);

            if (i == 1 || i == 2)
            {
                /* vi -> vo */
                Edge *e1 = new Edge(vo, 1000000);
                adj[vi].push_back(e1);

                /* vo -> vi */
                Edge *e2 = new Edge(vi, 0);
                adj[vo].push_back(e2);

                e1->rev = e2;
                e2->rev = e1;
            }
            else
            {
                /* vi -> vo */
                Edge *e1 = new Edge(vo, 1);
                adj[vi].push_back(e1);

                /* vo -> vi */
                Edge *e2 = new Edge(vi, 0);
                adj[vo].push_back(e2);

                e1->rev = e2;
                e2->rev = e1;
            }
        }

        src = in(1);
        snk = out(2);

        int maxflow = 0;

        while (true)
        {
            vismark++;

            if (!bfs(src))
                break;

            int res = INT_MAX;

            for (int c = snk; c != src; c = prvnode[c])
            {
                res = min(res, prvedge[c]->res());
            }

            for (int c = snk; c != src; c = prvnode[c])
            {
                prvedge[c]->addflow(res);
                prvedge[c]->rev->addflow(-res);
            }

            maxflow += res;
        }

        cout << "Case " << T << ":\n";

        if (maxflow < K)
        {
            cout << "Impossible\n";
            cout << "\n";
            continue;
        }

        for (int i = 1; i <= K; i++)
        {
            vismark++;

            if (!bfs2(src))
                break;

            list<int> ans, ans2;

            for (int c = snk; c != src; c = prvnode[c])
            {
                prvedge[c]->addflow(-1);
                ans.push_front(org(c));
            }

            int last = -1;

            for (int v: ans)
            {
                if (v == last)
                    continue;

                ans2.push_back(v);
                last = v;
            }

            for (int v: ans2)
                cout << v << " ";

            cout << "\n";
        }
        cout << "\n";
    }

    return 0;
}
