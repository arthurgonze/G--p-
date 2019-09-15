#include "SymbolTable.h"

SymbolTable::SymbolTable(int size)
{
    tableSize = size;
    vector<list<newToken>> theLists; // The array of lists
}

/**
 * Verify if the table contains an item
 * @param x
 * @return
 */
bool SymbolTable::contains(const newToken &x)const
{
    auto &whichList = theLists[hash(x.tokenName)];
    return find(begin(whichList), end(whichList), x) != end(whichList);
}

/**
 * Add an item to the table
 * @param x
 * @return
 */
bool SymbolTable::insert(const char* tokenName, const char* lexeme)
{
    newToken x;
    x.tokenName = const_cast<char *>(tokenName);
    x.attributeValue = const_cast<char *>(lexeme);

    if(contains(x)) // search item, no duplicates allowed
    {
        return false;
    }
    theLists[hash(x.tokenName)].push_back(x); // add item

    return true;
}

/**
 * A hash routine for string objects
 * @param x
 * @return
 */
size_t SymbolTable::hash(const string tokenName)const
{
    unsigned  int hashVal = 0;

    for(char ch : tokenName)
    {
        hashVal = 37 * hashVal + ch;
    }

    return hashVal % tableSize;
}

