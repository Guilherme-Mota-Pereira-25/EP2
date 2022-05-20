#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "SymbolTable.h"

template <class Key, class Item>
class BinarySearchTree: public SymbolTable<Key,Item>{

    template<class N_Key, class N_Item>
    class Node {
    public:
        N_Key key;
        N_Item item;
        int nqtt;
        Node<N_Key,N_Item>* left;
        Node<N_Key,N_Item>* right;
        Node(N_Key nkey = N_Key{}, N_Item nitem = N_Item{}) {
            key = nkey; item = nitem;
            left = nullptr; right = nullptr; nqtt = 1;
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
        BinarySearchTree();
};

template <class Key, class Item>
BinarySearchTree<Key,Item>::BinarySearchTree() {
    root = nullptr;
}

template <class Key, class Item>
char* BinarySearchTree<Key,Item>::show() {
    char* s = (char*)"Opa! Eu sou uma árvore binária de busca! :)";
    return s;
}

template <class Key, class Item>
void BinarySearchTree<Key,Item>::add(Key key, Item item) {
    root = r_add(root,key,item);
}

template <class Key, class Item>
BinarySearchTree<Key,Item>::Node<Key,Item>* BinarySearchTree<Key,Item>::r_add(Node<Key,Item>* r_root, Key key, Item item) {
    if (r_root == nullptr)
        return new Node<Key,Item>(key,item);
    
    if (r_root->key == key)
        r_root->item = item;
    else if (key > r_root->key) {
        r_root->right = r_add(r_root->right,key,item);
        r_root->nqtt += 1;
    }
    else {
        r_root->left = r_add(r_root->left,key,item);
        r_root->nqtt += 1;
    }

    return r_root;
}

template <class Key, class Item>
Item BinarySearchTree<Key,Item>::value(Key key) {    
    return r_value(root,key);
}

template <class Key, class Item>
Item BinarySearchTree<Key,Item>::r_value(Node<Key,Item>* r_root, Key key) {    
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
int BinarySearchTree<Key,Item>::rank(Key key) {
    return r_rank(root,key);
}

template <class Key, class Item>
int BinarySearchTree<Key,Item>::r_rank(Node<Key,Item>* r_root, Key key) {
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
Key BinarySearchTree<Key,Item>::select(int s) {
    if (root == nullptr)
        return Key{};
    return r_select(root, s);
}

template <class Key, class Item>
Key BinarySearchTree<Key,Item>::r_select(Node<Key,Item>* r_root, int s) {
    if (r_root == nullptr)
        return Key{};
    
    if (r_root->left != nullptr) {
        if (s == r_root->left->nqtt)
            return r_root->key;
        else if (s < r_root->left->nqtt)
            return r_select(r_root->left, s);
        else
            return r_select(r_root->right, s-(r_root->left->nqtt+1));
    } else if (r_root->right != nullptr) {
        if (s == 0)
            return r_root->key;
        else
            return r_select(r_root->right, s-1);
    } else {
        if (s == 0)
            return r_root->key;
        else
            return Key{};
    }
}

#endif