#ifndef DINAMICARRAY_H
#define DINAMICARRAY_H

#include "SymbolTable.h"

template <class Key, class Item>
class DinamicArray: public SymbolTable<Key,Item>{
    
    template <class N_Key, class N_Item>
    class Node {
    public:
        N_Item item;
        N_Key key;
        Node(N_Key nkey = N_Key{}, N_Item nitem = N_Item{}) {
            key = nkey; item = nitem;
        }
    };
    
    private:
        int size;
        int n;
        Node<Key,Item>** array;
        void resize(int nn);

    public:
        // Métodos herdados
        char* show();
        void add(Key key, Item item);
        Item value(Key key);
        int rank(Key key);
        Key select(int s);
        // Métodos próprios
        DinamicArray();
        ~DinamicArray();
        bool isEmpty();
};

template <class Key, class Item>
DinamicArray<Key,Item>::DinamicArray() {
    n = 0; size = 1;
    array = new Node<Key, Item>*[1] {};
}

template <class Key, class Item>
DinamicArray<Key,Item>::~DinamicArray() {
    if (array != nullptr)
        delete[] array;
}

template <class Key, class Item>
void DinamicArray<Key,Item>::resize(int nn) {
    Node<Key,Item>** new_arr = new Node<Key,Item>*[nn];
    for (int i = 0; i < size; i++)
        new_arr[i] = array[i];
    delete[] array;
    array = new_arr;
    size = nn;
}

template <class Key, class Item>
char* DinamicArray<Key,Item>::show() {
    char* s = (char*)"Oi, eu sou uma array dinâmica! B)";
    return s;
}

template <class Key, class Item>
void DinamicArray<Key,Item>::add(Key key, Item item) {

    if (n == size)
        resize(2*size);

    int i = rank(key);
    if (i != n)
        if (array[i]->key == key) {
            array[i]->item = item;
            return;
        }
    
    for (int j = n; j > i; j--)
        array[j] = array[j-1];
    array[i] = new Node<Key,Item>(key,item);
    n++;
}

template <class Key, class Item>
Item DinamicArray<Key,Item>::value(Key k)  {
    if (isEmpty())
        return Item{};

    int _rank = rank(k);
    if (_rank == n)
        return Item{};
    else
        return array[_rank]->item;
}

template <class Key, class Item>
int DinamicArray<Key,Item>::rank(Key k) {
    int a = 0, b = n-1;
    while (a <= b) {
        int m = (a+b)/2;
        if (array[m]->key > k)
            b = m-1;
        else if (array[m]->key < k)
            a = m+1;
        else
            return m;
    }
    return a;
}
template <class Key, class Item>
Key DinamicArray<Key,Item>::select(int s) {
    if (s == n)
        return Key{};
    else
        return array[s]->key;
}

template <class Key, class Item>
bool DinamicArray<Key,Item>::isEmpty() {
    return (n==0 && size==1);
}

#endif