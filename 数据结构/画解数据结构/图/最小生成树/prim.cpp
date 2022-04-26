int minSpanningTree(int n, int dist[maxn][maxn]) {
    int i, u, ret, dis;
    int cost[maxn];                                
    for(i = 0; i < n; ++i) {
        cost[i] = (i == 0) ? 0 : dist[0][i];       // (1)  
    }
    ret = 0;                                       // (2)
    while(1) {
        dis = inf;
        for(i = 0; i < n; ++i) {                   // (3)
            if(cost[i] && lessthan(cost[i], dis) ) {
                dis = cost[i];
                u = i;
            }
        }
        if(dis == inf) {
            return ret;                            // (4)
        }
        ret += cost[u];                            // (5)
        cost[u] = 0;                               // (6)
        for(i = 0; i < n; ++i) {                   // (7)
            if(cost[i] && lessthan(dist[u][i], cost[i])) {
                cost[i] = dist[u][i];
            }
        }
    }

    return inf;
}
