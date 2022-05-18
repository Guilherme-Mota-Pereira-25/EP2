#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

template <class Key, class Item>
class RedBlackTree: public SymbolTable<Key,Item> {

    template<class N_Key, class N_Item>
    class Node {
    
    public:
        enum NodeType {BLACK,RED};
        N_Key key;
        N_Item item;
        NodeType nodeType;
        int nqtt;
        Node<N_Key,N_Item>* parent;
        Node<N_Key,N_Item>* left;
        Node<N_Key,N_Item>* right;
        Node(N_Key nkey = N_Key{}, N_Item nitem = N_Item{}) {
            key = nkey; item = nitem;
            left = nullptr; right = nullptr; parent = nullptr;
            nqtt = 1; nodeType = RED;
        }       
    };

    private:
        Node<Key,Item>* root;
        // Métodos recursivos
        Node<Key,Item>* r_add(Node<Key,Item>* r_root, Key key, Item item);
        Item r_value(Node<Key,Item>* r_root, Key key);
        int r_rank(Node<Key,Item>* r_root, Key key);
        Key r_select(Node<Key,Item>* r_root, int s);
        Node<Key,Item>* rotateLeft(Node<Key,Item>* m_root);
        Node<Key,Item>* rotateRight(Node<Key,Item>* m_root);

    public:
        char* show();
        void add(Key key, Item item);
        Item value(Key key);
        int rank(Key key);
        Key select(int s);
};

template <class Key, class Item>
RedBlackTree<Key,Item>::Node<Key,Item>* RedBlackTree<Key,Item>::rotateLeft(Node<Key,Item>* m_root) {
    Node<Key,Item>* n_root = m_root->right;
    Node<Key,Item>* move = n_root->left;

    n_root->parent = m_root->parent;
    n_root->left = m_root;
    m_root->right = move;
    m_root->parent = n_root;
    m_root->nqtt -= n_root->nqtt - (move==nullptr?0:move->nqtt);
    n_root->nqtt += m_root->nqtt - (move==nullptr?0:move->nqtt);
    if(move != nullptr)
        move->parent = m_root;
    return n_root;
}

template <class Key, class Item>
RedBlackTree<Key,Item>::Node<Key,Item>* RedBlackTree<Key,Item>::rotateRight(Node<Key,Item>* m_root) {
    Node<Key,Item>* n_root = m_root->left;
    Node<Key,Item>* move = n_root->right;

    n_root->parent = m_root->parent;
    n_root->right = m_root;
    m_root->left = move;
    m_root->parent = n_root;
    m_root->nqtt -= n_root->nqtt - (move==nullptr?0:move->nqtt);
    n_root->nqtt += m_root->nqtt - (move==nullptr?0:move->nqtt);
    if(move != nullptr)
        move->parent = m_root;
    return n_root;
}

template <class Key, class Item>
char* RedBlackTree<Key,Item>::show() {
    char* s = (char*)"E eu sou uma Árvore rubro-negra!!! XD";
    return s;
}

template <class Key, class Item>
void RedBlackTree<Key,Item>::add(Key key, Item item) {
    root = r_add(root,key,item);
    root->nodeType = Node<Key,Item>::BLACK;
}

