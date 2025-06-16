
template <typename T>
bool compare(Node<T>* a, Node<T>* b){
    if(a==null && b==null)
        return true;
    if(a==null || b==null)
        return false;
    if(a->val != b->val)
        return false

    return compare(a->left, b->left) && compare(a->right, b->right);
}
