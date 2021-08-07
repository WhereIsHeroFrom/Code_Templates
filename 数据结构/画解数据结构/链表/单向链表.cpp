#include <stdio.h>
#include <malloc.h>

typedef int DataType;

enum RetType {
    RET_OK = 0,
    RET_ERROR = 1,
};
 
 // 链表结点 
struct ListNode {
    DataType data;
    ListNode *next;
};

// 给定链表头结点，打印链表 
void ListPrint(ListNode *head) {
    ListNode *vtx = head;
    while(vtx) {                      // (1) 从头结点开始，循环遍历所有结点； 
        printf("%d -> ", vtx->data);  // (2) 遍历到的结点，将结点的 数据域 带上```->```后输出;
        vtx = vtx->next;              // (3) 将当前结点指向当前结点的后继结点； 
    }
    printf("NULL\n");                 // (4) 最后输出一个```NULL```，代表一个完整的链表；
}

// 创建一个链表结点 
ListNode *ListCreateNode(DataType data) {
    ListNode *node = (ListNode *) malloc ( sizeof(ListNode) );
    node->data = data;
    node->next = NULL;
    return node;
}


// 获取链表第 i 个结点 
// 1）  head 为链表头，并且头结点内有数据 
// 2）  i >= 0
ListNode *ListGetNode(ListNode *head, int i) {
    ListNode *temp = head;       // (1) temp 代表从链表头开始的游标指针 
    int j = 0;                   // (2) j 代表当前访问到了第几个结点 
    while(temp && j < i) {       // (3) 如果游标指针非空，并且 j < i 
        temp = temp->next;       // (4) 游标指针指向它的后继结点 
        ++j;                     // (5) j = j + 1 
    }
    if(!temp || j > i) {
        return NULL;             // (6) 跑到这种情况说明：给定的 i 超过了链表长度 
    }
    return temp;                 // (7) 最后，返回找到的第 i 个结点 
}

// 查找链表中值为 v 的第一个结点 
// 1）  head 为链表头，并且头结点内有数据 
ListNode *ListFindNodeByValue(ListNode *head, DataType v) {
    ListNode *temp = head;       // (1) temp 代表从链表头开始的游标指针
    while(temp) {                // (2) 如果游标指针非空，继续循环 
        if(temp->data == v) {
            return temp;         // (3) 一旦发现值相等，立即返回该结点 
        } 
        temp = temp->next;       // (4) 游标指针指向它的后继结点 
    }
    return NULL;                 // (5) 找不到，返回 NULL 
}



// 在第 i 个结点后面插入一个数据，数据值为 v 
// 规则说明：  
// 1）  Head 为链表头，并且头结点内有数据 
// 2）  i >= 0
ListNode *ListInsertNode(ListNode *head, int i, DataType v) {
    ListNode *pre, *aft, *vtx;                     // (1) 插入完毕后，  pre -> vtx -> aft 
    int j = 0;                                     // (2) 定义一个计数器，当 j == i 时，表明找到要插入的位置 
    pre = head;                                    // (3) 从链表头开式 
    while(pre && j < i) {                          // (4) 如果还没有到链表尾，或者没有找到插入位置则继续循环 
        pre = pre->next;                           // (5) 游标指针指向它的后继结点 
        ++j;                                       // (6) 计数器加 1 
    }
    if(!pre) { 
        return NULL;                               // (7) 元素个数不足，无法找到给定位置，返回 NULL 
    }
    vtx = ListCreateNode(v);                       // (8) 创建一个值为 v 的鼓孤立结点 
    aft = pre->next;                               // (9) - (11) 为了串成  pre -> vtx -> aft 
    vtx->next = aft;                               // (10)
    pre->next = vtx;                               // (11)
    return vtx;                                    // (12) 返回插入的那个结点 
}

