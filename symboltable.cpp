#include "symboltable.h"
#include "analyzer.h"

/**
 * Polynomial Rolling Hash Function
 * p-> It is reasonable to make p a prime number roughly equal to the number of characters in the input alphabet
 * m-> m should be a large number, since the probability of two random strings colliding is about 1/m
 * Precomputing the powers of p might give a performance boost.
 * @param name is the lexeme to be hashed
 * @return
 */
unsigned long SymbolTable::cHash(string const &name)
{
    const int p = 173; // prime number near ASC2 character table size
    const int m = 1e9 + 9; // This is a large number, but still small enough so that we can perform multiplication of two values using 64 bit integers.
    unsigned long hash_value = 0;
    long long p_pow = 1;
    for (char c : name)
    {
        hash_value = (hash_value + (c - 'a' + 1)*p_pow)%m;
        p_pow = (p_pow*p)%m;
    }
    return hash_value%TABLE_SIZE;
}

/**
 * Constructor
 */
SymbolTable::SymbolTable()
{
    headIndex = 0; // A variable to store the first free position in this array
    lexemeArraySize = 0; // Actual size of the lexeme array

    block = new Symbol *[TABLE_SIZE];
    memset(block, 0, sizeof(block)*TABLE_SIZE); //Initialize all positions with nullptr

}

/**
*	Recursive function to perform the insertion
*	@param root Root node to vist
*	@param token Token ID
*	@param lexeme Lexeme string
*/
Symbol *SymbolTable::auxInsert(Symbol *root, int token, const char *lexeme)
{

    if (root==nullptr)
    {
        // TODO Create the new node with info
        Symbol *newNode = new Symbol(NULL, NULL, NULL);
//        newNode->token = token;
//        newNode->pos = headIndex;
//        newNode->next = nullptr;

        //Add the lexeme to array
        int lexemeSize = strlen(lexeme);
        if (lexemeSize + headIndex >= lexemeArraySize) // Check if the string will exceed the array free space
        {
            lexemeArraySize += LEXEME_ARRAY_SIZE;
            lexemeArray = (char *) realloc(lexemeArray, lexemeArraySize);
        }
        strcpy(lexemeArray + headIndex, lexeme);
        headIndex += lexemeSize + 1; // +1 cause of \0

        return newNode;
    }

    // TODO If there's a valid node, check if its not a duplicate
//    if (!(root->token==token && strcmp(lexemeArray + root->pos, lexeme)==0))
//    {
//        root->next = auxInsert(root->next, token, lexeme);
//    }

    return root;
}

/**
 * If there is no element in the chain then new element is added in front,
 * otherwise through hashing if we reach a chain or, bucket that contains an
 * element then we insert the new element at the beginning of the chain and
 * the rest of the elements is linked to the end of new node.
 * @param token
 * @param lexeme
 */
//void SymbolTable::cInsert(int token, const char *lexeme)
//{
//    unsigned long pos = cHash(lexeme);
//    block[pos] = auxInsert(block[pos], token, lexeme);
//}

/** Given a Symbol inserts it in the hash table and add the lexeme
* to the lexemes array.
* Reallocs the lexemes array if needed.
*/
void SymbolTable::cInsert(Symbol *symbol, const char *lexeme)
{
    if (lexeme!=NULL)
    {
        unsigned int index = cHash(lexeme);
        symbol->setNextSymbol(block[index]);
        // TODO block[index] = auxInsert(block[index], lexeme);
//        symbol->setNextSymbol(newBlock[index]);
//        newBlock[index] = symbol;
//        if (headIndex + strlen(lexeme) >= lexemeArraySize)
//        {
//            lexemeArraySize += (1 + strlen(lexeme)/LEXEME_ARRAY_SIZE)*LEXEME_ARRAY_SIZE;
//            lexemeArray = (char *) realloc(lexemeArray, lexemeArraySize*sizeof(char));
//        }
//        strcpy(&lexemeArray[headIndex], lexeme);
//        lexemeArraySize += strlen(lexeme) + 1;
    }
}

/**
 * Go to certain chain through hashing since we know others will not contain the searched lexeme.
 * Next in that chain do a linear search on all element to see if it is there.
 * @param lexeme
 * @return
 */

