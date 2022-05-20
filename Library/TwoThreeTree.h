#ifndef TWOTHREETREE_H
#define TWOTHREETREE_H

#include "SymbolTable.h"

template <class Key, class Item>
class TwoThreeTree: public SymbolTable<Key,Item>{

    enum NodeType {LEAF,TWO,THREE};

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
                        r_root->nodeType = THREE;
                        r_root->add(aux->f_key, aux->f_item);
                        r_root->left = aux->left; r_root->middle = aux->right;
                        r_root->nqtt = 2 + r_root->left->nqtt + r_root->middle->nqtt + r_root->right->nqtt;
                    }
                } else {// Inserção pela direita do 2-nó
                    Node<Key,Item>* aux = r_add(r_root->right,key,item);
                    if (aux == r_root->right) { // Não cresceu
                        r_root->right = aux; r_root->nqtt++;
                    } else { // Cresceu
                        r_root->nodeType = THREE;
                        r_root->add(aux->f_key, aux->f_item);
                        r_root->right = aux->right; r_root->middle = aux->left;
                        r_root->nqtt = 2 + r_root->left->nqtt + r_root->middle->nqtt + r_root->right->nqtt;
                    }
                }
            } break;
            case THREE: {// Caso o nó visitado seja 3-nó
                if (key < r_root->f_key) {
                    Node<Key,Item>* aux = r_add(r_root->left,key,item);
                    if (aux == r_root->left){// Não cresceu
                        r_root->left = aux; r_root->nqtt++;
                    } else {// Cresceu
                        Node<Key,Item>* n_root = new Node<Key,Item>(r_root->f_key,r_root->f_item);
                        n_root->nodeType = TWO;
                        r_root->nqtt -= aux->nqtt;
                        n_root->nqtt += aux->nqtt + r_root->nqtt;
                        n_root->left = aux; n_root->right = r_root;
                        r_root->f_key = r_root->s_key; r_root->f_item = r_root->s_item;
                        r_root->s_key = Key{}; r_root->s_item = Item{};
                        r_root->nodeType = TWO;
                        r_root->left = r_root->middle; r_root->middle = nullptr;
                        r_root = n_root;
                    }
                } else if (key < r_root->s_key) {
                    Node<Key,Item>* aux = r_add(r_root->middle,key,item);
                    if (aux == r_root->middle){// Não cresceu
                        r_root->middle = aux; r_root->nqtt++;
                    } else {// Cresceu
                        Node<Key,Item>* n_right = new Node<Key,Item>(r_root->s_key,r_root->s_item);
                        n_right->nodeType = TWO;
                        r_root->s_key = Key{}; r_root->s_item = Item{};
                        n_right->right = r_root->right;
                        r_root->right = aux->left;
                        n_right->left = aux->right;
                        n_right->nqtt += n_right->right->nqtt + n_right->left->nqtt;
                        // r_root->nqtt -= n_right->right->nqtt - r_root->right->nqtt;
                        r_root->nqtt = 1 + r_root->left->nqtt + r_root->right->nqtt;
                        aux->left = r_root; aux->right = n_right;
                        r_root->nodeType = TWO; r_root->middle = nullptr;
                        aux->nqtt = 1 + aux->left->nqtt + aux->right->nqtt;
                        r_root = aux;
                    }
                } else {
                    Node<Key,Item>* aux = r_add(r_root->right,key,item);
                    if (aux == r_root->right){// Não cresceu
                        r_root->right = aux; r_root->nqtt++;
                    } else {// Cresceu
                        Node<Key,Item>* n_root = new Node<Key,Item>(r_root->s_key,r_root->s_item);
                        n_root->nodeType = TWO;
                        r_root->nqtt -= aux->nqtt;
                        r_root->s_key = Key{}; r_root->s_item = Item{};
                        r_root->nodeType = TWO;
                        n_root->left = r_root; n_root->right = aux;
                        r_root->right = r_root->middle; r_root->middle = nullptr;
                        n_root->nqtt += n_root->left->nqtt + n_root->right->nqtt;
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

    switch (r_root->nodeType) {
        case LEAF: {
            if (s == 0) return r_root->f_key;
            if (s == 1) return r_root->s_key;
            return Key{};
        } break;
        case TWO: {
            if (s < r_root->left->nqtt)
                return r_select(r_root->left,s);
            else if (s == r_root->left->nqtt)
                return r_root->f_key;
            else
                return r_select(r_root->right, s-(r_root->left->nqtt+1));
        } break;
        case THREE: {
            if (s < r_root->left->nqtt)
                return r_select(r_root->left,s);
            else if (s == r_root->left->nqtt)
                return r_root->f_key;
            else {
                s -= r_root->left->nqtt+1;
                if (s < r_root->middle->nqtt)
                    return r_select(r_root->middle,s);
                else if (s == r_root->middle->nqtt)
                    return r_root->s_key;
                else
                    return r_select(r_root->right, s-(r_root->middle->nqtt+1));
            }
        } break;
    }

    return Key{};
}

#endif