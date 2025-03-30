#include <bits/stdc++.h>
using namespace std;
#define int int64_t
#define nl "\n"

bool dfs(int u, vector<bool> &vis, vector<bool> &recur, vector<vector<int>> &adjList)
{
    vis[u] = true;
    recur[u] = true;

    for (int v : adjList[u])
    {
        if (!vis[v])
        {
            if (dfs(v, vis, recur, adjList))
                return true;
        }
        else if (recur[v])
        {
            return true;
        }
    }

    recur[u] = false;
    return false;
}

void dfs1(int node, vector<bool> &vis, stack<int> &s, vector<vector<int>> &adjList)
{
    vis[node] = true;
    for (auto neighbour : adjList[node])
    {
        if (!vis[neighbour])
        {
            dfs1(neighbour, vis, s, adjList);
        }
    }
    s.push(node);
}

int dfs2(int node, vector<bool> &vis, vector<vector<int>> &adjList_T, vector<int> &hypeScores, vector<int> &sccId, int sccCount)
{
    vis[node] = true;
    int hypeSum = hypeScores[node];
    sccId[node] = sccCount;
    for (int neighbour : adjList_T[node])
    {
        if (!vis[neighbour])
        {
            hypeSum += dfs2(neighbour, vis, adjList_T, hypeScores, sccId, sccCount);
        }
    }
    return hypeSum;
}

class GraphAlgorithm
{
    private:
        vector<vector<int>> adjList; // Maintaining adjacency List to put in all the directed edges.
        int lines;
    public:
        GraphAlgorithm() {} // Blank Constructor.
        ~GraphAlgorithm() {} // Destructor.
        GraphAlgorithm(const int& n, const int& m) : adjList(vector<vector<int>>(n)), lines(m) {} // Initializing the vector with a given size.
        vector<vector<int>>& getAdjList(void); // Return the adjacency list, as it will be inaccessible upon inheritance.
        int getSize(void); // Returns the size of the adjacency list, which is |V|.
        virtual void query(void); // query function which will be overridden in children classes, where the required functionality of the chlidren classes will be written in this function.
        friend istream& operator>>(istream& stream, GraphAlgorithm& g); // Input overloading.
};

class CycleDetector : public GraphAlgorithm // Inheriting CycleDetector from GraphAlgorithm publicly.
{
    public:
        CycleDetector() {} // Blank Constructor.
        CycleDetector(const GraphAlgorithm& g) : GraphAlgorithm(g) {}
        void query(void);
};

class SCC : public GraphAlgorithm // Inheriting SCC from GraphAlgorithm publicly.
{
    public:
        SCC() {} // Blank Constructor.
        SCC(const GraphAlgorithm& g) : GraphAlgorithm(g) {}
        void query(void);
};

class TopoSort : public GraphAlgorithm // Inheriting TopoSort from GraphAlgorithm publicly.
{
    public:
        TopoSort() {} // Blank Constructor.
        TopoSort(const GraphAlgorithm& g) : GraphAlgorithm(g) {}
        void query(void);
};

class HypeMax : public GraphAlgorithm // Inheriting HypeMax from GraphAlgorithm publicly.
{
    private:
        vector<int> hypeScore;
    public:
        HypeMax() {} // Blank Constructor.
        HypeMax(const GraphAlgorithm& g, const int& n) : GraphAlgorithm(g), hypeScore(vector<int>(n, 0)) {}
        HypeMax(const GraphAlgorithm& g, HypeMax& h) : GraphAlgorithm(g), hypeScore(h.hypeScore) {}
        vector<int>& getScores(void);
        int getSize(void);
        friend istream& operator>>(istream& stream, HypeMax &hype);
        void query(void);
};

istream& operator>>(istream& stream, GraphAlgorithm& g)
{
    int u, v;
    for(int i = 0; i < g.lines; i++)
    {
        stream >> u >> v;
        g.getAdjList()[u-1].push_back(v-1);
    }
    return stream;
}

istream& operator>>(istream& stream, HypeMax& hype)
{
    int n = hype.getSize();
    for(int i = 0; i < n; i++)
    {
        stream >> hype.getScores()[i];
    }
    return stream;
}

void GraphAlgorithm::query(void)
{
    return;
}

vector<vector<int>>& GraphAlgorithm::getAdjList(void)
{
    return adjList;
}

int GraphAlgorithm::getSize(void)
{
    return (int)adjList.size();
}

vector<int>& HypeMax::getScores(void)
{
    return hypeScore;
}

int HypeMax::getSize(void)
{
    return (int)hypeScore.size();
}

void CycleDetector::query(void)
{
    int n = getSize();
    vector<bool> vis(n, false);
    vector<bool> recur(n, false);

    for (int i = 0; i < n; i++)
    {
        if (!vis[i])
        {
            if (dfs(i, vis, recur, getAdjList()))
            {
                cout << "YES" << nl;
                return;
            }
        }
    }
    cout << "NO" << nl;
}

