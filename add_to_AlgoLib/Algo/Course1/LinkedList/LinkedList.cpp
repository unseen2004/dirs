
struct node{
    int m_val;
    node* m_next;

    node(int val) : m_val(val), m_next(nullptr)  
    {
    }
};

class LinkedList{
    node* m_head;
public: 
    LinkedList() : m_head(nullptr);
    {

    }

    ~LinkedList(){
        while(m_head){
            pop();
        }
    }

    void add(int val){
        node* newNode = new node(val);
        if(m_head == nullptr){
            m_head = newNode;
        }else{
            node* tmp = m_head;
            while(tmp->m_next){
                tmp = tmp->m_next;
            }
            tmp->m_next = newNode;
        }
    }

    void pop(){
        if(m_head==nullptr){
            return;
        }else if(m_head->next == nullptr){
            delete m_head;
            m_head = nullptr;
        }else{
            node* tmp = m_head;
            while(tmp->next->next){
            tmp = tmp->next;
            }
            delete tmp->next;
            tmp->next = nullptr;
        }
    }
