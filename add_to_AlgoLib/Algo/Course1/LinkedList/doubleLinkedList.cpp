struct Node{
    int val;
    Node* next;
    Node* prev;

    Node(int v, Node*next = nullptr, Node* prev = nullptr);
};

class DoubleLinkedList{
    Node* m_head;
    Node* m_tail;
public: 
    DoubleLinkedList() : m_head(nullptr), m_tail(nullptr){}
    
    ~DoubleLinkedList(){
        while(m_tail){
            pop();
        }
    }

    void push(int val){
        Node* newNode = new Node(val, nullptr, nullptr);
        if(m_tail==nullptr){
            m_head = m_tail = newNode;
        }else{
            m_tail->next = newNode;
            newNode->prev = m_tail;
            m_tail = newNode;
        }
    }

    void pop(){
        if(m_tail == nullptr){
            return;
        }else if(m_tail == m_head){
            delete m_tail;
            m_tail = m_head = nullptr;
        }else{
            Node* toDel = m_tal;
            m_tail = m_tail->prev;
            m_tail->next = nullptr;
            delete toDel;
        }
    }

};
