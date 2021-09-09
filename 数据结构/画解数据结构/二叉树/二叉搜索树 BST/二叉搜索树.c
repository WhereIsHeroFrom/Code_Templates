#include <stdio.h>

#define bool int

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};
 
// BST 的递归查找 
bool find(struct TreeNode* now, int val) {                      // (1) ```find```这个函数用于查找```now```这棵子树中是否存在```val```这个结点;
    if(now == NULL) {
        return 0;                                               // (2) 空树是不可能存在```val```这个结点的，则返回```false```;
    }
    if(now->val == val) {
        return 1;                                               // (3) 一旦发现有```val```，直接返回```true```；
    }
    if(val < now->val) {
        return find(now->left, val);                            // (4) 二叉搜索树性质：如果发现比当前结点小，递归搜索左子树；
    }else {
        return find(now->right, val);                           // (5) 二叉搜索树性质：如果发现比当前结点大，递归搜索右子树；
    }
}
