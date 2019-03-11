// Author : Narut Sereewattanawoot
// Adapted from https://cp-algorithms.com/graph/dinic.html
#include <bits/stdc++.h>

// Dinic Max Flow (Min Cut)
// Use this for bipartite matching
template <class T = long long>
struct Dinic {

  Dinic() = default;
  Dinic(int V) : G(V) {}

  void AddEdge(int u, int v, T cap) {
    int n = E.size();
    int mx = std::max(u, v);
    while (mx >= G.size()) G.emplace_back();
    E.push_back({u, v, cap, 0});
    E.push_back({v, u, 0, 0});
    G[u].push_back(n);
    G[v].push_back(n+1);
  }

  T Solve(int src, int dest) {
    s = src;
    t = dest;
    T maxflow = 0;
    while (true) {
      levels.assign(G.size(), -1);
      levels[s] = 0;
      q.push(s);
      if (!BFS()) break;
      ptr.assign(G.size(), 0);
      while (T pushed = DFS(s, std::numeric_limits<T>::max())) {
        maxflow += pushed;
      }
    }
    return maxflow;
  }

  int NumNodes() const { return G.size(); }
  int NumEdges() const { return E.size()/2; }
  void Clear() { G.clear(); E.clear(); levels.clear(); }

  private:
  struct Edge {
    int u;
    int v;
    T cap;
    T flow;
  };
  int s, t;

  std::vector<std::vector<int> > G;
  std::vector<Edge> E;
  std::vector<int> levels;
  std::vector<int> ptr;
  std::queue<int> q;

  bool BFS() {
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int e : G[u]) {
        if (E[e].cap - E[e].flow < 1) continue;
        if (levels[E[e].v] != -1) continue;
        levels[E[e].v] = levels[u] + 1;
        q.push(E[e].v);
      }
    }
    return levels[t] != -1;
  }

  T DFS(int u, T pushed) {
    if (pushed == 0) return 0;
    if (u == t) return pushed;
    for (; ptr[u] < (int) G[u].size(); ptr[u]++) {
      int e = G[u][ptr[u]];
      int v = E[e].v;
      if (levels[u]+1 != levels[v] or E[e].cap - E[e].flow < 1) continue;
      T tr = DFS(v, std::min(pushed, E[e].cap - E[e].flow));
      if (tr == 0) continue;
      E[e].flow += tr;
      E[e ^ 1].flow -= tr;
      return tr;
    }
    return 0;
  }
};


using namespace std;
int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin>>n>>m;

  Dinic<> dnc(n);
  for (int i = 0; i < m; i++) {
    int a,b,c;
    cin>>a>>b>>c;
    if (a == b) continue;
    a--, b--;
    dnc.AddEdge(a,b,c);
    dnc.AddEdge(b,a,c);
  }


  cout << dnc.Solve(0, n-1);
}
