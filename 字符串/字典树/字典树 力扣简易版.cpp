class TrieNode {
public:
    bool end;             // 1 表示从根结点到它是一个完整字典中的串
                          // 0 表示它是某个字符串的前缀
    TrieNode *next[26];   // 指向所有的子结点

    TrieNode() {
        end = false;
        memset(next, 0, sizeof(next));
    }
};

class Trie {
    TrieNode* root;
public:
    Trie() {
        root = new TrieNode();
    }
    
    void insert(string word) {
        TrieNode *now = root;
        for(int i = 0; i < word.size(); ++i) {
            int child = word[i] - 'a';
            if( nullptr == now->next[child] ) {
                now->next[child] = new TrieNode();
            }
            now = now->next[child];
        }
        now->end = true;
    }
    
    bool search(string word) {
        TrieNode *now = root;
        for(int i = 0; i < word.size(); ++i) {
            int child = word[i] - 'a';
            if( nullptr == now->next[child]) {
                return false;
            }
            now = now->next[child];
        }
        return now->end;
    }
    
    bool startsWith(string prefix) {
        TrieNode *now = root;
        for(int i = 0; i < prefix.size(); ++i) {
            int child = prefix[i] - 'a';
            if( nullptr == now->next[child]) {
                return false;
            }
            now = now->next[child];
        }
        return true;
    }
};
