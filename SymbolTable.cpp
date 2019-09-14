//
// Created by caveira on 9/13/19.
//

#include "SymbolTable.h"

explicit  SymbolTable::SymbolTable(int size = 101)
{

}

bool SymbolTable::contains(const HashedObj &x)const
{
    auto &whichList = theLists[myhash(x)];
    return find(begin(whichList), end(whichList), x) != end(whichList);
}

bool SymbolTable::insert(const HashedObj &x)
{
    auto &whichList = theLists[myhash(x)];
    if(find(begin(whichList), end(whichList), x) != end(whichList))
    {
        return false;
    }
    whichList.push_back(x);

    return true;
}


size_t SymbolTable::myhash(const HashedObj &x)const
{
    static hash<HashedObj> hf;
    return hf(x) % theLists.size();
}

/**
 * A hash routine for string objects
 * @param key
 * @param tableSize
 * @return
 */
unsigned int SymbolTable::hash(const string &key, int tableSize)
{
    unsigned  int hashVal = 0;

    for(char ch : key)
    {
        hashVal = 37 * hashVal + ch;
    }

    return hashVal % tableSize;
}