// 要分情况： 
// 将第 Index 个结点删除，并且将值存储在 deleteData 中 
// 规则说明： 
// 1）  Head 为链表头，并且头结点内有数据 
// 2）  i >= 0 
ListNode *ListDeleteNode(ListNode *head, int i) {
    ListNode *pre, *del, *aft;
    int j = 0;
    if(head == NULL) {
        return NULL;              // (1) 空链表，无法执行删除，直接返回 
    }
    if(i == 0) {                  // (2) 需要删除链表第 0 个结点 
        del = head;               // (3) 缓存第 0 个结点       
        head = head->next;        // (4) 将新的链表头结点变为当前头结点的后继结点 
        free(del);                // (5) 调用系统库函数 free 释放内存 
        return head;              // (6) 返回新的链表头结点 
    }
    
    pre = head;                   // (7) 执行链表结点的游走 
    while(pre && j < i - 1) {     // (8) 找到将要被删除结点的前驱结点 
        pre = pre->next;
        ++ j;
    }
    if(!pre || !pre->next) {      // (9) 如果前驱结点为空，或者需要被删除的结点为空，则直接返回当前链表头 
        return head;
    }
    del = pre->next;              // (10) 缓存需要删除的结点到 del 
    aft = del->next;              // (11) 缓存删除结点的后继结点到 aft 
    pre->next = aft;              // (12) 将需要删除的结点的前驱结点指向它的后继结点 
    free(del);                    // (13) 释放需要删除结点的内存空间 
    return head;                  // (14) 返回新的链表头结点 
} 

ListNode *ListCreateListByTail(int n, int a[]) {
    ListNode *head, *tail, *vtx;         // (1) head存储头结点的地址，tail存储尾结点的地址，vtx存储当前正在插入结点的地址； 
    int idx;                              
    if(n <= 0)
        return NULL;                     // (2) 当元素个数为 0 时，直接返回空链表； 
    idx = 0;
    vtx = ListCreateNode(a[0]);          // (3) 创建一个元素值为 a[0] 的链表结点； 
    head = tail = vtx;                   // (4) 将链表头 和 链表尾 都置为 vtx； 
    while(++idx < n) {                   // (5) 循环遍历数组 ； 
        vtx = ListCreateNode(a[idx]);    // (6) 对数组中除了第一个元素以外，依次创建链表结点； 
        tail->next = vtx;                // (7) 将当前链表尾结点的后继结点置为 vtx； 
        tail = vtx;                      // (8) 将 vtx 作为新的 链表尾结点； 
        
    } 
    return head;                         // (9) 返回链表头结点 
} 

ListNode *ListCreateListByHead(int n, int *a) {
    ListNode *head = NULL, *vtx;       // (1) head存储头结点的地址，初始为空， vtx存储当前正在插入结点的地址； 
    while(n--) {                       // (2) 总共需要插入 n 个结点，逆序的 n 层循环； 
        vtx = ListCreateNode(a[n]);    // (3) 创建一个元素值为 a[i] 的链表结点，注意，这里 i 的取值为 $n-1 \to 0$； 
        vtx->next = head;              // (4) 将当前创建的结点指向 链表的头结点； 
        head = vtx;                    // (5) 将链接头结点置为 vtx；
    } 
    return head;                       // (6) 返回链表头结点； 
} 

/*                     以下为测试用例                     */

void testListGetNode(ListNode *head) {
    int i;
    for(i = 0; i < 7; ++i) {
        ListNode *node = ListGetNode(head, i);
        if(!node)
            printf("index(%d) is out of range.\n", i);
        else 
            printf("node(%d) is %d.\n", i, node->data);
    }    
}

void testListFindNodeByValue(ListNode *head) {
    int i;
    for(i = 1; i <= 6; ++i) {
        ListNode *node = ListFindNodeByValue(head, i);
        if(!node)
            printf("value(%d) is not found!\n", i);
        else 
            printf("value(%d) is found!\n", i);
    }    
}

void testListInsertNode(ListNode *head) {
    ListPrint(head);
    printf("执行插入操作！\n"); 
    ListInsertNode(head, 1, 8);
    ListPrint(head);
}

void testListDeleteNode(ListNode *head) {
    ListPrint(head);
    printf("执行 2 号操作！\n"); 
    head = ListDeleteNode(head, 2);
    ListPrint(head);

    printf("执行 0 号操作！\n"); 
    head = ListDeleteNode(head, 0);
    ListPrint(head);
}

int main() {    
    int a[5] = {1, 3, 8, 2, 6};
    ListNode *head = ListCreateListByHead(5, a);
    testListDeleteNode(head);
    return 0;
}










