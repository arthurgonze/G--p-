#include "SymbolTable.h"
#pragma pack(1)
// Hashing position is calculated using sum of all character ascii values
// Then performing Modulo operation to go to any bucket from 0 to CHAIN_LENGTH
int SymbolTable::cHash(char *name)
{
    int idx = 0;
    for (int i = 0; name[i]; ++i)
    {
        idx = idx + name[i];
    }
    return (idx%CHAIN_LENGTH);
}

// If there is no element in the chain then new element is added in front,
// otherwise through hashing if we reach a chain or, bucket that contains an
// element then we insert the new element at the beginning of the chain and
// the rest of the elements is lniked to the end of new node.
void SymbolTable::cInsert(char *name, char *classtype)
{
    if (!cSearch(name, classtype))
    {
        int pos = cHash(name);// symbol position
        if (block[pos]==NULL) // if there is nothing there
        {
            block[pos] = new symbol_info(); // create a new symbol info
            block[pos]->name = name; // set name
            block[pos]->classtype = classtype; // set classtype
            block[pos]->next = NULL; // set next to null because is the first insertion on that list
        }
        else
        {
            symbol_info *newNode = new symbol_info(); // create a new symbol
            newNode->name = name; // set name
            newNode->classtype = classtype; // set classtype

            // pointer swap
            symbol_info *nextNode = block[pos]; // set the next node to be the previous one
            block[pos] = newNode; // insert the not into the position
            newNode->next = nextNode; // set the next node pointer
        }
    }
    else
    {
        printf("Simbolo ja presente na tabela\n");
    }
}

// Go to certain chain through hashing since we know others will not contain the searched value.
// Next in that chain do a linear search on all element to see if it is there.
bool SymbolTable::cSearch(char *name, char *classtype)
{
    int pos = cHash(name);

    symbol_info *temp = block[pos];

    while (temp!=NULL)
    {
        if (!strcmp(temp->name, name) && !strcmp(temp->classtype, classtype))
        {
            return true;
        }
        temp = temp->next;
    }

    return false;
}

// Print the symbol table chain wise.
void SymbolTable::showSymbolTable()
{
    int cont = 0;
    // Implement
    for (int i = 0; i < CHAIN_LENGTH ; ++i)
    {
        printf("%d:", i);

        // Do not modify the head
        symbol_info *temp = block[i];

        while (temp!=NULL)
        {
            printf("->[%s|%s]", temp->name, temp->classtype);
            temp = temp->next;
            cont++;
        }
        //printf("->%d", cont);
        printf("\n");
        cont = 0;
    }
}

