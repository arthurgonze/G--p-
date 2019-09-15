//
// Created by caveira on 9/13/19.
//

#ifndef COMPILADOR_2019_3_SYMBOLTABLE_H
#define COMPILADOR_2019_3_SYMBOLTABLE_H

#include <cstdlib>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

template <typename HashedObj>
     class SymbolTable{
public:
    explicit  SymbolTable(int size = 101);

    bool contains(const HashedObj &x)const;
    bool insert(const HashedObj &x);

private:
    vector<list<HashedObj>> theLists; // The array of lists
    int currentSize;

    size_t myhash(const HashedObj &x)const;
    unsigned int hash(const string &key, int tableSize);
};

#endif //COMPILADOR_2019_3_SYMBOLTABLE_H
