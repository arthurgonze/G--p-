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
unsigned long SymbolTable::cHash(string const &name) {
    const int p = 173; // prime number near ASC2 character table size
    const int m = 1e9 +
                  9; // This is a large number, but still small enough so that we can perform multiplication of two values using 64 bit integers.
    unsigned long hash_value = 0;
    long long p_pow = 1;
    for (char c : name) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value % TABLE_SIZE;
}

/**
 * Constructor
 */
SymbolTable::SymbolTable() {
    headIndex = 0; // A variable to store the first free position in this array
    lexemeArraySize = 0; // Actual size of the lexeme array

    block = new Symbol *[TABLE_SIZE];
    memset(block, 0, sizeof(block) * TABLE_SIZE); //Initialize all positions with nullptr

}

/**
 * If there is no element in the chain then new element is added in front,
 * otherwise through hashing if we reach a chain or, bucket that contains an
 * element then we insert the new element at the beginning of the chain and
 * the rest of the elements is linked to the end of new node.
 * @param symbol
 * @param lexeme
 */
void SymbolTable::cInsert(Symbol *symbol, const char *lexeme) {
    if (lexeme != NULL) {
        unsigned long index = cHash(lexeme);
        symbol->setNextSymbol(block[index]);
        block[index] = symbol;
        if (headIndex + strlen(lexeme) >= (unsigned) lexemeArraySize) {
            while (headIndex + strlen(lexeme) >= (unsigned) lexemeArraySize)
                lexemeArraySize += LEXEME_ARRAY_SIZE;
//            lexemeArraySize += (1 + strlen(lexeme)/LEXEME_ARRAY_SIZE)*LEXEME_ARRAY_SIZE;
            lexemeArray = (char *) realloc(lexemeArray, lexemeArraySize * sizeof(char));
        }
        strcpy(lexemeArray + headIndex, lexeme);
        symbol->setLexemeIndex(headIndex);
        headIndex += (int) strlen(lexeme) + 1;
    }
}

/**
 * Go to certain chain through hashing since we know others will not contain the searched lexeme.
 * Next in that chain do a linear search on all element to see if it is there.
 * @param lexeme
 * @return
 */
Symbol *SymbolTable::cSearch(const char *lexeme) {
    if (lexeme != nullptr) {
        unsigned long pos = cHash(lexeme);
        Symbol *temp = block[pos];
        while (temp != nullptr) {

            if (strcmp(lexemeArray + temp->getLexemeIndex(), lexeme) == 0) {
                temp->setLexeme(lexemeArray + temp->getLexemeIndex());
                return temp;
            }
            temp = temp->getNextSymbol();
        }
        return nullptr;
    }
    return nullptr;
}

void SymbolTable::print() {}

void SymbolTable::beginScope(const char *lexemeScope) {
    previousScopeLexeme = currentScopeLexeme;
    currentScopeLexeme = lexemeScope;
    currentScope += 1;
}

void SymbolTable::endScope() {
    currentScopeLexeme = previousScopeLexeme;
    previousScopeLexeme = "nonLocal";
    currentScope -= 1;
}

void LiteralsTable::cInsert(const char *lexeme) {
    if (cSearch(lexeme) == NULL) {
        SymbolTable::cInsert(new LiteralSymbol(0), lexeme);
    }
}

void LiteralsTable::print() {
    printf("\nTABELA DE SÍMBOLOS: LITERAIS\n");
    printf("-------------------------------------------\n");

    //Check if column names headers needs to be printed
    //If necessary, print selected;
    printf("LEXEMA\t\t");

    printf("\n-------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; ++i) { //Get all the entries

        Symbol *temp = block[i];
        if (temp == nullptr) //If its null, nothing to be done
        {
            continue;
        }

        while (temp != nullptr) {
            //Verify flags to print only requested columns
            printf("%s\t\t", this->lexemeArray + temp->getScope());

            printf("\n");
            temp = temp->getNextSymbol();
        }
    }
    printf("\n");
}

void IdsTable::cInsert(const char *lexeme) {
    if (cSearch(lexeme) == NULL) {
        SymbolTable::cInsert(new IdSymbol(), lexeme);
    }
}

