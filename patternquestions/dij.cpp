
int main() {


int n,m;
cin >> n >> m;



vector<vector<pair<int, int>>> adj(n);

for (int i = 0; i < m; i++) {
    int u, v, w;

cin>>u>>v>>w;

adj [u].push_back({v, w});

adj[v].push_back({u, w}); // since i/p graph is undirected

int s = 0; // assume node 0 to be the src vertex

vector<int> dis(n, INF);

dis [s] = 0;

vector<bool> ex(n, false);

set<pair<int, int>> minHeap;

for (int i = 0; i < n; i++) {
    minHeap.insert({dis[i], i});
}

return 0;
}