#ifndef TWOTHREETREE_H
#define TWOTHREETREE_H

#include "SymbolTable.h"

template <class Key, class Item>
class TwoThreeTree: public SymbolTable<Key,Item>{

    enum NodeType {LEAF,TWO,THREE};

    // template<class N_Key, class N_Item>
    // class subNode {
    //     public:
    //     N_Key key;
    //     N_Item item;
    //     subNode(N_Key nkey = N_Key{}, N_Item nitem = N_Item{}) {
    //         key = nkey; item = nitem;
    //     }
    // }
    template<class N_Key, class N_Item>
    class Node {
    public:
        N_Key f_key;
        N_Item f_item;
        N_Key s_key;
        N_Item s_item;
        int nqtt;
        Node<N_Key,N_Item>* left;
        Node<N_Key,N_Item>* middle;
        Node<N_Key,N_Item>* right;
        NodeType nodeType;
        Node(N_Key key = N_Key{}, N_Item item = N_Item{}) {
            f_key = key; f_item = item;
            left = nullptr; middle = nullptr; right = nullptr; nqtt = 1;
            nodeType = LEAF; s_key = N_Key{}; s_item = N_Item{};
        }
        // Retorna false caso o node esteja saturado (Somente é chamado em casos de LEAF e TWO)
        bool add(N_Key key, N_Item item) {
            if (s_key != N_Key{})
                return false;
            
            nqtt++;
            if (key > f_key) {
                s_key = key; s_item = item;
            } else {
                s_key = f_key; s_item = f_item;
                f_key = key; f_item = item;
            }
            return true;
        }
        // Divide uma folha de dois elementos adicionada de [key,item] a fim de se estabelecer uma nova raiz.
        Node<N_Key,N_Item>* split(N_Key key, N_Item item) {
            Node<N_Key,N_Item>* n_root;
            if (key > s_key) {
                n_root = new Node<N_Key,N_Item>(s_key,s_item);
                n_root->nodeType = TWO;
                s_key = N_Key{}; s_item = N_Item{};
                n_root->left = this;
                n_root->right = new Node<N_Key,N_Item>(key,item);
            } else if (key > f_key) {
                n_root = new Node<N_Key,N_Item>(key,item);
                n_root->nodeType = TWO;
                n_root->left = this;
                n_root->right = new Node<N_Key,N_Item>(s_key,s_item);
                s_key = N_Key{}; s_item = N_Item{};
            } else {
                n_root = new Node<N_Key,N_Item>(f_key,f_item);
                n_root->nodeType = TWO;
                n_root->right = this;
                n_root->left = new Node<N_Key,N_Item>(key,item);
                f_key = s_key; f_item = s_item;
                s_key = N_Key{}; s_item = N_Item{};
            }
            n_root->nqtt = 3; nqtt = 1;
            return n_root;
        }
    };

    private:
        Node<Key,Item>* root;
        // Métodos recursivos
        Node<Key,Item>* r_add(Node<Key,Item>* r_root, Key key, Item item);
        Item r_value(Node<Key,Item>* r_root, Key key);
        int r_rank(Node<Key,Item>* r_root, Key key);
        Key r_select(Node<Key,Item>* r_root, int s);

    public:
        char* show();
        void add(Key key, Item item);
        Item value(Key key);
        int rank(Key key);
        Key select(int s);
        TwoThreeTree();
};

template <class Key, class Item>
TwoThreeTree<Key,Item>::TwoThreeTree() {
    root = nullptr;
}

template <class Key, class Item>
char* TwoThreeTree<Key,Item>::show() {
    char* s = (char*)"E aí!? Eu sou uma árvore 2-3! :D";
    return s;
}

template <class Key, class Item>
void TwoThreeTree<Key,Item>::add(Key key, Item item) {
    root = r_add(root,key,item);
}