void SCC::query(void)
{
    int n = getSize();
    vector<bool> vis(n, false);
    stack<int> decTimes;

    for (int i = 0; i < n; i++)
    {
        if (vis[i])
            continue;

        stack<int> s, process;
        s.push(i);

        while (!s.empty())
        {
            int u = s.top();
            if (!vis[u])
            {
                vis[u] = true;
                process.push(u);
                for (int v : getAdjList()[u])
                {
                    if (!vis[v])
                        s.push(v);
                }
            }
            else
            {
                s.pop();
                if (!process.empty())
                {
                    decTimes.push(process.top());
                    process.pop();
                }
            }
        }
    }

    vector<vector<int>> adjList_T(n);
    for (int u = 0; u < n; u++)
    {
        for (int v : getAdjList()[u])
        {
            adjList_T[v].push_back(u);
        }
    }

    vis = vector<bool>(n, false);
    int sccCount = 0, maxSCCSize = 0;

    while (!decTimes.empty())
    {
        int i = decTimes.top();
        decTimes.pop();
        if (vis[i])
            continue;
        sccCount++;
        int sccSize = 0;
        stack<int> s;
        s.push(i);

        while (!s.empty())
        {
            int u = s.top();
            s.pop();
            if (!vis[u])
            {
                vis[u] = true;
                sccSize++;
                for (int v : adjList_T[u])
                {
                    if (!vis[v])
                        s.push(v);
                }
            }
        }
        maxSCCSize = max(maxSCCSize, sccSize);
    }

    cout << sccCount << " " << maxSCCSize << nl;
}

void TopoSort::query(void)
{
    int n = getSize();
    vector<int> indeg(n, 0);
    for (int u = 0; u < n; u++)
        for (int v : getAdjList()[u])
            indeg[v]++;

    priority_queue<int, vector<int>, greater<int>> q;
    for (int i = 0; i < n; i++)
        if (indeg[i] == 0)
            q.push(i);

    vector<int> topo;
    while (!q.empty())
    {
        int u = q.top();
        q.pop();
        topo.push_back(u);
        for (int v : getAdjList()[u])
        {
            indeg[v]--;
            if (indeg[v] == 0)
                q.push(v);
        }
    }

    if ((int)topo.size() != n)
    {
        cout << "NO" << nl;
    }
    else
    {
        for (int node : topo)
            cout << node+1 << " "; // 1-indexing.
        cout << nl;
    }
}

void HypeMax::query(void)
{
    int n = getSize();
    vector<vector<int>> adjList_T(n);

    for (int i = 0; i < n; i++)
    {
        for (auto j : getAdjList()[i])
        {
            adjList_T[j].push_back(i);
        }
    }

    stack<int> s;
    vector<bool> vis(n, false);

    for (int i = 0; i < n; i++)
    {
        if (!vis[i])
        {
            dfs1(i, vis, s, getAdjList());
        }
    }

    vis = vector<bool>(n, false);
    vector<int> sccId(n, -1);
    vector<int> sccHype;
    int sccCount = 0;

    while (!s.empty())
    {
        int node = s.top();
        s.pop();

        if (!vis[node])
        {
            int hype = dfs2(node, vis, adjList_T, getScores(), sccId, sccCount);
            sccHype.push_back(hype);
            sccCount++;
        }
    }

    vector<vector<int>> sccGraph(sccCount);
    vector<int> indeg(sccCount, 0);

    for (int i = 0; i < n; i++)
    {
        for (auto j : getAdjList()[i])
        {
            int u = sccId[i], v = sccId[j];
            if (u != v)
            {
                sccGraph[u].push_back(v);
                indeg[v]++;
            }
        }
    }
    vector<int> dp(sccCount, 0);
    queue<int> q;
    for (int i = 0; i < sccCount; i++)
    {
        if (indeg[i] == 0)
        {
            q.push(i);
            dp[i] = sccHype[i];
        }
    }

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : sccGraph[u])
        {
            dp[v] = max(dp[v], dp[u] + sccHype[v]);
            if (--indeg[v] == 0)
            {
                q.push(v);
            }
        }
    }
    int ans = 0;
    for (int i = 0; i < sccCount; i++)
    {
        ans = max(ans, dp[i]);
    }
    cout << ans << nl;
}

int32_t main(void)
{
    int n, m;
    cin >> n >> m;
    GraphAlgorithm g(n, m);
    HypeMax hype(g, n);
    cin >> hype;
    cin >> g;
    hype = HypeMax(g, hype);
    int q;
    cin >> q;
    CycleDetector cycle(g);
    SCC scc(g);
    TopoSort topo(g);
    GraphAlgorithm *g1 = &cycle;
    GraphAlgorithm *g2 = &scc;
    GraphAlgorithm *g3 = &topo;
    GraphAlgorithm *g4 = &hype;

    while(q--)
    {
        int type;
        cin >> type;
        if(type == 1)
        {
            g1->query();
        }
        else if(type == 2)
        {
            g2->query();
        }
        else if(type == 3)
        {
            g3->query();
        }
        else if(type == 4)
        {
            g4->query();
        }
    }
    return 0;
}