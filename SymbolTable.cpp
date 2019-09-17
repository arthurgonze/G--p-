#include "SymbolTable.h"

/**
 *
 * @param name
 * @return
 */
int SymbolTable::cHash(char *name)
{
    unsigned long hash = 5381;
    int c;

    while (c = *name++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (hash%TABLE_SIZE);
}

/**
 * Constructor
 */
SymbolTable::SymbolTable() {
    headIndex = 0; // A variable to store the first free position in this array
    lexemeArraySize = 0; // Actual size of the lexeme array
}


/**
 * If there is no element in the chain then new element is added in front,
 * otherwise through hashing if we reach a chain or, bucket that contains an
 * element then we insert the new element at the beginning of the chain and
 * the rest of the elements is linked to the end of new node.
 * @param token
 * @param lexeme
 */
void SymbolTable::cInsert(int token, char *lexeme)
{
    int pos = cHash(lexeme);

    if (block[pos]==NULL)
    {
        block[pos] = new symbol_info();
        block[pos]->token = token;

        block[pos]->pos = headIndex;

        int lexemeSize = strlen(lexeme);
        if (lexemeSize + headIndex >= lexemeArraySize) // Check if the string will exceed the array free space
        {
            lexemeArraySize += LEXEME_ARRAY_SIZE;
            lexemeArray = (char *) realloc(lexemeArray, lexemeArraySize);
        }
        strcpy(lexemeArray + headIndex, lexeme);
        headIndex += lexemeSize + 1; // +1 cause of \0
        block[pos]->next = NULL;
    }
    else
    {
        symbol_info *newNode = new symbol_info();
        newNode->token = token;
        newNode->pos = headIndex;

        int lexemeSize = strlen(lexeme);
        if (lexemeSize + headIndex >= lexemeArraySize) // Check if the string will exceed the array free space
        {
            lexemeArraySize += LEXEME_ARRAY_SIZE;
            lexemeArray = (char *) realloc(lexemeArray, lexemeArraySize);
        }
        strcpy(lexemeArray + headIndex, lexeme);
        headIndex += lexemeSize + 1;// +1 cause of \0

        // pointer swap
        symbol_info *nextNode = block[pos]; // set the next node to be the previous one
        block[pos] = newNode; // insert the node into the position
        newNode->next = nextNode; // set the next node pointer
    }
}

/**
 * Go to certain chain through hashing since we know others will not contain the searched value.
 * Next in that chain do a linear search on all element to see if it is there.
 * @param lexeme
 * @return
 */
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

