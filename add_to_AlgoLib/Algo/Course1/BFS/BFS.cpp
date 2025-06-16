#include <queue>

template <typename T>
struct Node {
    T val;
    Node* left;
    Node* right;

    Node(T x) : val(x), left(nullptr), right(nullptr) {}
};

template <typename T>
bool BFS(Node<T>* head, T target){
    if(!head)
        return false;

    std::queue<Node<T>*> q;
    q.push(head);

    while(!q.empty()){
        const auto curr = q.front();
        q.pop();

        if(curr->val == target){
            return true;
        }

        if(curr->left){
            q.push(curr->left);
        }

        if(curr->right){
            q.push(curr->right);
        }


    }

    return false;
}
