#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

class TrieNode {
public:
    char data;
    vector<TrieNode*> next;
    bool end_char;
    TrieNode(char c) {
        data = c;
        end_char = false;
    }
};

TrieNode* create_trie() {
    return new TrieNode('/');
}

TrieNode* find_next(TrieNode* node, char c, bool add) {
    int left = 0;
    int right = node->next.size();
    while (left < right) {
        int mid = (left + right) / 2;
        TrieNode* p = node->next[mid];
        if (p->data == c) {
            return p;
        }
        else if (p->data < c) {
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }
    if (add) {
        TrieNode* n = new TrieNode(c);
        node->next.insert(node->next.begin()+left, n);
        return n;
    }
    return nullptr;
}

void add_word(TrieNode* root, string word) {
    TrieNode* p = root;
    for (char c : word) {
        p = find_next(p, c, true);
    }
    p->end_char = true;
}

bool find_word(TrieNode* root, string word) {
    TrieNode* p = root;
    for (char c : word) {
        p = find_next(p, c, false);
        if (p == nullptr) {
            return false;
        }
    }
    return p->end_char;
}

void release_trie(TrieNode* root) {
    for (TrieNode* p : root->next) {
        release_trie(p);
    }
    root->next.clear();
    delete root;
}

// 回溯法打印trie树
void print_traceback(TrieNode* root, string& word) {
    if (root->end_char) {
        printf("trie : %s\n", word.c_str());
        return;
    }
    for (TrieNode* p : root->next) {
        word.push_back(p->data);
        print_traceback(p, word);
        word.pop_back();
    }
}
void print_trie(TrieNode* root){
    string word = "";
    print_traceback(root, word);
    printf("\n");
}

// 测试
void test_trie(vector<string>& words) {
    TrieNode* root = create_trie();
    for (string& word : words) {
        add_word(root, word);
    }
    print_trie(root);

    srand(time(NULL));
    for (int i = 0; i < 5; ++i) {
        int index = rand() % words.size();
        bool ret = find_word(root, words[index]);
        printf("find %s->%d\n", words[index].c_str(), ret);
    }
    bool ret = find_word(root, "aaa");
    printf("find %s->%d\n", "aaa", ret);

    release_trie(root);
}
