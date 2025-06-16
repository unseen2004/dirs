

template <typename KeyType>
class TrieNode {
public:
    std::unordered_map<KeyType, TrieNode*> children;
    bool end_of_word;

    TrieNode() : end_of_word(false) {}
    ~TrieNode() {
        for (auto& pair : children) {
            delete pair.second;
        }
    }
};

template <typename KeyType>
class Trie {
private:
    TrieNode<KeyType>* m_root;

    void print(TrieNode<KeyType>* node, std::basic_string<KeyType> prefix) const {
        if (node->end_of_word) {
            std::cout << prefix << "\n";
        }
        for (const auto& pair : node->children) {
            print(pair.second, prefix + pair.first);
        }
    }

public:
    Trie() {
        m_root = new TrieNode<KeyType>();
    }

    ~Trie() {
        delete m_root;
    }

    void insert(const std::basic_string<KeyType>& word) {
        TrieNode<KeyType>* node = m_root;
        for (const KeyType& key : word) {
            if (node->children.find(key) == node->children.end()) {
                node->children[key] = new TrieNode<KeyType>();
            }
            node = node->children[key];
        }
        node->end_of_word = true;
    }

    bool search(const std::basic_string<KeyType>& word) const {
        TrieNode<KeyType>* node = m_root;
        for (const KeyType& key : word) {
            if (node->children.find(key) == node->children.end()) {
                return false;
            }
            node = node->children[key];
        }
        return node->end_of_word;
    }

    void erase(const std::basic_string<KeyType>& word) {
        TrieNode<KeyType>* node = m_root;
        for (const KeyType& key : word) {
            if (node->children.find(key) == node->children.end()) {
                return; // Word does not exist.
            }
            node = node->children[key];
        }
        if (node->end_of_word) {
            node->end_of_word = false;
        }
    }

    void print() const {
        print(m_root, std::basic_string<KeyType>());
    }
};

