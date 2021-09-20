#include <stdio.h>

#define MAXN 1024
#define DataType int

 
typedef struct TreeNode {
    DataType data;
    struct TreeNode *left;
    struct TreeNode *right;
}TreeNode;

typedef struct {
    TreeNode nodes[MAXN];
    int root;
    int n;
}Tree;

#define N 9
int nodes[N][3] = {
    {'a', 1, 2},
    {'b', 3, -1},
    {'c', 4, 5},
    {'d', 6, 7},
    {'e', -1, -1},
    {'f', 8, -1},
    {'g', -1, -1},
    {'h', -1, -1},
    {'i', -1, -1},
};

void initTree(Tree *tree) {
   int i;
   for(i = 0; i < MAXN; ++i) {
       tree->nodes[i].data = -1;
       tree->nodes[i].left = NULL;
       tree->nodes[i].right = NULL;
   }
   tree->n = N;
   tree->root = 0;
   
   for(i = 0; i < N; ++i) {
       tree->nodes[i].data  = nodes[i][0];
       tree->nodes[i].left  = nodes[i][1] == -1 ? NULL : &tree->nodes[ nodes[i][1] ];
       tree->nodes[i].right = nodes[i][2] == -1 ? NULL : &tree->nodes[ nodes[i][2] ];
   }
}

void visit(TreeNode *root) {
    printf("%c", root->data);
}
 
void preorder(TreeNode *root) {
    if(root == NULL) {
        return ;
    }
    visit(root);
    preorder(root->left);
    preorder(root->right);
}

void inorder(TreeNode *root) {
    if(root == NULL) {
        return ;
    }
    inorder(root->left);
    visit(root);
    inorder(root->right);
}

void postorder(TreeNode *root) {
    if(root == NULL) {
        return ;
    }
    postorder(root->left);
    postorder(root->right);
    visit(root);
}


int main() {
    Tree tree;
    initTree(&tree);
    printf("前序遍历："), preorder(& tree.nodes[tree.root]), printf("\n");
    printf("中序遍历："), inorder(& tree.nodes[tree.root]), printf("\n");
    printf("后序遍历："), postorder(& tree.nodes[tree.root]), printf("\n");
    
    return 0;
}
