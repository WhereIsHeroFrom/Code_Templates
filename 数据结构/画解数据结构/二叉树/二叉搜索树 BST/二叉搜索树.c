#include <stdio.h>
#include <malloc.h>

#define bool int
#define true 1
#define false 0

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode* createNode(int val);
int BSTFindMin(struct TreeNode* root);
bool BSTFind(struct TreeNode* root, int val);
struct TreeNode* BSTInsert(struct TreeNode* root, int val);
struct TreeNode* BSTDelete(struct TreeNode* root, int val);
struct TreeNode* BSTConstruct(int *vals, int valSize);

struct TreeNode* createNode(int val) { 
    struct TreeNode* node = (struct TreeNode*) malloc( sizeof(struct TreeNode) );
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}
 
 /* 查找 
 * $(1)$ ```BSTFind```这个函数用于查找以```now```为根结点的树中是否存在值为```val```这个结点；
 * $(2)$ 空树是不可能存在值为```val```的结点的，直接返回```false```；
 * $(3)$ 一旦发现有值为```val```的结点，直接返回```true```；
 * $(4)$ ```val```的值 小于 **树根结点的值**，说明```val```对应的结点不在根结点，也不在右子树上，则递归返回左子树的 **查找** 结果；
 * $(5)$ ```val```的值 大于 **树根结点的值**，说明```val```对应的结点不在根结点，也不在左子树上，则递归返回右子树的 **查找** 结果；
 * */
bool BSTFind(struct TreeNode* root, int val) {    // (1) 
    if(root == NULL) {
        return false;                             // (2) 
    }
    if(root->val == val) {
        return true;                              // (3) 
    } 
    if(val < root->val) {
        return BSTFind(root->left, val);          // (4)
    }else {
        return BSTFind(root->right, val);         // (5)
    }
}


/* 插入 
* $(1)$ ```BSTInsert```函数用于将值为```val```的结点插入到以```root```为根结点的子树中；
* $(2)$ 如果是空树，则创建一个值为```val```的结点并且返回；
* $(3)$ ```val```的值 等于 **树根结点的值**，无须执行插入，直接返回根结点；
* $(4)$ ```val```的值 小于 **树根结点的值**，那么插入位置一定在 **左子树**，递归执行插入左子树的过程，并且返回插入结果作为新的**左子树**；
* $(5)$ ```val```的值 大于 **树根结点的值**，那么插入位置一定在 **右子树**，递归执行插入右子树的过程，并且返回插入结果作为新的**右子树**；
* */
struct TreeNode* BSTInsert(struct TreeNode* root, int val){ // (1)
    if(root == NULL) {                              
        return createNode(val);                             // (2)
    }
    if(val == root->val) {
        return root;                                        // (3)
    }
    if(val < root->val) {                                   // (4)
        root->left = BSTInsert(root->left, val);  
    }else {                                                 // (5)
        root->right = BSTInsert(root->right, val);          
    }
    return root;
}

/* 查找最小结点的值 
* $(1)$ 如果左子树存在，则递归调用左子树的查找最小结点接口；
* $(2)$ 如果左子树不存在，则当前根结点的值一定是最小的，直接返回接口；
*/
int BSTFindMin(struct TreeNode* root) {
    if(root->left)
        return BSTFindMin(root->left);  // (1)
    return root->val;                   // (2)
}



/*
* $(1)$ 如果左子树为空，则用右子树做为新的树根；
* $(2)$ 如果右子树为空，则用左子树作为新的树根；
* $(3)$ 否则，当左右子树都为非空时，利用```BSTFindMin```，从右子树上找出最小的结点，作为新的根，并且在右子树中删除对应的结点，删除过程就是递归调用```BSTDelete```的过程；
*/
struct TreeNode* Delete(struct TreeNode* root) {
    struct TreeNode *delNode, *retNode;
    if(root->left == NULL) {          // (1)
        delNode = root, retNode = root->right, free(delNode);
    }else if(root->right == NULL) {   // (2)
        delNode = root, retNode = root->left, free(delNode);
    }else {                           // (3)
        retNode = (struct TreeNode*) malloc (sizeof(struct TreeNode));
        retNode->val = BSTFindMin(root->right);
        retNode->right = BSTDelete(root->right, retNode->val);
        retNode->left = root->left;
    }
    return retNode;
}

/*
* $(1)$ 如果为空树，则直接返回空结点；
* $(2)$ 如果需要删除的结点，是这棵树的根结点，则直接调用接口```Delete```，下文会介绍它的实现；
* $(3)$ 如果需要删除的结点的值 小于 **树根结点的值**，则需要删除的结点必定在左子树上，递归调用左子树的删除，并且将返回值作为新的左子树的根结点；
* $(4)$ 如果需要删除的结点的值 大于 **树根结点的值**，则需要删除的结点必定在右子树上，递归调用右子树的删除，并且将返回值作为新的右子树的根结点；
* $(5)$ 最后，返回当前树的根结点；
*/ 
struct TreeNode* BSTDelete(struct TreeNode* root, int val){
    if(NULL == root) {
        return NULL;                                  // (1)
    }
    if(val == root->val) {
        return Delete(root);                          // (2)
    }
    else if(val < root->val) {
        root->left = BSTDelete(root->left, val);      // (3)
    }else if(val > root->val) {
        root->right = BSTDelete(root->right, val);    // (4)
    }
    return root;                                      // (5)
}

struct TreeNode* BSTConstruct(int *vals, int valSize) {
    int i;
    struct TreeNode* root = NULL;
    for(i = 0; i < valSize; ++i) {
        root = BSTInsert(root, vals[i]);
    }
    return root;
}