/** Verify if a symbol is in the table.
* If true, returns the Symbol, else returns NULL.
*/
Symbol *SymbolTable::cSearch(const char *lexeme)
{
    if (lexeme!=nullptr)
    {
        unsigned long pos = cHash(lexeme);
        Symbol *temp = block[pos];
        while (temp!=nullptr)
        {
            if (strcmp(temp->getLexeme(), lexeme)==0)
            {
                return temp;
            }
            temp = temp->getNextSymbol();
        }
        return nullptr;
    }
    return nullptr;
}

void SymbolTable::print() {}

void SymbolTable::beginScope(const char *lexemeScope)
{
    previousScopeLexeme = currentScopeLexeme;
    currentScopeLexeme = lexemeScope;
    currentScope += 1;
}

void SymbolTable::endScope()
{
    currentScopeLexeme = previousScopeLexeme;
    previousScopeLexeme = "nonLocal";
    currentScope -= 1;
}

void LiteralsTable::cInsert(const char *lexeme)
{
    if (cSearch(lexeme)==NULL)
    {
        SymbolTable::cInsert(new LiteralSymbol(&lexemeArray[headIndex]), lexeme);
    }
}

void LiteralsTable::print()
{
    printf("\nTABELA DE SÍMBOLOS: LITERAIS\n");
    printf("-------------------------------------------\n");

    //Check if column names headers needs to be printed
    //If necessary, print selected;
    printf("LEXEMA\t\t");

    printf("\n-------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; ++i)
    { //Get all the entries

        Symbol *temp = block[i];
        if (temp==nullptr) //If its null, nothing to be done
        {
            continue;
        }

        while (temp!=nullptr)
        {
            //Verify flags to print only requested columns
            printf("%s\t\t", this->lexemeArray + temp->getScope());// TODO temp->getScope() era temp->pos

            printf("\n");
            temp = temp->getNextSymbol();
        }
    }
    printf("\n");
}

void IdsTable::cInsert(const char *lexeme)
{
    if (cSearch(lexeme)==NULL)
    {
        SymbolTable::cInsert(new IdSymbol(&lexemeArray[headIndex]), lexeme);
    }
}

void IdsTable::print()
{
    printf("\nTABELA DE SÍMBOLOS: LITERAIS\n");
    printf("-------------------------------------------\n");

    //Check if column names headers needs to be printed
    //If necessary, print selected;
    printf("LEXEMA\t\t");
    printf("\n-------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; ++i)
    { //Get all the entries

        Symbol *temp = block[i];
        if (temp==nullptr) //If its null, nothing to be done
        {
            continue;
        }

        while (temp!=nullptr)
        {
            //Verify flags to print only requested columns
            printf("%s\t\t", this->lexemeArray + temp->getScope());// TODO temp->getScope() era temp->pos

            printf("\n");
            temp = temp->getNextSymbol();
        }
    }
    printf("\n");
}

/// ReservedTable
/** Verify if the symbol is in the table.
* If not, instantiates a new ReservedSymbol and inserts it.
*/
void ReservedWordsTable::cInsert(int tokenID, const char *lexeme)
{
    if (cSearch(lexeme)==NULL)
    {
        SymbolTable::cInsert(new ReservedTokenSymbol(lexeme, tokenID), lexeme);
    }
}

void ReservedWordsTable::print()
{
    printf("\nTABELA DE SÍMBOLOS: PALAVRAS RESERVADAS\n");
    cout << "Lexeme" << "\t" << "Token number" << endl;
    cout << "_____________________________" << endl;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        for (ReservedTokenSymbol *sym = (ReservedTokenSymbol *) block[i]; sym!=NULL; sym = (ReservedTokenSymbol *) sym->getNextSymbol())
        {
            std::cout << sym->getLexeme() << "\t" << sym->getTokenID() << std::endl;
        }
    }
}

/// IntegerTable
/** Verify if the symbol is in the table.
* If not, instantiates a new IntegerSymbol and inserts it.
*/
void NumIntTable::cInsert(const char *lexeme)
{
    if (cSearch(lexeme)==NULL)
    {
        SymbolTable::cInsert(new NumIntSymbol(&lexemeArray[headIndex]), lexeme);
    }
}
void NumIntTable::print()
{
    SymbolTable::print();
}

/// RealTable
/** Verify if the symbol is in the table.
* If not, instantiates a new RealSymbol and inserts it.
*/
void NumFloatTable::cInsert(const char *lexeme)
{
    if (cSearch(lexeme)==NULL)
    {
        SymbolTable::cInsert(new NumFloatSymbol(&lexemeArray[headIndex]), lexeme);
    }
}
void NumFloatTable::print()
{
    SymbolTable::print();
}

/// VarTable
VarTable::VarTable()
{
    currentScope = 0;
    previousScopeLexeme = "nonLocal";
    currentScopeLexeme = "nonLocal";
}

VarTable::~VarTable()
{
    currentScope = 0;
    previousScopeLexeme = NULL;
    currentScopeLexeme = NULL;
}

VarSymbol *VarTable::search(const char *lexeme)
{
    if (lexeme!=NULL)
    {
        VarSymbol *varSymbol = (VarSymbol *) SymbolTable::cSearch(lexeme);
        while (varSymbol!=NULL && varSymbol->getScope() <= currentScope)
        {
            if ((varSymbol->getLexeme()==lexeme && varSymbol->isScope(currentScopeLexeme)) ||
                varSymbol->isScope(previousScopeLexeme) || varSymbol->isScope("nonLocal"))
            {
                return varSymbol;
            }
            varSymbol = (VarSymbol *) varSymbol->getNextSymbol();
        }
    }
    return NULL;
}

VarSymbol *VarTable::searchInScope(const char *lexeme, const char *scopeLexeme)
{
    if (lexeme!=NULL)
    {
        VarSymbol *varSymbol = (VarSymbol *) SymbolTable::cSearch(lexeme);
        while (varSymbol!=NULL)
        {
            if (varSymbol->getLexeme()==lexeme && varSymbol->isScope(scopeLexeme))
            {
                return varSymbol;
            }
            varSymbol = (VarSymbol *) varSymbol->getNextSymbol();
        }
    }
    return NULL;
}

/**
 * @return true if inserted and false if not
 */
bool VarTable::insert(TypeNode *type, const char *lexeme, bool pointer, int arraySize, bool parameter)
{
    VarSymbol *varSymbol = search(lexeme);
    if (varSymbol==NULL || varSymbol->getScope() < currentScope || !varSymbol->isScope(currentScopeLexeme))
    {
        // VarSymbol(const char *lexeme, int scope, const char *lexemeScope, TypeNode *type, bool pointer, int arraySize, bool parameter);
        SymbolTable::cInsert(new VarSymbol(lexeme, currentScope, currentScopeLexeme, type, pointer, arraySize, parameter), lexeme);
        return true;
    }
    return false;
}

void VarTable::print()
{
    cout << "VARS" << std::endl;
    cout << "Lexeme" << "\t" << "Scope" << "\t" << "Scope Lexeme" << std::endl;
    std::cout << "_____________________________" << std::endl;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        for (VarSymbol *symbol = (VarSymbol *) block[i]; symbol!=NULL; symbol = (VarSymbol *) symbol->getNextSymbol())
        {
            std::cout << symbol->getLexeme() << "\t" << symbol->getScope() << "\t" << symbol->getLexemeScope() << std::endl;
        }
    }
}

