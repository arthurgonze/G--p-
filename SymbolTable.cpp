//
// Created by caveira on 9/13/19.
//


#include "SymbolTable.h"

template <typename HashedObj>
bool SymbolTable<HashedObj>::contains(const HashedObj &x)const
{
    auto &whichList = theLists[myhash(x)];
    return find(begin(whichList), end(whichList), x) != end(whichList);
}
template <typename HashedObj>
bool SymbolTable<HashedObj>::insert(const HashedObj &x)
{
    auto &whichList = theLists[myhash(x)];
    if(find(begin(whichList), end(whichList), x) != end(whichList))
    {
        return false;
    }
    whichList.push_back(x);

    return true;
}

template <typename HashedObj>
size_t SymbolTable<HashedObj>::myhash(const HashedObj &x)const
{
    static ::hash<HashedObj> hf;
    return hf(x) % theLists.size();
}

/**
 * A hash routine for string objects
 * @param key
 * @param tableSize
 * @return
 */
template <typename HashedObj>
unsigned int SymbolTable<HashedObj>::hash(const string &key, int tableSize)
{
    unsigned  int hashVal = 0;

    for(char ch : key)
    {
        hashVal = 37 * hashVal + ch;
    }

    return hashVal % tableSize;
}
