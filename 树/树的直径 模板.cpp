#include <iostream>
#include <algorithm>

using namespace std;

#define MAXN 100010
#define MAXE 2000020
#define INF 100000000

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
///////////////////// Diameter of tree /////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

/*    
    有向带权边
*/ 
class Edge {
public:    
    int toVertex;
    int edgeValue; 
    Edge* next;

    Edge() {}
    void reset(int _to, int _val, Edge* _next) {
        toVertex = _to;
        edgeValue = _val; 
        next = _next;
    }
};
typedef Edge* EdgePtr;

/*
    有向图 
*/
class Graph {
    EdgePtr *head;
    Edge *edges;
    int *visit;
    int edgeCount;
    int vertexCount;
    int diameter;

    Graph() {
        // 链式前向星 存储
        // 邻接表首结点
        head = new EdgePtr[MAXN];
        // 标记结点是否已经被访问 
        visit = new int[MAXN];
        // 边内存池
        edges = new Edge[MAXE];
    }
    ~Graph() {
        delete [] edges;
        delete [] head;
        delete [] visit;
    }
public:
    
    void init(int vCount);
    void resetVisit();
    void addEdge(int from, int to, int value);
    bool isEnd(int vertex);
    void updateDiameter(int dist);
    int getTreeDiameter(int root, int fat);
    int getDiameter();

    static Graph& Instance() {
        static Graph inst;
        return inst; 
    }
};

void Graph::init(int vCount) {
    vertexCount = vCount;
    edgeCount = 0;
    for(int i = 0; i <= vCount; i++) {
        head[i] = NULL;
    }
}

void Graph::addEdge(int from, int to, int value) {
    edges[edgeCount].reset(to, value, head[from]);
    head[from] = &edges[edgeCount++];
}

void Graph::resetVisit() {
    for(int i = 0; i <= vertexCount; ++i) {
        visit[i] = 0;
    }
}

bool Graph::isEnd(int vertex) {
    if(!head[vertex]) {
        return true;
    }
    int u = head[vertex]->toVertex;
    for(Edge *e = head[vertex]->next; e; e = e->next) {
        if(e->toVertex != u) {
            return false;
        }
    }
    return true;
}

void Graph::updateDiameter(int dist) {
    if(dist > diameter) {
        diameter = dist;
    }
}

int Graph::getTreeDiameter(int root, int fat) {
    int maxDist = 0, secMaxDist = 0;
    visit[root] = 1;
    for(Edge *e = head[root]; e; e = e->next) {
        if(e->toVertex == fat) {
            continue;
        }
        int dist = e->edgeValue + getTreeDiameter(e->toVertex, root);
        updateDiameter(dist);
        
        // 更新最大路径和次大路径 
        if(dist > maxDist) {
            secMaxDist = maxDist;
            maxDist = dist;
        }else if(dist > secMaxDist) {
            secMaxDist = dist;
        }
    }
    updateDiameter(maxDist + secMaxDist);
    return maxDist;
}

int Graph::getDiameter() {
    int i;
    diameter = 0;
    resetVisit(); 
    for(i = 1; i <= vertexCount; ++i) {
        if(!visit[i] && isEnd(i)) {
            getTreeDiameter(i, -1);
        }
    }
    return diameter;
}


int n, m;

// 并查集判无向环 
// SPFA判有向环 
int f[MAXN];
int find_set(int a) {
    return f[a] == a ? a : f[a] = find_set(f[a]); 
}

int merge_set(int a, int b) {
    int fa = find_set(a);
    int fb = find_set(b);
    if(fa == fb) {
        return false;
    }
    f[fa] = fb;
}

int main() {
    int i;
    while(scanf("%d %d", &n, &m) != EOF) {
        Graph &g = Graph::Instance();
        g.init(n);
        bool flag = false;
        for(i = 0; i <= n; ++i) f[i] = i;
        for(i = 0; i < m; ++i) {
            int x, y, z;
            scanf("%d %d %d", &x, &y, &z);
            g.addEdge(x, y, z);
            g.addEdge(y, x, z);
            
            if(!flag && !merge_set(x, y)) {
                flag = 1;
            }
         }
         if(flag) {
             puts("YES");
             continue;
        }
         printf("%d\n", g.getDiameter());
    }
    return 0;
}
/*
3 2
2 1 3
1 3 3
*/