template <class Key, class Item>
RedBlackTree<Key,Item>::Node<Key,Item>* RedBlackTree<Key,Item>::r_add(Node<Key,Item>* r_root, Key key, Item item) {
    if (r_root == nullptr)
        return new Node<Key,Item>(key,item);

    Node<Key,Item>* uncle;
    if (r_root->key == key)
        r_root->item = item;
    else if (key > r_root->key) { // Chave inserida pela direita
        r_root->right = r_add(r_root->right,key,item);
        r_root->right->parent = r_root;
        r_root->nqtt += 1;        
        // Paridade consecutiva de vermelhos resolvida pelo avô:
        if (r_root->right->nodeType == Node<Key,Item>::RED) {
            if (r_root->right->right != nullptr && r_root->right->right->nodeType == Node<Key,Item>::RED) {
                if (r_root->left == nullptr || r_root->left->nodeType == Node<Key,Item>::BLACK) {// Tio Preto
                    r_root = rotateLeft(r_root);
                    r_root->nodeType = Node<Key,Item>::BLACK;
                    r_root->left->nodeType = Node<Key,Item>::RED;
                } else {// Tio vermelho
                    r_root->nodeType = Node<Key,Item>::RED;
                    r_root->right->nodeType = Node<Key,Item>::BLACK;
                    r_root->left->nodeType = Node<Key,Item>::BLACK;
                }
            } else if (r_root->right->left != nullptr && r_root->right->left->nodeType == Node<Key,Item>::RED) {
                if (r_root->left == nullptr || r_root->left->nodeType == Node<Key,Item>::BLACK) {// Tio Preto
                    r_root->right = rotateRight(r_root->right);
                    r_root = rotateLeft(r_root);
                    r_root->nodeType = Node<Key,Item>::BLACK;
                    r_root->left->nodeType = Node<Key,Item>::RED;
                } else {// Tio vermelho
                    r_root->nodeType = Node<Key,Item>::RED;
                    r_root->right->nodeType = Node<Key,Item>::BLACK;
                    r_root->left->nodeType = Node<Key,Item>::BLACK;
                }
            }
        }
    }
    else { // Chave inserida pela esquerda
        r_root->left = r_add(r_root->left,key,item);
        r_root->left->parent = r_root;
        r_root->nqtt += 1;        
        // Paridade consecutiva de vermelhos resolvida pelo avô:
        if (r_root->left->nodeType == Node<Key,Item>::RED) {
            if (r_root->left->left != nullptr && r_root->left->left->nodeType == Node<Key,Item>::RED) {
                if (r_root->right == nullptr || r_root->right->nodeType == Node<Key,Item>::BLACK) {// Tio Preto
                    r_root = rotateRight(r_root);
                    r_root->nodeType = Node<Key,Item>::BLACK;
                    r_root->right->nodeType = Node<Key,Item>::RED;
                } else {// Tio vermelho
                    r_root->nodeType = Node<Key,Item>::RED;
                    r_root->right->nodeType = Node<Key,Item>::BLACK;
                    r_root->left->nodeType = Node<Key,Item>::BLACK;
                }
            } else if (r_root->left->right != nullptr && r_root->left->right->nodeType == Node<Key,Item>::RED) {
                if (r_root->right == nullptr || r_root->right->nodeType == Node<Key,Item>::BLACK) {// Tio Preto
                    r_root->left = rotateLeft(r_root->left);
                    r_root = rotateRight(r_root);
                    r_root->nodeType = Node<Key,Item>::BLACK;
                    r_root->right->nodeType = Node<Key,Item>::RED;
                } else {// Tio vermelho
                    r_root->nodeType = Node<Key,Item>::RED;
                    r_root->right->nodeType = Node<Key,Item>::BLACK;
                    r_root->left->nodeType = Node<Key,Item>::BLACK;
                }
            }
        }
    }

    return r_root;
}

template <class Key, class Item>
Item RedBlackTree<Key,Item>::value(Key key) {    
    return r_value(root,key);
}

template <class Key, class Item>
Item RedBlackTree<Key,Item>::r_value(Node<Key,Item>* r_root, Key key) {    
    if (r_root == nullptr)
        return Item{};
    
    if (key == r_root->key)
        return r_root->item;
    else if (key > r_root->key)
        return r_value(r_root->right, key);
    else
        return r_value(r_root->left, key);
}

template <class Key, class Item>
int RedBlackTree<Key,Item>::rank(Key key) {
    return r_rank(root,key);
}

template <class Key, class Item>
int RedBlackTree<Key,Item>::r_rank(Node<Key,Item>* r_root, Key key) {
    if (r_root == nullptr)
        return 0;
    
    if (r_root->key == key)
        return (r_root->left == nullptr)?0:r_root->left->nqtt;
    else if (r_root->key < key)
        return ((r_root->left==nullptr)?0:r_root->left->nqtt) + 1 + ((r_root->right==nullptr)?0:r_rank(r_root->right,key));
    else
        return r_rank(r_root->right,key);
}

template <class Key, class Item>
Key RedBlackTree<Key,Item>::select(int s) {
    if (root == nullptr)
        return Key{};
    return r_select(root, s);
}

template <class Key, class Item>
Key RedBlackTree<Key,Item>::r_select(Node<Key,Item>* r_root, int s) {
    if (r_root->left != nullptr) {
        if(r_root->left->nqtt == s)
            return r_root->key;
        else if (r_root->left->nqtt > s)
            return r_select(r_root->left,s);
        else {
            Node<Key,Item>* n_root = (r_root->right==nullptr?r_root->left:r_root->right);
            return r_select(n_root, s-(r_root->left->nqtt+1));
        }
    } else if (r_root->left != nullptr) {
        if(r_root->right->nqtt == s)
            return r_root->key;
        else
            return r_select(r_root->right,s-1);
    } else {
        if(s == 0)
            return r_root->key;
        else
            return Key{};
    }
}

#endif