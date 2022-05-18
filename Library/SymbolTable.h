#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

template <class Key, class Item>
class SymbolTable {

    public:
    virtual char* show() = 0;
    virtual void add(Key key, Item item) = 0;
    virtual Item value(Key k) = 0;
    virtual int rank(Key k) = 0;
    virtual Key select(int s) = 0;
};

#endif