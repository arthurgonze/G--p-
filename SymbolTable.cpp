#include "SymbolTable.h"
// Hashing position is calculated using sum of all character ascii values
// Then performing Modulo operation to go to any bucket from 0 to TABLE_SIZE
int SymbolTable::cHash(char *name)
{
    unsigned long hash = 5381;
    int c;

    while (c = *name++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (hash%TABLE_SIZE);

//    int idx = 0;
//    for (int i = 0; name[i]; ++i)
//    {
//        idx += name[i];
//    }
//    return (idx%TABLE_SIZE);
}

SymbolTable::SymbolTable() {
    headIndex = 0; // A variable to store the first free position in this array
    lexemeArraySize = 0; // Actual size of the lexeme array
}

// If there is no element in the chain then new element is added in front,
// otherwise through hashing if we reach a chain or, bucket that contains an
// element then we insert the new element at the beginning of the chain and
// the rest of the elements is lniked to the end of new node.
void SymbolTable::cInsert(int token, char *lexeme)
{
    int pos = cHash(lexeme);// symbol position

    if (block[pos]==NULL) // if there is nothing there
    {
        block[pos] = new symbol_info(); // create a new symbol info
        block[pos]->token = token; // set token id

        // Add the lexeme to the lexeme array, char by char, and put a \0 EOS signal in the end of each string
        block[pos]->pos = headIndex; // set the position of the lexeme in the array

        int lexemeSize = strlen(lexeme);
        if (lexemeSize + headIndex >= lexemeArraySize) // +1 cause of \0
        {
            lexemeArraySize += LEXEME_ARRAY_SIZE;
            lexemeArray = (char *) realloc(lexemeArray, lexemeArraySize);
        }
        strcpy(lexemeArray + headIndex, lexeme);
        headIndex += lexemeSize + 1;
        block[pos]->next = NULL; // set next to null because is the first insertion on that list
    }
    else
    {
        symbol_info *newNode = new symbol_info(); // create a new symbol
        newNode->token = token; // set token id

        // Add the lexeme to the lexeme array, char by char, and put a \0 EOS signal in the end of each string
        newNode->pos = headIndex; // set the position of the lexeme in the array

        int lexemeSize = strlen(lexeme);
        if (lexemeSize + headIndex >= lexemeArraySize) // +1 cause of \0
        {
            lexemeArraySize += LEXEME_ARRAY_SIZE;
            lexemeArray = (char *) realloc(lexemeArray, lexemeArraySize);
        }
        strcpy(lexemeArray + headIndex, lexeme);
        headIndex += lexemeSize + 1;

        // pointer swap
        symbol_info *nextNode = block[pos]; // set the next node to be the previous one
        block[pos] = newNode; // insert the not into the position
        newNode->next = nextNode; // set the next node pointer
    }
}

// Go to certain chain through hashing since we know others will not contain the searched value.
// Next in that chain do a linear search on all element to see if it is there.
int SymbolTable::cSearch(char *lexeme)
{
    int pos = cHash(lexeme);

    symbol_info *temp = block[pos];

    while (temp!=NULL)
    {
        if (strcmp(lexemeArray+temp->pos, lexeme)==0)
        {
            return temp->token;
        }
        temp = temp->next;
    }
    return -1;
}

