#define MAXN 1024
#define DataType int

typedef struct  {
    DataType data;
    int parent;
}TreeNode; 

typedef struct  {
    TreeNode nodes[MAXN];
    int root;
    int n;
}Tree;

