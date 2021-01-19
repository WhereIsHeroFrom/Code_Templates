#include <iostream>
#include <queue>
#include <algorithm>
#include <cstring>

using namespace std;

//************ 整数散列哈希模板 ************
// 静态哈希数组大小
const int MAXH = ((1 << 20) - 1);
#define StateType long long 

class Hash {

public:
    Hash();
    virtual ~Hash();

private:
    bool *hashkey_;                   // 状态hash的key
    StateType *hashval_;              // 状态hash的val

public:
    // 销毁调用
    void finalize();
    // 初始化调用
    void initialize();
    // 设置获取给定值的哈希值
    int setKey(StateType val);
    int getKey(StateType val);
    // 查询是否有这个值在哈希表中
    bool hasKey(StateType val);
    // 获取给定哈希值的原值
    StateType getValue(int key);
};

Hash::Hash() : hashkey_(NULL), hashval_(NULL) {
    initialize();
}

Hash::~Hash() {
    finalize();
}

void Hash::finalize() {
    if (hashkey_) {
        delete[] hashkey_;
        hashkey_ = NULL;
    }
    if (hashval_) {
        delete[] hashval_;
        hashval_ = NULL;
    }
}
void Hash::initialize() {
    // 1. 释放空间避免内存泄漏
    // 2. 初始化哈希的key和val
    if (!hashkey_) {
        hashkey_ = new bool[MAXH + 1];
    }
    if (!hashval_) {
        hashval_ = new StateType[MAXH + 1];
    }
    memset(hashkey_, false, (MAXH + 1) * sizeof(bool));
}

int Hash::setKey(StateType val) {
    // 1. 采用 位与 代替 取模，位运算加速
    int key = (val & MAXH);
    while (1) {
        if (!hashkey_[key]) {
            // 2. 如果对应的key没有出现过，则代表没有冲突过；则key的槽位留给val；
            hashkey_[key] = true;
            hashval_[key] = val;
            return key;
        }
        else {
            if (hashval_[key] == val) {
                // 3. 如果key 的槽位正好和val匹配，则说明找到了，返回 key；
                return key;
            }
            // 4. 没有找到合适的 key， 进行二次寻址
            key = (key + 1) & MAXH;
        }
    }
}

int Hash::getKey(StateType val) {
    return setKey(val);
}

bool Hash::hasKey(StateType val) {
    int key = (val & MAXH);
    while (1) {
        if (!hashkey_[key]) {
            return false;
        }
        else {
            if (hashval_[key] == val) {
                return true;
            }
            key = (key + 1) & MAXH;
        }
    }
}

StateType Hash::getValue(int key) {
    if (key < MAXH && hashkey_[key]) {
        return hashval_[key];
    }
    return -1;
}
//************ 整数散列哈希模板 ************




//************ 字符串哈希模板 ************
// 字符串最大长度
const int MAXS = 100010;
const int BASE[] = { 19260817, 19260817, 12289, 133, 233 };
const int MOD[] = { 1e9 + 7, 1e9 + 9, 19260817 };

#define ll long long
#define ValueType int
#define AUTO_OVERFLOW


// 定义自动溢出后，相当于所有的数不显式进行取模，程序会根据补码规则对 2^64 取模
#ifdef AUTO_OVERFLOW
// 719MS
#define HashType unsigned long long
#else
// Double Hash 1938MS
#define HashType long long
#endif // AUTO_OVERFLOW


int valueTypeIndex(ValueType v) {
    return v;
}

class StringifyHash {
public:
    StringifyHash();
    virtual ~StringifyHash();

private:
    int base_, mod_;
    HashType *hash_;
    HashType *power_;

public:
    // 理论上只需要调用一次
    void initialize_basepower(int b = BASE[0], int p = MOD[0]);
    void initialize(int size, ValueType v[]);
    HashType get(int l, int r);
};

StringifyHash::StringifyHash() : hash_(NULL), power_(NULL) {
    hash_ = new HashType[MAXS];
    power_ = new HashType[MAXS];
}

StringifyHash::~StringifyHash() {
    if (hash_) {
        delete[] hash_;
        hash_ = NULL;
    }
    if (power_) {
        delete[] power_;
        power_ = NULL;
    }
}


void StringifyHash::initialize_basepower(int b /*= BASE[0]*/, int p /*= MOD[0]*/)
{
    base_ = b;
    mod_ = p;
    power_[0] = 1;
    for (int i = 1; i < MAXS; ++i) {
#ifdef AUTO_OVERFLOW
        power_[i] = power_[i - 1] * b;
#else
        power_[i] = power_[i - 1] * b;
        if (power_[i] >= p) power_[i] %= p;
#endif
    }
}

void StringifyHash::initialize(int size, ValueType v[]) {
    hash_[0] = 0;
    for (int i = 1; i <= size; ++i) {
#ifdef AUTO_OVERFLOW
        hash_[i] = (hash_[i - 1] * base_ + valueTypeIndex(v[i]));
#else
        hash_[i] = (hash_[i - 1] * base_ + valueTypeIndex(v[i]));
        if (hash_[i] >= mod_) hash_[i] %= mod_;
#endif
    }
}

HashType StringifyHash::get(int l, int r) {
#ifdef AUTO_OVERFLOW
    HashType hst = (hash_[r] - hash_[l - 1] * power_[r - l + 1]);
#else
    HashType hst = (hash_[r] - hash_[l - 1] * power_[r - l + 1]) % mod_;
    if (hst < 0) hst += mod_;
#endif
    return hst;
}
//************ 字符串哈希模板 ************



//************ 双哈希模板 ************
class DoubleStringifyHash {
private:
    StringifyHash shash_[2];

public:
    void initialize_basepower(int b1 = BASE[0], int p1 = MOD[0], int b2 = BASE[1], int p2 = MOD[1]);
    void initialize(int size, ValueType v[]);
    HashType get(int l, int r);
};



void DoubleStringifyHash::initialize_basepower(int b1 /*= BASE[0]*/, int p1 /*= MOD[0]*/, int b2 /*= BASE[1]*/, int p2 /*= MOD[1]*/){
    shash_[0].initialize_basepower(b1, p1);
    shash_[1].initialize_basepower(b2, p2);
}

void DoubleStringifyHash::initialize(int size, ValueType v[]) {
    shash_[0].initialize(size, v);
    shash_[1].initialize(size, v);
}

HashType DoubleStringifyHash::get(int l, int r) {
    HashType high = shash_[0].get(l, r);
    HashType low = shash_[1].get(l, r);
    return  high << 31 | low;
}
//************ 双哈希模板 ************