/// FunctionTable
FunctionTable::FunctionTable()
{
    currentScope = 0;
    previousScopeLexeme = "nonLocal";
    currentScopeLexeme = "nonLocal";
}

FunctionTable::~FunctionTable()
{
    currentScope = 0;
    previousScopeLexeme = NULL;
    currentScopeLexeme = NULL;
}

FunctionSymbol *FunctionTable::search(const char *lexeme)
{
    if (lexeme!=NULL)
    {
        FunctionSymbol *funcSymbol = (FunctionSymbol *) SymbolTable::cSearch(lexeme);
        while (funcSymbol!=NULL && funcSymbol->getScope() <= currentScope)
        {
            if ((funcSymbol->getLexeme()==lexeme && funcSymbol->isScope(currentScopeLexeme)) ||
                funcSymbol->isScope(previousScopeLexeme) || funcSymbol->isScope("nonLocal"))
            {
                return funcSymbol;
            }
            funcSymbol = (FunctionSymbol *) funcSymbol->getNextSymbol();
        }
    }
    return NULL;
}

FunctionSymbol *FunctionTable::searchInScope(const char *lexeme, const char *scopeLexeme)
{
    if (lexeme!=NULL)
    {
        FunctionSymbol *funcSymbol = (FunctionSymbol *) SymbolTable::cSearch(lexeme);
        while (funcSymbol!=NULL)
        {
            if (funcSymbol->getLexeme()==lexeme && funcSymbol->isScope(scopeLexeme))
            {
                return funcSymbol;
            }
            funcSymbol = (FunctionSymbol *) funcSymbol->getNextSymbol();
        }
    }
    return NULL;
}

