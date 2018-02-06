#include <iostream>
#include <queue>
#include <cstring>
#include <algorithm>
#include <stack>

#define MAXN 4010
#define MAXE 2000010
 
using namespace std;

enum eColorType {
	COLOR_UNDEF = 0,
	COLOR_RED = 1,
	COLOR_BLUE = 2,
};

enum eSCCAlgorithmType {
	ESCCA_KOSARAJU = 0,
	ESCCA_TARJAN = 1,
};

class Edge {
public:
	int toVertex;
	Edge* next;

	Edge() {}
	void reset(int _to, Edge* _next) {
		toVertex = _to;
		next = _next;
	}
};
typedef Edge* EdgePtr;

class Graph {
	EdgePtr *head;
	Edge *edges;
	int edgeCount;
	int vertexCount;

	int *stack, stackTop;
	bool *inStack;

	int *visited;
	int *inDeg, *outDeg;

	void doPostOrderTraversal(int u, int *order, int &orderCount);
	void doPreOrderTraversal(int u, int *sccID, int &sccCount);
	void doTarjanTraversal(int u, int *dfn, int *low, int &dfnCount, int *sccID, int &sccCount);
public:


	Graph() {
		// 链式前向星 存储
		// 邻接表首结点
		head = new EdgePtr[MAXN];
		// 边内存池
		edges = new Edge[MAXE];
		// 遍历时，记录顶点是否被访问
		visited = new int[MAXN];
		// 遍历时的栈结构
		stack = new int[MAXN];
		inStack = new bool[MAXN];
		// 入度 + 出度
		inDeg = new int[MAXN];
		outDeg = new int[MAXN];
	}

	~Graph() {
		delete [] edges;
		delete [] head;
 		delete [] visited;
		delete [] stack;
		delete [] inStack;
		delete [] inDeg;
		delete [] outDeg;
	}

	void init(int vCount);
	void addEdge(int from, int to);
	Edge* getHead(int vertex);
	int getVertexCount();
	
	int getInDegree(int vertex);
	void subInDegree(int vertex);

	////////后序遍历 - 记录时间戳//////////
	void postOrderTraversal(int *order, int &orderCount);
	////////前序遍历 - 记录强连通//////////
	void preOrderTraversal(int *order, int orderCount, int *sccID, int &sccCount);

	////////Tarjan算法的遍历///////////////
	void tarjanTraversal(int *dfn, int *low, int &dfnCount, int *sccID, int &sccCount);
	//////// 缩图 //////////
	void doShrink(Graph* orgGraph, int *sccID, int sccCount, bool inverted);
};

void Graph::init(int vCount) {
	vertexCount = vCount;
	edgeCount = 0;
	stackTop = 0;
	for(int i = 0; i < vCount; i++) {
		head[i] = NULL;
		visited[i] = 0;
		inStack[i] = false;
		inDeg[i] = 0;
		outDeg[i] = 0;
	}
}

void Graph::addEdge(int from, int to) {
	edges[edgeCount].reset(to, head[from]);
	head[from] = &edges[edgeCount++];
	++ inDeg[to];
	++ outDeg[from];
}

Edge* Graph::getHead(int vertex) {
	return head[vertex];
}

int Graph::getVertexCount() {
	return vertexCount;	
} 

int Graph::getInDegree(int vertex) {
	return inDeg[vertex];
}

void Graph::subInDegree(int vertex) {
	--inDeg[vertex];
}
	
void Graph::doPostOrderTraversal(int u, int *order, int &orderCount) {
	if(visited[u]) {
		return;
	}
	visited[u] = 1;
	for(Edge *e = head[u]; e; e = e->next) {
		doPostOrderTraversal(e->toVertex, order, orderCount);
	}
	order[orderCount++] = u;
}

void Graph::postOrderTraversal(int *order, int &orderCount) {
	for(int i = 0; i < vertexCount; ++i) {
		doPostOrderTraversal(i, order, orderCount);
	}
}

void Graph::doPreOrderTraversal(int u, int *sccID, int &sccCount) {
	if(visited[u]) {
		return;
	}
	visited[u] = 1;
	sccID[u] = sccCount;
	for(Edge *e = head[u]; e; e = e->next) {
		doPreOrderTraversal(e->toVertex, sccID, sccCount);
	}
}

void Graph::preOrderTraversal(int *order, int orderCount, int *sccID, int &sccCount) {
	for(int i = orderCount-1; i >= 0; --i) {
		int u = order[i];
		if(visited[u]) {
			continue;
		}
		doPreOrderTraversal(u, sccID, sccCount);
		++ sccCount;
	}
}

