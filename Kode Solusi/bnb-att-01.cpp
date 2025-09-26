#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> res;
vector<int> cover_set;
vector<int> status;
bool edge_covered[151][151]; 

void bb(vector<vector<int>> G, int k, int uncov, int& best) {
    if (k == 0) {
        if (uncov < best) {
            best = uncov;
            res.clear();
            res = cover_set;
        }

        return;
    }
    int max_coverable = 0;
    vector<int> degrees;
    for (int i = 0; i < G.size(); i++) if(status[i]==0) degrees.push_back(G[i].size());
    sort(degrees.rbegin(), degrees.rend());
    for(int i = 0; i < min(k, (int)degrees.size()); i++) max_coverable += degrees[i];
    if(uncov - max_coverable >= best) return;

    int vertex = -1, max_deg = -1;
    for (int i = 0; i < G.size(); i++) {
        if (status[i] != 0) continue;

        int deg = G[i].size();
        if (deg > max_deg) {
            max_deg = deg;
            vertex = i;
        }
    }

    if (vertex == -1) return;

    vector<pair<int,int>> newly_covered;
    for (int neighbor : G[vertex]) {
        if (!edge_covered[vertex][neighbor]) {
            edge_covered[vertex][neighbor] = edge_covered[neighbor][vertex] = true;
            newly_covered.push_back({vertex, neighbor});
        }
    }
    cover_set.push_back(vertex);
    status[vertex] = 1;
    bb(G, k - 1, uncov - newly_covered.size(), best);
    cover_set.pop_back();
    status[vertex] = 0;
    for (auto e : newly_covered) edge_covered[e.first][e.second] = edge_covered[e.second][e.first] = false;

    status[vertex] = 2;
    bb(G, k, uncov, best);
    status[vertex] = 0;

    return;
}

int main() {
    int T;
    scanf("%d", &T);

    while (T--) {
        int M, N;
        scanf("%d %d", &M, &N);
        vector<vector<int>> G(M);
        for (int i = 0; i < N; i++) {
            int u, v;
            scanf("%d %d", &u, &v);
            G[u].push_back(v);
            G[v].push_back(u);
        }
        int best = 1e9;

        status.assign(M, 0);
        for (int k = 1; k <= M; k++) bb(G, k, N, best);

        printf("%d\n", res.size());
        res.clear();
    }

    return 0;
}