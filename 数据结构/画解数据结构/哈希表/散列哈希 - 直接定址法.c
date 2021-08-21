/******************** 哈希表 偏移法 ********************/
#define maxn ((1<<17)-1)
#define DataType int
#define Boolean int
#define NULLKEY (maxn+maxn)    /* 空槽标记不能用-1，会导致正常值也为-1的情况*/
#define Base 100002

typedef struct {
    DataType data[maxn + 1];
}HashTable;

void HashInit(HashTable *ht) {
    int i;
    for(i = 0; i < maxn + 1; ++i) {
        ht->data[i] = NULLKEY;
    }
}

int HashInsert(HashTable *ht, DataType key) {
    int addr = key + Base;
    ht->data[addr] = key;
    return addr;
}

Boolean HashSearchKey(HashTable *ht, DataType key, int *addr) {
    *addr = key + Base;
    return ht->data[*addr] == key;
}

/******************** 哈希表 便宜法 ********************/
