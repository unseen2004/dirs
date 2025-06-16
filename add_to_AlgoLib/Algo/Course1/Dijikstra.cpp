
typedef std::pair<int, int> Edge;


void dijkstra(int src, const std::vector<std::vector<Edge>>& graph){
    int n = graph.size();
    std::vector<int> dist(n, MAX_INT);
    std::vector<bool> seen(n, false);

    dist[src] = 0;
    Using Pair = std::pair<int, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<>> pq; // min heap
    pq.push({0, src});

    while(!pq.empty()){
        int currDist = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        if(seen[node]) continue;
        seen[node] = true;

        for(const auto& edge : graph[node]){
            int v = edge.first;
            int weight = edge.second;
            
            if(!seen[v] && dist[v] > currDist + weigth){
                dist[v] = currDist + weight;
                pq.push({dist[v], v});
            }
        }
    }

}

