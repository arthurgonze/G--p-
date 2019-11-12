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

	block = new symbol_info* [TABLE_SIZE];
	memset(block, 0, sizeof(block) * TABLE_SIZE); //Initialize all positions with nullptr
	
}

/**
*	Recursive function to perform the insertion
*	@param root Root node to vist
*	@param token Token ID
*	@param lexeme Lexeme string
*/
symbol_info* SymbolTable::auxInsert(symbol_info* root, int token, char const* lexeme) {

	if (root == nullptr)
	{
		//Create the new node with info
		symbol_info* newNode = new symbol_info();
		newNode->token = token;
		newNode->pos = headIndex;
		newNode->next = nullptr;

		//Add the lexeme to array
		int lexemeSize = strlen(lexeme);
		if (lexemeSize + headIndex >= lexemeArraySize) // Check if the string will exceed the array free space
		{
			lexemeArraySize += LEXEME_ARRAY_SIZE;
			lexemeArray = (char*)realloc(lexemeArray, lexemeArraySize);

		}
		strcpy(lexemeArray + headIndex, lexeme);
		headIndex += lexemeSize + 1; // +1 cause of \0

		return newNode;
	}

	//If there's a valid node, check if its not a duplicate
	if (!(root->token == token && strcmp(lexemeArray + root->pos, lexeme) == 0))
		root->next = auxInsert(root->next, token, lexeme);
	
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
void SymbolTable::cInsert(int token, char const *lexeme)
{
    unsigned long pos = cHash(lexeme);
	block[pos] = auxInsert(block[pos], token, lexeme);
}

/**
 * Go to certain chain through hashing since we know others will not contain the searched value.
 * Next in that chain do a linear search on all element to see if it is there.
 * @param lexeme
 * @return
 */
int SymbolTable::cSearch(char *lexeme)
{
    unsigned long pos = cHash(lexeme);
    symbol_info *temp = block[pos];

    while (temp!=nullptr)
    {
        if (strcmp(lexemeArray + temp->pos, lexeme)==0)
        {
            return temp->token;
        }
        temp = temp->next;
    }
    return -1;
}

SymbolTable::~SymbolTable() = default;
void SymbolTable::print() {}


void LiteralsTable::cInsert(char const* lexeme)
{
	SymbolTable::cInsert(LITERAL, lexeme);
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

		symbol_info* temp = block[i];
		if (temp == nullptr) //If its null, nothing to be done
			continue;

		while (temp != nullptr)
		{

			//Verify flags to print only requested columns

			printf("%s\t\t", this->lexemeArray + temp->pos);

			printf("\n");
			temp = temp->next;

		}

	}

	printf("\n");
}

void IdentifiersTable::cInsert(char const* lexeme)
{
	SymbolTable::cInsert(ID, lexeme);
}

void IdentifiersTable::print()
{
	printf("\nTABELA DE SÍMBOLOS: LITERAIS\n");
	printf("-------------------------------------------\n");

	//Check if column names headers needs to be printed
	//If necessary, print selected;
	printf("LEXEMA\t\t");

	printf("\n-------------------------------------------\n");

	for (int i = 0; i < TABLE_SIZE; ++i)
	{ //Get all the entries

		symbol_info* temp = block[i];
		if (temp == nullptr) //If its null, nothing to be done
			continue;

		while (temp != nullptr)
		{

			//Verify flags to print only requested columns

			printf("%s\t\t", this->lexemeArray + temp->pos);

			printf("\n");
			temp = temp->next;

		}

	}

	printf("\n");
}

symbol_info::~symbol_info()
{
	delete next;
}

void ReservedWordsTable::print()
{
	printf("\nTABELA DE SÍMBOLOS: PALAVRAS RESERVADAS\n");
	printf("-------------------------------------------\n");

	//Check if column names headers needs to be printed
	//If necessary, print selected;

	printf("TOKEN\t\tCÓDIGO");

	printf("\n-------------------------------------------\n");
	
	for (int i = 0; i < TABLE_SIZE; ++i)
	{ //Get all the entries

		symbol_info* temp = block[i];
		if (temp == nullptr) //If its null, nothing to be done
			continue;

		while (temp != nullptr)
		{

			//Verify flags to print only requested columns
			printf("%s\t\t", token_id_to_name(temp->token));

			printf("%d", temp->token);

			printf("\n");
			temp = temp->next;

		}

	}

	printf("\n");
}