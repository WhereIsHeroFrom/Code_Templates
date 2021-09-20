#define MAXN 1024
#define DataType int

 
typedef struct TreeNode {
    DataType data;
    struct TreeNode *left;
    struct TreeNode *right;
}TreeNode;

typedef struct  {
    TreeNode nodes[MAXN];
    int root;
    int n;
}Tree;
