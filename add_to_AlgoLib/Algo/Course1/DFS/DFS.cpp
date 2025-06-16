
struct Node{
    int val;
    Node* next;
    Node* prev;

    Node(int x) : val(x), next{nullptr}, prev{nullptr} {
    }
};

bool search(Node* node, int target){
    if(!node){
        return false;
    }
    if(node->val == target){
        return true;
    }
    if(node->val < target){
        return search(node->right, target);
    }
    else{
        return search(node->left, target);
    }
}

bool dfs(Node* head, int target){
    return search(head, target);
}
