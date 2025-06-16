class BST {
private:
    struct Node {
        int m_data;
        Node* m_left;
        Node* m_right;

        Node(int data) : m_data(data), m_left(nullptr), m_right(nullptr) {}
    };

    Node* m_root;

public:
    // Constructor
    BST() : m_root(nullptr);

    // Destructor
    ~BST(){
	    clear(m_root);
    };

    // Insert a new value into the BST
    void insert(int value){
	m_root = insert(m_root, value);
    }

    // Remove a value from the BST
    bool remove(int value){
    	if(contains(value)){
		m_root = remove(m_root, value);
		return true;
	}
	return false;
    }

    // Check if a value exists in the BST
    bool contains(int value) const{
   	return contains(m_root, value); 
    }

    // Get the minimum value in the BST
    int getMin() const{
    	if(!m_root) throw runtime_error("Trie is empty!");
	Node * minNode = getMinNode(m_root);
	return minNode;
    }

    // Get the maximum value in the BST
    int getMax() const{
    	if(!m_root) throw runtime_error("Trie is empty!");
	Node* maxNode = getMaxNode(m_root);
	return maxNode;
    }

    // Get the height of the BST
    int getHeight() const{
    	return getHeight(m_root);
    }

    // Print the BST in-order
    void printInOrder() const{
    	printInOrder(m_root);
	std::cout<<endl;
    }

private:
    // Helper function for inserting a value
    Node* insert(Node* node, int value){
    	if(!node)	return new Node(value);
	
	    if(value < node->m_data){
			node->m_left =  insert(node->m_left, value);
		}else if(value > node->m_data){
			node->m_right = insert(node->m_right, value);
		}
			return node;
		
    }

    Node* remove(Node* node, int value){
	if(!node) return nullptr;

	if(value < node->m_data){
		node->m_left = remove(node->m_left, value);
	}else if(value > node->m_data){
		node->m_right = remove(node->m_right, value);
	}else{
		if(!node->m_left){
			Node* temp = node->m_right;
			delete node;
			return temp;
		}else if(!node->m_right){
			Node* temp = node->m_left;
			delete node;
			return temp;
		}else{
			Node* temp = getMinNode(node->m_right);
			node->m_data = temp->m_data;
			node->m_right = remove(node->m_right, temp->m_data);
		}
	}
    }

    // Helper function for searching a value
    bool contains(Node* node, int value) const{
    	if(!node) return false;

	if(value < node-> m_value){
		node->left = contains(node->left, value);
	}else if(value > node->m_value){
		node->right = contains(node->right, value);
	}
	return true;
    }

    // Helper function to get the minimum node
    Node* getMinNode(Node* node) const{
    	while(node && node->m_left){
		node = node->m_left;
	}
	return node;
    }

    // Helper function to get the maximum node
    Node* getMaxNode(Node* node) const{
    	while(node && node->m_right){
		node = node->m_right;
	}
	return node;
    }

    // Helper function to get the height of a node
    int getHeight(Node* node) const{
    	if(!node) return -1;
	return 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    // Helper function to print in-order traversal
    void printInOrder(Node* node) const{
    	if(!node) return;
	printInOrder(node->m_left);
	std::cout<<node->m_data<<" ";
	printInOrder(node->m_right);
    }

    // Helper function to delete all nodes in the tree
    void clear(Node* node){
    	if(!node) return;
	clear(node->m_left);
	clear(node->m_right);
	delete node;
    }
};

