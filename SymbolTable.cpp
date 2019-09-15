#include "SymbolTable.h"

// Hashing position is calculated using sum of all character ascii values
// Then performing Modulo operation to go to any bucket from 0 to CHAIN_LENGTH
int SymbolTable::cHash(char* name){
    int idx = 0;
    for(int i = 0; name[i]; ++i){
        idx = idx + name[i];
    }
    return (idx % CHAIN_LENGTH);
}


// If there is no element in the chain then new element is added in front,
// otherwise through hashing if we reach a chain or, bucket that contains an
// element then we insert the new element at the beginning of the chain and
// the rest of the elements is lniked to the end of new node.
void SymbolTable::cInsert(char* name, char* classtype){
    int pos = cHash(name);

    if( block[pos] == NULL ){
        block[pos] = new symbol_info();
        block[pos]->name = name;
        block[pos]->classtype = classtype;
        block[pos]->next = NULL;
    }
    else{
        symbol_info* newNode = new symbol_info();
        newNode->name = name;
        newNode->classtype = classtype;

        // pointer swap
        symbol_info* nextNode = block[pos];
        block[pos] = newNode;
        newNode->next = nextNode;
    }
}

// Go to certain chain through hashing since we know others will not contain the searched value.
// Next in that chain do a linear search on all element to see if it is there.
bool SymbolTable::cSearch(char* name, char* classtype){
    // Implement
    int pos = cHash(name);

    symbol_info* temp = block[pos];

    while( temp != NULL ){
        if( !strcmp( temp->name, name ) && !strcmp( temp->classtype, classtype ) ){
            return true;
        }
        temp = temp->next;
    }

    return false;
}

// Print the symbol table chain wise.
void SymbolTable::showSymbolTable(){
    // Implement
    for(int i = 0; i < CHAIN_LENGTH; ++i){
        printf("%d:", i);

        // Do not modify the head
        symbol_info* temp = block[i];

        while( temp != NULL ){
            printf("->[%s|%s]", temp->name, temp->classtype);
            temp = temp->next;
        }

        printf("\n");
    }
}

