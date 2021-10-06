#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>

#define false 0
#define true 1

typedef struct KeyValue {
    int key;
    int value;
}KeyValue;

typedef KeyValue DataType;
typedef int bool;

typedef struct TreeNode {
    DataType data;                    // (1) 二叉平衡树的键值，数据类型要求能够比较大小； 
    int height;                       // (2) 树的高度； 
    struct TreeNode* left, * right;    // (3) 左右孩子结点的指针； 
}TreeNode;                            // (4) 为了少写一个 struct；

TreeNode* AVLRemove(TreeNode* T, DataType data);

int Max(int a, int b) {
    return a > b ? a : b;
}

int CompareData(DataType* a, DataType* b) {
    if (a->key < b->key) {
        return -1;
    }
    else if (a->key > b->key) {
        return 1;
    }
    return 0;
}

TreeNode* AVLCreateNode(DataType data) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = data;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

int AVLGetHeight(TreeNode* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

void AVLCalcHeight(TreeNode* node) {
    if (NULL == node) {
        return;
    }
    node->height = 1 + Max(AVLGetHeight(node->left), AVLGetHeight(node->right));
}

int AVLGetBalanceFactor(TreeNode* node) {
    if (node == NULL)
        return 0;
    return AVLGetHeight(node->left) - AVLGetHeight(node->right);
}


/*
  当传入一个二叉搜索树 T，完成一次右旋操作；
*/
TreeNode* RRotate(TreeNode* T) {
    TreeNode* L = T->left;  // (1) L 为 T 的左孩子结点；
    T->left = L->right;     // (2) 将 L 的右子树作为 T 的左子树； 
    L->right = T;           // (3) 将 T 作为 L 的右子树；
    AVLCalcHeight(T);       // (4) T 的左孩子结点产生变化，所以需要重新计算树高； 
    AVLCalcHeight(L);       // (5) L 是 T 的父结点，T的树高变了，L也需要重新计算； 
    return L;               // (6) 将原先的 L 作为新的树根返回； 
}


/*
  当传入一个二叉搜索树 T，完成一次左旋操作；
*/
TreeNode* LRotate(TreeNode* T) {
    TreeNode* R = T->right; // (1) R 作为 T 的右孩子结点； 
    T->right = R->left;     // (2) 将 R 的左子树作为 T 的右子树； 
    R->left = T;            // (3) 将 T 作为 R 的左子树； 
    AVLCalcHeight(T);       // (4) T 的右孩子结点产生变化，所以需要重新计算树高； 
    AVLCalcHeight(R);       // (5) R 是 T 的父结点，T的树高变了，R 的树高也需要重新计算； 
    return R;               // (6) 将原先的 R 作为新的树根返回； 
}

// LL 型 
TreeNode* AVLTreeLL(TreeNode* T) {
    return RRotate(T);
}

// RR 型 
TreeNode* AVLTreeRR(TreeNode* T) {
    return LRotate(T);
}

// LR 型 
TreeNode* AVLTreeLR(TreeNode* T) {
    T->left = LRotate(T->left);
    return RRotate(T);
}

// RL 型
TreeNode* AVLTreeRL(TreeNode* T) {
    T->right = RRotate(T->right);
    return LRotate(T);
}

TreeNode* AVLBalance(TreeNode* T) {
    int bf = AVLGetBalanceFactor(T);

    if (bf > +1) {
        if (AVLGetBalanceFactor(T->left) > 0)
            T = AVLTreeLL(T);                 // (1) LL 型
        else
            T = AVLTreeLR(T);                 // (2) LR 型
    }

    if (bf < -1) {
        if (AVLGetBalanceFactor(T->right) > 0)
            T = AVLTreeRL(T);                 // (3) RL 型
        else
            T = AVLTreeRR(T);                 // (4) RR 型
    }

    AVLCalcHeight(T);

    return T;                                 // (5) 已经平衡，无需调整； 
}

/*
  平衡树的插入，并且在后序遍历的时候进行平衡，这样，就能做到从叶子到根都是逐级平衡的；
*/
TreeNode* AVLInsert(TreeNode* T, DataType data) {
    int cmpResult;
    if (T == NULL) {
        T = AVLCreateNode(data);               // (1) 即往空树插入一个结点，则直接生成一个结点返回即可； 
        return T;
    }
    cmpResult = CompareData(&data, &T->data);
    if (cmpResult == 0) {
        return T;                              // (2) 插入的值等于当前树根结点，直接返回当前树根，相当于没有执行插入； 
    }
    else if (cmpResult < 0) {
        T->left = AVLInsert(T->left, data);    // (3) 往左子树插入； 
    }
    else {
        T->right = AVLInsert(T->right, data);  // (4) 往右子树插入； 
    }
    return AVLBalance(T);                      // (5) 平衡性调整； 
}

/*
  平衡树的查找，由于查找过程不改变树的形态，所以不需要执行平衡操作；
*/
bool AVLFind(TreeNode* T, DataType data) {
    int cmpResult;
    if (T == NULL) {
        return false;
    }
    cmpResult = CompareData(&data, &T->data);
    if (cmpResult < 0) {
        return AVLFind(T->left, data);
    }
    else if (cmpResult > 0) {
        return AVLFind(T->right, data);
    }
    return true;
}

TreeNode* AVLGetMin(TreeNode* T) {
    while (T && T->left) {
        T = T->left;
    }
    return T;
}

TreeNode* AVLGetMax(TreeNode* T) {
    while (T && T->right) {
        T = T->right;
    }
    return T;
}

/*
    移除 T 的根结点，并且保持树的平衡性；
*/
TreeNode* AVLRemoveRoot(TreeNode* T) {

    TreeNode* retNode;
    if (T == NULL) {
        return NULL;
    }
    if (!T->left && !T->right) {        // (1) 叶子结点，可直接删除，删除后返回空树； 
        free(T);
        return NULL;
    }
    else if (T->left && !T->right) {   // (2) 只有左子树的情况：删除根，返回左子树； 
        retNode = T->left;
        free(T);
        return retNode;
    }
    else if (!T->left && T->right) {   // (3) 只有右子树的情况：删除根，返回右子树； 
        retNode = T->right;
        free(T);
        return retNode;
    }

    if (AVLGetHeight(T->left) > AVLGetHeight(T->right)) {  // (4) 左子树更重，则从左子树选择最大值替换根结点，并且递归删除左子树对应结点；
        T->data = AVLGetMax(T->left)->data;
        T->left = AVLRemove(T->left, T->data);
    }else {                                                // (5) 右子树更重，则从右子树选择最小值替换根结点，并且递归删除右子树对应结点；         
        T->data = AVLGetMin(T->right)->data;
        T->right = AVLRemove(T->right, T->data);
    }
    AVLCalcHeight(T);
    return T;
}

TreeNode* AVLRemove(TreeNode* T, DataType data) {
    int cmpResult;
    if (T == NULL) {
        return NULL;
    }
    cmpResult = CompareData(&data, &T->data);
    if (cmpResult == 0) {
        return AVLRemoveRoot(T);
    }else if (cmpResult < 0) {
        T->left = AVLRemove(T->left, data);
    }
    else if (cmpResult > 0) {
        T->right = AVLRemove(T->right, data);
    }
    return AVLBalance(T);
}



void AVLDestroy(TreeNode* T) {
    if (T == NULL) {
        return;
    }
    AVLDestroy(T->left);
    AVLDestroy(T->right);
    free(T);
}