void IdsTable::print() {
    printf("\nTABELA DE SÍMBOLOS: LITERAIS\n");
    printf("-------------------------------------------\n");

    //Check if column names headers needs to be printed
    //If necessary, print selected;
    printf("LEXEMA\t\t");
    printf("\n-------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; ++i) { //Get all the entries

        Symbol *temp = block[i];
        if (temp == nullptr) //If its null, nothing to be done
        {
            continue;
        }

        while (temp != nullptr) {
            //Verify flags to print only requested columns
            printf("%s\t\t", this->lexemeArray + temp->getScope());

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
void ReservedWordsTable::cInsert(int tokenID, const char *lexeme) {
    if (cSearch(lexeme) == NULL) {
        SymbolTable::cInsert(new ReservedTokenSymbol(0, tokenID), lexeme);
    }
}

void ReservedWordsTable::print() {
    printf("\nTABELA DE SÍMBOLOS: PALAVRAS RESERVADAS\n");
    cout << "Lexeme" << "\t" << "Token" << endl;
    cout << "_____________________________" << endl;
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (ReservedTokenSymbol *sym = (ReservedTokenSymbol *) block[i];
             sym != NULL; sym = (ReservedTokenSymbol *) sym->getNextSymbol()) {
            std::cout << lexemeArray + sym->getLexemeIndex() << "\t" << token_id_to_name(sym->getTokenID())
                      << std::endl;
        }
    }
}

/// IntegerTable
/** Verify if the symbol is in the table.
* If not, instantiates a new IntegerSymbol and inserts it.
*/
void NumIntTable::cInsert(const char *lexeme) {
    if (cSearch(lexeme) == NULL) {
        SymbolTable::cInsert(new NumIntSymbol(0), lexeme);
    }
}

void NumIntTable::print() {
    SymbolTable::print();
}

/// RealTable
/** Verify if the symbol is in the table.
* If not, instantiates a new RealSymbol and inserts it.
*/
void NumFloatTable::cInsert(const char *lexeme) {
    if (cSearch(lexeme) == NULL) {
        SymbolTable::cInsert(new NumFloatSymbol(0), lexeme);
    }
}

void NumFloatTable::print() {
    SymbolTable::print();
}

/// VarTable
VarTable::VarTable() {
    currentScope = 0;
    previousScopeLexeme = "nonLocal";
    currentScopeLexeme = "nonLocal";
}

VarTable::~VarTable() {
    currentScope = 0;
    previousScopeLexeme = NULL;
    currentScopeLexeme = NULL;
}

VarSymbol *VarTable::cSearch(const char *lexeme) {
    if (lexeme != NULL) {
        VarSymbol *varSymbol = (VarSymbol *) SymbolTable::cSearch(lexeme);
        while (varSymbol != NULL && varSymbol->getScope() <= currentScope) {
            if ((!strcmp(lexemeArray + varSymbol->getLexemeIndex(), lexeme) &&
                 varSymbol->isScope(currentScopeLexeme)) ||
                varSymbol->isScope(previousScopeLexeme) || varSymbol->isScope("nonLocal")) {
                return varSymbol;
            }
            varSymbol = (VarSymbol *) varSymbol->getNextSymbol();
        }
    }
    return NULL;
}

VarSymbol *VarTable::searchInScope(const char *lexeme, const char *scopeLexeme) {
    if (lexeme != NULL) {
        VarSymbol *varSymbol = (VarSymbol *) SymbolTable::cSearch(lexeme);
        while (varSymbol != NULL) {
            if (!strcmp(lexemeArray + varSymbol->getLexemeIndex(), lexeme) && varSymbol->isScope(scopeLexeme)) {
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
bool VarTable::cInsert(TypeNode *type, const char *lexeme, bool pointer, int arraySize, bool parameter) {
    VarSymbol *varSymbol = cSearch(lexeme);
    if (varSymbol == NULL || varSymbol->getScope() < currentScope || !varSymbol->isScope(currentScopeLexeme)) {
        SymbolTable::cInsert(new VarSymbol(0, currentScope, currentScopeLexeme, type, pointer, arraySize, parameter),
                             lexeme);
        return true;
    }
    return false;
}

void VarTable::print() {
    cout << "\n**********************************************************" << endl;
    cout << "\t\t" << "VARS" << endl;
    cout << "**********************************************************" << endl;
    cout << "Lexeme" << "\t" << "Scope" << "\t" << "Scope Lexeme" << "\t" << "Type" << endl;
    cout << "**********************************************************" << endl;
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (VarSymbol *symbol = (VarSymbol *) block[i];
             symbol != NULL; symbol = (VarSymbol *) symbol->getNextSymbol()) {
            cout << lexemeArray + symbol->getLexemeIndex() << "\t\t" << symbol->getScope() << "\t\t"
                 << symbol->getLexemeScope() << "\t\t"
                 << token_id_to_name(symbol->getType()->getType()/*symbol->getType()->getId()->getToken()*/);

            if (symbol->getType()->getType() == ID)
                cout << " " << symbol->getType()->getTypeLexeme();

            if (symbol->isPointer())
                cout << " POINTER";

            if (symbol->getArraySize() >= 0)
                cout << " ARRAY";

            cout << endl;
        }
    }
    cout << "**********************************************************" << endl;
    cout
            << "####################################################################################################################"
            << endl;
}

/// FunctionTable
FunctionTable::FunctionTable() {
    currentScope = 0;
    previousScopeLexeme = "nonLocal";
    currentScopeLexeme = "nonLocal";
}

FunctionTable::~FunctionTable() {
    currentScope = 0;
    previousScopeLexeme = NULL;
    currentScopeLexeme = NULL;
}

FunctionSymbol *FunctionTable::cSearch(const char *lexeme) {
    if (lexeme != NULL) {
        FunctionSymbol *funcSymbol = (FunctionSymbol *) SymbolTable::cSearch(lexeme);
        while (funcSymbol != NULL && funcSymbol->getScope() <= currentScope) {
            if ((!strcmp(lexemeArray + funcSymbol->getLexemeIndex(), lexeme) &&
                 funcSymbol->isScope(currentScopeLexeme)) ||
                funcSymbol->isScope(previousScopeLexeme) || funcSymbol->isScope("nonLocal")) {
                return funcSymbol;
            }
            funcSymbol = (FunctionSymbol *) funcSymbol->getNextSymbol();
        }
    }
    return NULL;
}

FunctionSymbol *FunctionTable::searchInScope(const char *lexeme, const char *scopeLexeme) {
    if (lexeme != NULL) {
        FunctionSymbol *funcSymbol = (FunctionSymbol *) SymbolTable::cSearch(lexeme);
        while (funcSymbol != NULL) {
            if (!strcmp(lexemeArray + funcSymbol->getLexemeIndex(), lexeme) && funcSymbol->isScope(scopeLexeme)) {
                return funcSymbol;
            }
            funcSymbol = (FunctionSymbol *) funcSymbol->getNextSymbol();
        }
    }
    return NULL;
}

bool FunctionTable::cInsert(TypeNode *returnType, const char *lexeme, FormalListNode *varDecl, bool pointer) {
    FunctionSymbol *funcSymbol = cSearch(lexeme);
    if (funcSymbol == NULL || funcSymbol->getScope() < currentScope || !funcSymbol->isScope(currentScopeLexeme)) {
        SymbolTable::cInsert(new FunctionSymbol(0, currentScope, currentScopeLexeme, returnType, pointer, varDecl),
                             lexeme);
        return true;
    }
    return false;
}

void FunctionTable::print() {
    cout << "\n**********************************************************" << endl;
    cout << "\t\t" << "FUNCTIONS" << endl;
    cout << "**********************************************************" << endl;
    cout << "Lexeme" << "\t" << "Scope" << "\t" << "Scope Lexeme" << "\t" << "Type" << endl;
    cout << "**********************************************************" << endl;
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (FunctionSymbol *symbol = (FunctionSymbol *) block[i];
             symbol != NULL; symbol = (FunctionSymbol *) symbol->getNextSymbol()) {
            cout << lexemeArray + symbol->getLexemeIndex() << "\t\t" << symbol->getScope() << "\t\t"
                 << symbol->getLexemeScope() << "\t\t" << token_id_to_name(symbol->getReturnType()->getId()->getToken())
                 << endl;
        }
    }
    cout << "**********************************************************" << endl;
    cout
            << "####################################################################################################################"
            << endl;
}

/// StructTable
StructTable::StructTable() {
    currentScope = 0;
    previousScopeLexeme = "nonLocal";
    currentScopeLexeme = "nonLocal";
}

StructTable::~StructTable() {
    currentScope = 0;
    previousScopeLexeme = NULL;
    currentScopeLexeme = NULL;
}

StructSymbol *StructTable::cSearch(const char *lexeme) {
    if (lexeme != NULL) {
        StructSymbol *structSymbol = (StructSymbol *) SymbolTable::cSearch(lexeme);
        while (structSymbol != NULL && structSymbol->getScope() <= currentScope) {
            if ((!strcmp(lexemeArray + structSymbol->getLexemeIndex(), lexeme) &&
                 structSymbol->isScope(currentScopeLexeme)) ||
                structSymbol->isScope(previousScopeLexeme) || structSymbol->isScope("nonLocal")) {
                return structSymbol;
            }
            structSymbol = (StructSymbol *) structSymbol->getNextSymbol();
        }
    }
    return NULL;
}

bool StructTable::cInsert(const char *lexeme, VarDeclNode *varDecl) {
    StructSymbol *structSymbol = cSearch(lexeme);
    if (structSymbol == NULL || structSymbol->getScope() < currentScope || !structSymbol->isScope(currentScopeLexeme)) {
        //StructSymbol(const char *lexeme, int scope, const char *lexemeScope, VarDeclNode *varDecl)
        SymbolTable::cInsert(new StructSymbol(0, currentScope, currentScopeLexeme, varDecl), lexeme);
        return true;
    }
    return false;
}

void StructTable::print() {
    cout << "\n*****************************" << endl;
    cout << "\t\t" << "STRUCTS" << endl;
    cout << "*****************************" << endl;
    cout << "Lexeme" << "\t" << "Scope" << "\t" << "Scope Lexeme" << endl;
    cout << "*****************************" << endl;

    for (int i = 0; i < TABLE_SIZE; i++) {
        for (StructSymbol *symbol = (StructSymbol *) block[i];
             symbol != NULL; symbol = (StructSymbol *) symbol->getNextSymbol()) {
            cout << lexemeArray + symbol->getLexemeIndex() << "\t" << symbol->getScope() << "\t"
                 << symbol->getLexemeScope() << endl;
        }
    }
    cout << "*****************************" << endl;
    cout << "##########################################################" << endl;
}