template <class Key, class Item>
TwoThreeTree<Key,Item>::Node<Key,Item>* TwoThreeTree<Key,Item>::r_add(Node<Key,Item>* r_root, Key key, Item item) {
    if (r_root == nullptr)
        return new Node<Key,Item>(key,item);
    
    if (r_root->f_key == key)
        r_root->f_item = item;
    else if (r_root->s_key == key)
        r_root->s_item = item;
    else {
        switch (r_root->nodeType) {
            case LEAF: {// Caso o nó visitado seja folha [com 1 ou 2 elementos]
                if (!r_root->add(key,item)) // caso o nó esteja saturado:
                    r_root = r_root->split(key,item); //divide-o
            } break;
            case TWO: {// Caso o nó visitado seja 2-nó
                if (key < r_root->f_key) {// Inserção pela esquerda do 2-nó
                    Node<Key,Item>* aux = r_add(r_root->left,key,item);
                    if (aux == r_root->left) { // Não cresceu
                        r_root->left = aux; r_root->nqtt++;
                    } else { // Cresceu
                        aux->nodeType = THREE;
                        aux->add(r_root->f_key,r_root->f_item);
                        aux->middle = aux->right; aux->right = r_root->right;
                        r_root = aux;
                    }
                } else {// Inserção pela direita do 2-nó
                    Node<Key,Item>* aux = r_add(r_root->right,key,item);
                    if (aux == r_root->right) // Não cresceu
                        r_root->right = aux;
                    else { // Cresceu
                        aux->nodeType = THREE;
                        aux->add(r_root->f_key,r_root->f_item);
                        aux->middle = aux->left; aux->left = r_root->left;
                        r_root = aux;
                    }
                }
            } break;
            case THREE: {// Caso o nó visitado seja 3-nó
                if (key < r_root->f_key) {
                    Node<Key,Item>* aux = r_add(r_root->left,key,item);
                    if (aux == r_root->left)// Não cresceu
                        r_root->left = aux;
                    else {// Cresceu
                        Node<Key,Item>* n_root = new Node<Key,Item>(r_root->f_key,r_root->f_item);
                        n_root->nodeType = TWO;
                        n_root->left = aux; n_root->right = r_root;
                        r_root->f_key = r_root->s_key; r_root->f_item = r_root->s_item;
                        r_root->s_key = Key{}; r_root->s_item = Item{};
                        r_root->nodeType = TWO;
                        r_root->left = r_root->middle; r_root->middle = nullptr;
                        r_root = n_root;
                    }
                } else if (key < r_root->s_key) {
                    Node<Key,Item>* aux = r_add(r_root->middle,key,item);
                    if (aux == r_root->middle)// Não cresceu
                        r_root->middle = aux;
                    else {// Cresceu
                        Node<Key,Item>* n_right = new Node<Key,Item>(r_root->s_key,r_root->s_item);
                        n_right->nodeType = TWO;
                        r_root->s_key = Key{}; r_root->s_item = Item{};
                        r_root->right = aux->left;
                        n_right->left = aux->right;
                        aux->left = r_root; aux->right = n_right;
                        r_root->nodeType = TWO; r_root->middle = nullptr;
                        r_root = aux;
                    }
                } else {
                    Node<Key,Item>* aux = r_add(r_root->right,key,item);
                    if (aux == r_root->right)// Não cresceu
                        r_root->right = aux;
                    else {// Cresceu
                        Node<Key,Item>* n_root = new Node<Key,Item>(r_root->s_key,r_root->s_item);
                        n_root->nodeType = TWO;
                        r_root->s_key = Key{}; r_root->s_item = Item{};
                        n_root->left = r_root; n_root->right = aux;
                        r_root->right = r_root->middle; r_root->middle = nullptr;
                        r_root->nodeType = TWO;
                        r_root = n_root;
                    }
                }
            } break;
        }
    }

    return r_root;
}

template <class Key, class Item>
Item TwoThreeTree<Key,Item>::value(Key key) {    
    return r_value(root,key);
}

template <class Key, class Item>
Item TwoThreeTree<Key,Item>::r_value(Node<Key,Item>* r_root, Key key) {    
    if (r_root == nullptr)
        return Item{};
    
    if (key == r_root->f_key)
        return r_root->f_item;
    else if (key == r_root->s_key)
        return r_root->s_item;
    else {
        if (key < r_root->f_key)
            return r_value(r_root->left, key);
        else if (r_root->s_key != Key{}) {
            if (key < r_root->s_key)
                return r_value(r_root->middle, key);
            else
                return r_value(r_root->right, key);
        } else
            return r_value(r_root->right,key);
    }
}

template <class Key, class Item>
int TwoThreeTree<Key,Item>::rank(Key key) {
    return r_rank(root,key);
}

template <class Key, class Item>
int TwoThreeTree<Key,Item>::r_rank(Node<Key,Item>* r_root, Key key) {
    if (r_root == nullptr)
        return 0;

    if (r_root->f_key == key)
        return (r_root->left==nullptr)?0:r_root->left->nqtt;
    else if (r_root->s_key == key)
        return ((r_root->left==nullptr)?0:r_root->left->nqtt) + 1 + (r_root->middle==nullptr?0:r_root->middle->nqtt);
    else {
        if (key < r_root->f_key)
            return r_rank(r_root->left, key);
        else if (r_root->s_key != Key{}) {
            if (key < r_root->s_key)
                return (r_root->left==nullptr?0:r_root->left->nqtt) + 1+ r_rank(r_root->middle,key);
            else
                return (r_root->left==nullptr?0:r_root->left->nqtt) + 2 + (r_root->middle==nullptr?0:r_root->middle->nqtt) + r_rank(r_root->right,key);
        } else
            return (r_root->left==nullptr?0:r_root->left->nqtt) + 1 + r_rank(r_root->right,key);
    }
}

template <class Key, class Item>
Key TwoThreeTree<Key,Item>::select(int s) {
    if (root == nullptr)
        return Key{};
    return r_select(root, s);
}

template <class Key, class Item>
Key TwoThreeTree<Key,Item>::r_select(Node<Key,Item>* r_root, int s) {

    if (r_root->left != nullptr)  {
        if (s == r_root->left->nqtt)
            return r_root->f_key;
        else if (s < r_root->left->nqtt)
            return r_select(r_root->left,s);
        else if (r_root->s_key != Key{}) {
            s -= r_root->left->nqtt;
            // Colocar 
        } else {

        }
    }

    // if (r_root->left != nullptr) {
    //     if(r_root->left->nqtt == s)
    //         return r_root->key;
    //     else if (r_root->left->nqtt > s)
    //         return r_select(r_root->left,s);
    //     else {
    //         Node<Key,Item>* n_root = (r_root->right==nullptr?r_root->left:r_root->right);
    //         return r_select(n_root, s-(r_root->left->nqtt+1));
    //     }
    // } else if (r_root->left != nullptr) {
    //     if(r_root->right->nqtt == s)
    //         return r_root->key;
    //     else
    //         return r_select(r_root->right,s-1);
    // } else {
    //     if(s == 0)
    //         return r_root->key;
    //     else
    //         return Key{};
    // }
    return Key{};
}

#endif