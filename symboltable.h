#include <cstring>
#include <string>

#ifndef COMPILADOR_2019_3_SYMBOLTABLE_H
#define COMPILADOR_2019_3_SYMBOLTABLE_H


using namespace std;

#define TABLE_SIZE 101 // An arbitrary prime number that showed good performance
#define LEXEME_ARRAY_SIZE 4096 // A memory block size

// Two columns of the symbol table with token ID and lexeme position in the lexeme array
struct symbol_info
{
    int token; //token id
    int pos; // lexem position in the lexemeArray
    struct symbol_info *next; // pointer to the next symbol

	~symbol_info();
};

class SymbolTable
{
public:
    void cInsert(int token, char const *lexeme);
    int cSearch(char *lexeme);
	symbol_info** block; // Table
    char *lexemeArray = new char[LEXEME_ARRAY_SIZE]; // An array to allocate lexeme in continuous memory
    SymbolTable(); // Constructor
	symbol_info* auxInsert(symbol_info* root, int token, char const* lexeme);
    virtual ~SymbolTable();
	void print();
protected:
    int headIndex = 0; // A variable to store the first free position in the lexeme array
    int lexemeArraySize = 0; // Actual size of the lexeme array
    static unsigned long cHash(string const& name); // Hash Function
};


//Extend Symbol Table to create the Reserved words table, Literals table and Identifiers table

class ReservedWordsTable : public SymbolTable {
public:
	void print();
};

class LiteralsTable : public SymbolTable {
    public:
       void cInsert(char const *lexeme);
	   void print();

};
class IdentifiersTable : public SymbolTable {
    public:
        void cInsert(char const *lexeme);
		void print();
};


#endif //COMPILADOR_2019_3_SYMBOLTABLE_H