void Graph::doTarjanTraversal(int u, int *dfn, int *low, int &dfnCount, int *sccID, int &sccCount) {
	// 1.记录当前访问结点u的时间戳，并且将它入栈
	dfn[u] = low[u] = dfnCount++;
	stack[stackTop++] = u;
	inStack[u] = true;

	// 2.访问所有和u连接的边(u->v)
	for(Edge *e = getHead(u); e; e = e->next) {
		int v = e->toVertex;
		if(!visited[v]) {
			// 3.如果结点v尚未访问，则标记已访问，递归遍历v
			visited[v] = 1;
			doTarjanTraversal(v, dfn, low, dfnCount, sccID, sccCount);
			// 4.v能够访问到的时间戳更早的结点，则更新low[u]
			if(low[v] < low[u]) {
				low[u] = low[v];
			}
		}else if(inStack[v]) {
			// 5.v在栈中，说明在本次访问中，v为u的祖先
			// 并且如果v的时间戳早于u，则更新u的时间戳
			if(dfn[v] < low[u]) {
				low[u] = dfn[v];
			}
		}
	}

	// 6.当u能够追溯到的最小时间戳等于它本身，
	//   说明它是该强连通分量子树的根结点，栈中的元素属于同一个强连通分量
	if(dfn[u] == low[u]) {
		while(stackTop) {
			int v = stack[--stackTop];
			inStack[v] = false;
			sccID[v] = sccCount;
			if(v == u) {
				break;
			}
		}
		++ sccCount;
	}
}

void Graph::tarjanTraversal(int *dfn, int *low, int &dfnCount, int *sccID, int &sccCount) {
	stackTop = 0;
	for(int i = 0; i < vertexCount; i++) {
		if(!visited[i]) {
			visited[i] = 1;
			doTarjanTraversal(i, dfn, low, dfnCount, sccID, sccCount);
		}
	}
}

void Graph::doShrink(Graph* orgGraph, int *sccID, int sccCount, bool inverted) {
	init(sccCount);
	for(int i = 0; i < orgGraph->vertexCount; i++) {
		int from = sccID[i];
		for(Edge *e = orgGraph->getHead(i); e; e = e->next) {
			int to = sccID[e->toVertex]; 
			if(from != to) {
				if(inverted) {
					addEdge(to, from);
				}else {
					addEdge(from, to);
				}
			}
		}
	}
}

class GraphEx {

	// 原图、反图
	Graph *orgGraph, *invGraph;
	// 缩点后的图，一般建反向图便于拓扑排序 
	Graph *shrinkGraph;
	// 算法类型
	eSCCAlgorithmType esccaType;

	void buildSCCVertex();
	void clearSCCVertex();
	void addSCCVertex(int sccID, int vertexID);

	void Kosaraju();
	void Tarjan();
	void doColorFloodFill(Graph *g, int u, eColorType color);
	void doTopological(Graph* g);
public:
	// 用于Kosaraju算法记录后序遍历的顺序
	int *order, orderCount;
	// 用于Tarjan算法记录时间戳
	int *dfn, *low, dfnCount;

	int *sccID, sccCount;
	
	// 记录强连通
	EdgePtr *sccHead;
	Edge *sccVertex;
	int sccVertexCount;
	
	eColorType *vertexColor;
	int *result, resultCount;

	GraphEx() {
		orgGraph = new Graph;
		invGraph = new Graph;
		shrinkGraph = new Graph;

		// 拓扑排序时染色用
		vertexColor = new eColorType[MAXN];
		// 记录同一个强连通分量中原图的点
		sccHead = new EdgePtr[MAXN];
		sccVertex = new Edge[MAXE];
		// Kosaraju - 后序遍历记录的访问顺序
		order = new int[MAXN];
		// Tarjan - 时间戳
		dfn = new int[MAXN];
		// Tarjan - low数组
		low = new int[MAXN];
		// 强连通编号映射
		sccID = new int[MAXN];
		// 缩点后的拓扑序
		result = new int[MAXN]; 
	}
	
	~GraphEx() {
		delete orgGraph;
		delete invGraph;
		delete shrinkGraph;

		delete [] vertexColor;
		delete [] sccHead;
		delete [] sccVertex;
		
		delete [] dfn;
		delete [] low;
		delete [] order;
		delete [] sccID;
		delete [] result;
	}
	
	void init(int vCount, eSCCAlgorithmType et);
	void addEdge(int from, int to);
	int getOppsiteVertex(int v);
	bool SAT();

