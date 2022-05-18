#ifndef SYMBOLTABLEFACTORY_H
#define SYMBOLTABLEFACTORY_H

#include "SymbolTable.h"
#include "DinamicArray.h"
#include "BinarySearchTree.h"
#include "Treap.h"
#include "RedBlackTree.h"
#include "TwoThreeTree.h"
#include <stdio.h>

template <class Key, class Item>
class SymbolTableFactory {

    public:
    static SymbolTable<Key,Item>* createST(char* type);
    static constexpr unsigned int str2int(const char* str, int h=0);
};

template <class Key, class Item>
SymbolTable<Key,Item>* SymbolTableFactory<Key,Item>::createST(char* type) {

    switch(str2int(type)) {
        case str2int("VO"): return new DinamicArray<Key,Item>(); break;
        case str2int("ABB"): return new BinarySearchTree<Key,Item>(); break;
        case str2int("TR"): return new Treap<Key,Item>(); break;
        case str2int("ARN"): return new RedBlackTree<Key,Item>(); break;
        case str2int("A23"): return new TwoThreeTree<Key,Item>(); break;
        default: return nullptr;
    }

    return nullptr;
}

// Código hash de transformação string->int
template <class Key, class Item>
constexpr unsigned int SymbolTableFactory<Key,Item>::str2int(const char* str, int h) {
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

#endif