#ifndef TREAP_H
#define TREAP_H

#include "SymbolTable.h"
#include <random>
#include <time.h>
#include <stdio.h>

template <class Key, class Item>
class Treap: public SymbolTable<Key,Item> {

    template <class N_Key, class N_Item>
    class Node {
    public:
        N_Key key;
        N_Item item;
        int nqtt;
        int priority;
        Node<N_Key,N_Item>* right;
        Node<N_Key,N_Item>* left;
        Node<Key,Item>* parent;
        Node(N_Key nkey = N_Key{}, N_Item nitem = N_Item{}) {
            key = nkey; item = nitem;
            right = nullptr; left = nullptr; parent = nullptr; nqtt = 1;
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(1,100);
            // srand(time(NULL));
            priority = dist6(rng);
        }
        void genereteNewPriority() {
            srand(time(NULL)); priority = rand()%10000 + 1;
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
        Treap();
};

template <class Key, class Item>
Treap<Key,Item>::Treap() {
    root = nullptr;
}

template <class Key, class Item>
char* Treap<Key,Item>::show() {
    return (char*)"Epa! Eu sou uma treap. Cuidado, ;)";
}

template <class Key, class Item>
void Treap<Key,Item>::add(Key key, Item item) {
    root = r_add(root, key, item);
}

template <class Key, class Item>
Treap<Key,Item>::Node<Key,Item>* Treap<Key,Item>::r_add(Node<Key,Item>* r_root, Key key, Item item) {
    if (r_root == nullptr)
        return new Node<Key,Item>(key,item);
    
    if (key == r_root->key)
        r_root->item = item;
    else if (key < r_root->key) {
        r_root->nqtt += 1;
        r_root->left = r_add(r_root->left, key, item);
        root->left->parent = r_root;
        while(root->priority == root->left->priority)
            root->left->genereteNewPriority();
        if (((int)root->priority) < ((int)r_root->left->priority))
            r_root = rotateRight(r_root);
    }
    else {
        r_root->nqtt += 1;
        r_root->right = r_add(r_root->right, key, item);
        root->right->parent = r_root;
        while(root->priority == root->right->priority)
            root->right->genereteNewPriority();
        if (((int)r_root->priority) < ((int)r_root->right->priority))
            r_root = rotateLeft(r_root);
    }
    return r_root;
}

template <class Key, class Item>
Treap<Key,Item>::Node<Key,Item>* Treap<Key,Item>::rotateLeft(Node<Key,Item>* m_root) {
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
Treap<Key,Item>::Node<Key,Item>* Treap<Key,Item>::rotateRight(Node<Key,Item>* m_root) {
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
int Treap<Key,Item>::rank(Key key) {
    return r_rank(root,key);
}

template <class Key, class Item>
int Treap<Key,Item>::r_rank(Node<Key,Item>* r_root, Key key) {
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
Item Treap<Key,Item>::value(Key key) {
    return r_value(root,key);
}

template <class Key, class Item>
Item Treap<Key,Item>::r_value(Node<Key,Item>* r_root, Key key) {    
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
Key Treap<Key,Item>::select(int s) {
    if (root == nullptr)
        return Key{};
    return r_select(root, s);
}

template <class Key, class Item>
Key Treap<Key,Item>::r_select(Node<Key,Item>* r_root, int s) {
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