	static GraphEx& Instance() {
		static GraphEx inst;
		return inst; 
	}
};
void GraphEx::init(int vCount, eSCCAlgorithmType et) {
	orderCount = 0;
	sccCount = 0;
	esccaType = et;
	orgGraph->init(vCount);
	if(esccaType == ESCCA_KOSARAJU)
		invGraph->init(vCount);
}

void GraphEx::addEdge(int from, int to) {
	//printf("%d -> %d\n", from, to);
	orgGraph->addEdge(from, to);
	if(esccaType == ESCCA_KOSARAJU)
		invGraph->addEdge(to, from);
}

void GraphEx::buildSCCVertex() {
	// 将sccID存入scc链表
    clearSCCVertex();
	for(int i = 0; i < orgGraph->getVertexCount(); ++i) {
		addSCCVertex(sccID[i], i);
	}
}

void GraphEx::clearSCCVertex() {
	sccVertexCount = 0;
	for(int i = 0; i < orgGraph->getVertexCount(); ++i) {
		sccHead[i] = NULL;
	}
}

void GraphEx::addSCCVertex(int sccID, int vertexID) {
	sccVertex[sccVertexCount].reset(vertexID, sccHead[sccID]);
	sccHead[sccID] = &sccVertex[sccVertexCount++];
}

void GraphEx::Kosaraju() {
	// 对反图求一次后序遍历
	invGraph->postOrderTraversal(order, orderCount);
	// 按照后序的逆序遍历原图
	orgGraph->preOrderTraversal(order, orderCount, sccID, sccCount);
}

void GraphEx::Tarjan() {
	orgGraph->tarjanTraversal(dfn, low, dfnCount, sccID, sccCount);
}

int GraphEx::getOppsiteVertex(int v) {
	int p = v & 1;
	int q = v >> 1;
	return q << 1 | (1-p);
}

void GraphEx::doColorFloodFill(Graph *g, int u, eColorType color) {
	if(vertexColor[u] != COLOR_UNDEF) {
		return ;
	}
	vertexColor[u] = color;
	for(Edge *e = g->getHead(u); e; e = e->next) {
		doColorFloodFill(g, e->toVertex, color);
	}
}

void GraphEx::doTopological(Graph* g) {
	int i;
	queue <int> Q;
	// 1.初始化所有顶点颜色，置为“未定义色”
	// 2.将所有入度为0的顶点放入队列 
	for(i = 0; i < g->getVertexCount(); ++i) {
		vertexColor[i] = COLOR_UNDEF;
		if(!g->getInDegree(i)) {
			Q.push(i);
		}
	}
	
	while(!Q.empty()) {
		int u = Q.front();
		Q.pop();
		// 3.如果某个顶点颜色未定义，将它染成“红色” 
		if(vertexColor[u] != COLOR_UNDEF) {
			continue;
		}
		vertexColor[u] = COLOR_RED; 
		// 4.将所有和红色对立的顶点和它的子孙染成“蓝色” 
		for(Edge *vPtr = sccHead[u]; vPtr; vPtr = vPtr->next) {
			int ov = getOppsiteVertex(vPtr->toVertex);
			doColorFloodFill(g, sccID[ov], COLOR_BLUE);
		}
		// 5.将红色点能够到达的点的边去掉，然后将入度为0的点放入队列（拓扑排序核心） 
		for(Edge *ePtr = g->getHead(u); ePtr; ePtr = ePtr->next) {
			int v = ePtr->toVertex;
			g->subInDegree(v);
			if(!g->getInDegree(v)) {
				Q.push(v);	
			}
		}
	}
	
	// 6.将所有红色顶点塞到结果数组 
	resultCount = 0;
	for(i = 0; i < g->getVertexCount(); ++i) {
		if(vertexColor[i] == COLOR_RED) {
			for(Edge *vPtr = sccHead[i]; vPtr; vPtr = vPtr->next) {
				result[ resultCount++ ] = vPtr->toVertex;
			}
		}
	}
}

bool GraphEx::SAT() {

	if(esccaType == ESCCA_KOSARAJU)
		Kosaraju();
	else if(esccaType == ESCCA_TARJAN)
		Tarjan();

	// 需要约定 2i 和 2i+1 是对立面
	// 即：A = 2i，A' = 2i+1 
	for(int i = 0; i < orgGraph->getVertexCount(); i += 2) {
		// A 成立，推导出A'成立，和事实不符 
		if(sccID[i] == sccID[i+1]) {
			return false;
		}
	}
	// 建立强连通分量
	//buildSCCVertex();
	// 按照原图进行缩图（缩边 + 缩点） ，inverted为True表示建反图
	//shrinkGraph->doShrink(orgGraph, sccID, sccCount, true);
	// 拓扑排序
	//doTopological(shrinkGraph);
	return true;
}