bool FunctionTable::insert(TypeNode *returnType, const char *lexeme, FormalListNode *varDecl, bool pointer)
{
    FunctionSymbol *funcSymbol = search(lexeme);
    if (funcSymbol==NULL || funcSymbol->getScope() < currentScope || !funcSymbol->isScope(currentScopeLexeme))
    {
        //FunctionSymbol(const char *lexeme, int scope, const char *lexemeScope, TypeNode *returnType, bool pointer, FormalListNode *varDecl);
        SymbolTable::cInsert(new FunctionSymbol(lexeme, currentScope, currentScopeLexeme, returnType, pointer, varDecl), lexeme);
        return true;
    }
    return false;
}

void FunctionTable::print()
{
    cout << "FUNCTIONS" << std::endl;
    std::cout << "Lexeme" << "\t" << "Scope" << "\t" << "Scope Lexeme" << std::endl;
    std::cout << "_____________________________" << std::endl;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        for (FunctionSymbol *symbol = (FunctionSymbol *) block[i]; symbol!=NULL; symbol = (FunctionSymbol *) symbol->getNextSymbol())
        {
            std::cout << symbol->getLexeme() << "\t" << symbol->getScope() << "\t" << symbol->getLexemeScope() << std::endl;
        }
    }
}

/// StructTable
StructTable::StructTable()
{
    currentScope = 0;
    previousScopeLexeme = "nonLocal";
    currentScopeLexeme = "nonLocal";
}

StructTable::~StructTable()
{
    currentScope = 0;
    previousScopeLexeme = NULL;
    currentScopeLexeme = NULL;
}

StructSymbol *StructTable::search(const char *lexeme)
{
    if (lexeme!=NULL)
    {
        StructSymbol *structSymbol = (StructSymbol *) SymbolTable::cSearch(lexeme);
        while (structSymbol!=NULL && structSymbol->getScope() <= currentScope)
        {
            if ((structSymbol->getLexeme()==lexeme && structSymbol->isScope(currentScopeLexeme)) ||
                structSymbol->isScope(previousScopeLexeme) || structSymbol->isScope("nonLocal"))
            {
                return structSymbol;
            }
            structSymbol = (StructSymbol *) structSymbol->getNextSymbol();
        }
    }
    return NULL;
}

bool StructTable::insert(const char *lexeme, VarDeclNode *varDecl)
{
    StructSymbol *structSymbol = search(lexeme);
    if (structSymbol==NULL || structSymbol->getScope() < currentScope || !structSymbol->isScope(currentScopeLexeme))
    {
        //StructSymbol(const char *lexeme, int scope, const char *lexemeScope, VarDeclNode *varDecl)
        SymbolTable::cInsert(new StructSymbol(lexeme, currentScope, currentScopeLexeme, varDecl), lexeme);
        return true;
    }
    return false;
}

void StructTable::print()
{
    std::cout << "STRUCTS" << std::endl;
    std::cout << "_____________________________" << std::endl;
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        for (StructSymbol *symbol = (StructSymbol *) block[i]; symbol!=NULL; symbol = (StructSymbol *) symbol->getNextSymbol())
        {
            std::cout << symbol->getLexeme() << "\t" << symbol->getScope() << "\t" << symbol->getLexemeScope() << std::endl;
        }
    }
}

