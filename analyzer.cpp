//
// Created by souzajbr on 10/09/2019.
//

#include "ctype.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "token.h"
#include "symboltable.h"
#include "analyzer.h"

int currentState = INITIAL_STATE;
int currentInput;
int currentLine = 1;
int currentColumn = 0;

char *lexemeBuffer = nullptr;
char *lastLexemeFound = nullptr;
int lexemeLength = 0;
int lexemeBufferSize = 0;

char const *tokens[] = {"ENDOFILE", "LT", "LE", "EQ", "NE", "GT", "GE", "IF", "BOOL", "ELSE", "ID",
                        "INT", "FLOAT", "COMMA", "LPARENT", "RPARENT", "ASSIGN", "SEMICOLON",
                        "WHILE", "LBRACKET", "RBRACKET", "SWITCH", "BREAK", "RETURN", "PRINT",
                        "READLN", "THROW", "TRY", "CATCH", "CASE", "LITERAL", "TRUE", "FALSE",
                        "ADDRESS", "STAR", "DOT", "LBRACE", "RBRACE", "NOT", "CHAR", "QUOTE",
                        "SIMPLEQUOTE", "BACKSLASH", "COLON", "PLUS", "MINUS", "PIPE", "SLASH",
                        "PERCENT", "AND", "OR", "POINTER", "TYPEDEF", "STRUCT", "NUMINT",
                        "NUMFLOAT", "LITERALCHAR"};

ReservedWordsTable reservedWordsTable;
LiteralsTable literalsTable;
IdsTable identifiersTable;
ReservedWordsTable reservedWordsUsedTable;

FILE *filePointer;

LiteralsTable get_literals_table()
{
    return literalsTable;
}

IdsTable get_identifiers_table()
{
    return identifiersTable;
}

ReservedWordsTable get_reserved_words_table()
{
    return reservedWordsUsedTable;
}

/**
 * Set the currentInput to the next char in input stream,
 * updating the lexeme buffer with the new one and handling the column count
 */
void get_next_char()
{

    if (lexemeLength==lexemeBufferSize)
    {
        lexemeBufferSize += BUFFER_SIZE;
        lexemeBuffer = (char *) realloc(lexemeBuffer, lexemeBufferSize);
    }

    static char buf[IO_BUFFER_SIZE];
    static char *bufp = buf;
    static int n = 0;
    /* Buffer is empty */
    if (n==0)
    { ;
        n = fgets(buf, IO_BUFFER_SIZE, filePointer)!=nullptr ? strlen(buf) : 0;
        bufp = buf;
    }

    char next_input = (--n >= 0) ? *bufp++ : EOF;

    lexemeBuffer[lexemeLength++] = (currentInput = next_input);
    currentColumn++;
}

/**
 * Clear the dirty entries from lexeme buffer
 */
void clear_lexeme()
{

    memset(lexemeBuffer, 0, lexemeBufferSize);

    lexemeBuffer[0] = (char) currentInput;
    lexemeLength = 1;
}

/**
 * Get next input and set automaton state
 * @param state target automaton state
 */
void get_next_char_and_go_to(int state)
{
    get_next_char();
    currentState = state;
}

/**
 * Set automaton state keeping the current input
 * @param state target automaton state
 */
void go_to_state(int state)
{
    currentState = state;
}

/**
 * Remove last char from buffer (usually it belongs to the next token lexeme)
 */
void remove_last_char_from_lexeme()
{
    lexemeBuffer[lexemeLength - 1] = '\0';
}

/**
 * Handles a found token and restart automaton to get next
 * @param token
 * @return found token and lexeme
 */
int found_token_and_restart(int token)
{
    remove_last_char_from_lexeme();

    lastLexemeFound = (char *) malloc(strlen(lexemeBuffer) + 1); //Save the buffer
    strcpy(lastLexemeFound, lexemeBuffer);

    clear_lexeme();

    go_to_state(0);
    return token;
}

/**
 * Handles a found token, but get next input before it
 * @param token
 * @return found token
 */
int found_token_and_get_next_input(int token)
{
    get_next_char();
    return found_token_and_restart(token);
}

/**
 * Handles a found token, but it determines the token type, if it is a reserved word
 * @return found token
 */
int found_token_and_check_for_reserved_word()
{
    remove_last_char_from_lexeme();

    int token = reservedWordsTable.cSearch(lexemeBuffer);

    if (token >= 0)
    {
        reservedWordsUsedTable.cInsert(token, lexemeBuffer);
        return found_token_and_restart(token);
    }
    else
    {
        identifiersTable.cInsert(lexemeBuffer);
        return found_token_and_restart(ID);
    }
}

/**
 * Handles a literal found token, with the correct add in symbolaa table.
 * @param token
 * @return found token
 */
int found_literal_and_restart(int token)
{
    remove_last_char_from_lexeme(); //remove the char from next token
    literalsTable.cInsert(lexemeBuffer);
    return found_token_and_restart(token);
}

/**
 * [MANDATORY] Initialize the analyzer module, create the symtable for reserved words and
 * prepare the input
 */
void lexical_analyzer_init(FILE *fp)
{

    filePointer = fp;

    reservedWordsTable.cInsert(IF, "if");
    reservedWordsTable.cInsert(ELSE, "else");
    reservedWordsTable.cInsert(TRY, "try");
    reservedWordsTable.cInsert(CATCH, "catch");
    reservedWordsTable.cInsert(INT, "int");
    reservedWordsTable.cInsert(FLOAT, "float");
    reservedWordsTable.cInsert(BOOL, "bool");
    reservedWordsTable.cInsert(RETURN, "return");
    reservedWordsTable.cInsert(WHILE, "while");
    reservedWordsTable.cInsert(BREAK, "break");
    reservedWordsTable.cInsert(SWITCH, "switch");
    reservedWordsTable.cInsert(PRINT, "print");
    reservedWordsTable.cInsert(READLN, "readln");
    reservedWordsTable.cInsert(CASE, "case");
    reservedWordsTable.cInsert(THROW, "throw");
    reservedWordsTable.cInsert(TRUE, "true");
    reservedWordsTable.cInsert(FALSE, "false");
    reservedWordsTable.cInsert(CHAR, "char");
    reservedWordsTable.cInsert(TYPEDEF, "typedef");
    reservedWordsTable.cInsert(STRUCT, "struct");

    get_next_char();
}

bool is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

/**
 * Handles the fail state
 * @param reason
 */
void fail(char const *reason)
{
    fprintf(stderr, "[LEXICAL ERROR] %s at %d:%d\n", reason, currentLine, currentColumn - 1);
    clear_lexeme();
}

void handle_next_line()
{
    currentLine++;
    currentColumn = 1;
}
/**
 * Terminates the analyzer instance
 */
void lexical_analyzer_dispose()
{
    free(lexemeBuffer);
    free(lastLexemeFound);
}

/**
 * Search the input for the next token. At end of input stream, it must return
 * ENDOFFILE token.
 * @return found token
 */
int lexical_analyzer_next_token()
{

    while (true)
    {

        switch (currentState)
        {

            case INITIAL_STATE:
                if (currentInput=='\n')
                    handle_next_line();
                if (isspace(currentInput))
                {
                    get_next_char_and_go_to(INITIAL_STATE);
                    clear_lexeme();
                }
                else if (is_letter(currentInput))
                {
                    get_next_char_and_go_to(13);
                }
                else if (is_digit(currentInput))
                    get_next_char_and_go_to(19);
                else
                    switch (currentInput)
                    {
                        case '<':
                            get_next_char_and_go_to(1);
                            break;
                        case '=':
                            get_next_char_and_go_to(4);
                            break;
                        case '>':
                            get_next_char_and_go_to(5);
                            break;
                        case '!':
                            get_next_char_and_go_to(10);
                            break;
                        case '.':
                            get_next_char_and_go_to(14);
                            break;
                        case '+':
                            get_next_char_and_go_to(27);
                            break;
                        case ')':
                            get_next_char_and_go_to(28);
                            break;
                        case '-':
                            get_next_char_and_go_to(29);
                            break;
                        case '(':
                            get_next_char_and_go_to(30);
                            break;
                        case '{':
                            get_next_char_and_go_to(31);
                            break;
                        case '}':
                            get_next_char_and_go_to(32);
                            break;
                        case '[':
                            get_next_char_and_go_to(33);
                            break;
                        case ']':
                            get_next_char_and_go_to(34);
                            break;
                        case ',':
                            get_next_char_and_go_to(35);
                            break;
                        case ';':
                            get_next_char_and_go_to(36);
                            break;
                        case '"':
                            get_next_char_and_go_to(37);
                            break;
                        case '\'':
                            get_next_char_and_go_to(38);
                            break;
                        case '*':
                            get_next_char_and_go_to(39);
                            break;
                        case '|':
                            get_next_char_and_go_to(43);
                            break;
                        case '&':
                            get_next_char_and_go_to(46);
                            break;
                        case '/':
                            get_next_char_and_go_to(49);
                            break;
                        case ':':
                            get_next_char_and_go_to(97);
                            break;
                        case EOF:
                        case ENDOFFILE:
                            go_to_state(54);
                            break;
                        default:
                            get_next_char_and_go_to(18);
                    }
                break;
            case 1:
                switch (currentInput)
                {
                    case '=':
                        go_to_state(2);
                        break;
                    default:
                        go_to_state(3);
                        break;
                }
                break;
            case 2:
                return found_token_and_get_next_input(LE); //found LE
            case 3:
                return found_token_and_restart(LT); //found LT
            case 4:
                switch (currentInput)
                {
                    case '=':
                        get_next_char_and_go_to(6);
                        break;
                    default:
                        go_to_state(7);
                        break;
                }
                break;
            case 5:
                switch (currentInput)
                {
                    case '=':
                        get_next_char_and_go_to(8);
                        break;
                    default:
                        go_to_state(9);
                        break;
                }
                break;
            case 6:
                return found_token_and_get_next_input(EQ); //found EQ
            case 7:
                return found_token_and_restart(ASSIGN); //found assign
            case 8:
                return found_token_and_get_next_input(GE); //found GE
            case 9:
                return found_token_and_restart(GT); //found GT
            case 10:
                switch (currentInput)
                {
                    case '=':
                        get_next_char_and_go_to(11);
                        break;
                    default:
                        go_to_state(12);
                        break;
                }
                break;
            case 11:
                return found_token_and_get_next_input(NE); //found NE
            case 12:
                return found_token_and_restart(NOT); //found NOT
            case 13:
                if (is_letter(currentInput) || is_digit(currentInput))
                    get_next_char_and_go_to(13);
                else
                    go_to_state(15);
                break;
            case 14:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(55);
                else
                    go_to_state(53);
                break;
            case 15:
                return found_token_and_check_for_reserved_word();
            case 17: //Unexpected EOF fail state
                fail("Unexpected end of file");
                go_to_state(54);
                break;
            case 18: //Unexpected char fail state
                fail("Unexpected character");
                go_to_state(INITIAL_STATE);
                break;
            case 19:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(19);
                else if (currentInput=='.')
                    get_next_char_and_go_to(25);
                else if (currentInput=='e' || currentInput=='E')
                    get_next_char_and_go_to(20);
                else if (is_letter(currentInput))
                    go_to_state(94);
//                else if (currentInput == ',')
//                    go_to_state(95);
                else
                    go_to_state(24);
                break;
            case 20:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(22);
                else if (currentInput=='+' || currentInput=='-')
                    get_next_char_and_go_to(21);
                else
                    go_to_state(93);
                break;
            case 21:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(22);
                else
                    go_to_state(92);
                break;
            case 22:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(22);
                else
                {
                    go_to_state(23);
                }
                break;
            case 23:
                return found_token_and_restart(NUMFLOAT); //found NUMFLOAT
            case 24:
                return found_token_and_restart(NUMINT); //found NUMINT
            case 25:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(26);
                else if (is_letter(currentInput))
                    go_to_state(91);
                else
                    go_to_state(23);
                break;
            case 26:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(26);
                else if (currentInput=='E' || currentInput=='e')
                    get_next_char_and_go_to(20);
                else if (is_letter(currentInput))
                    go_to_state(91);
                else
                    go_to_state(23);
                break;
            case 27:
                return found_token_and_restart(PLUS); //found PLUS
            case 28:
                return found_token_and_restart(RPARENT); //found RPARENT
            case 29:
                if (currentInput=='>')
                    get_next_char_and_go_to(56);
                else
                    go_to_state(57);
                break;
            case 30:
                return found_token_and_restart(LPARENT); //found LPARENT
            case 31:
                return found_token_and_restart(LBRACE); //found LBREACE
            case 32:
                return found_token_and_restart(RBRACE); //found RBRACE
            case 33:
                return found_token_and_restart(LBRACKET); //found LBRACKET
            case 34:
                return found_token_and_restart(RBRACKET); //found RBRACKET
            case 35:
                return found_token_and_restart(COMMA); //found COMMA
            case 36:
                return found_token_and_restart(SEMICOLON); //found SEMICOLON
            case 37:
                switch (currentInput)
                {
                    case '"':
                        get_next_char_and_go_to(40);
                        break;
                    case '\\':
                        get_next_char_and_go_to(41);
                        break;
                    case EOF:
                    case ENDOFFILE:
                        go_to_state(17);
                        break;
                    case '\n':
                        handle_next_line();
                    default:
                        get_next_char_and_go_to(37);
                        break;
                }
                break;
            case 38:
                switch (currentInput)
                {
                    case '\\':
                        get_next_char_and_go_to(99);
                        break;
                    default:
                        get_next_char_and_go_to(42);
                        break;
                }
                break;
            case 39:
                return found_token_and_restart(STAR); //found STAR
            case 40:
                return found_literal_and_restart(LITERAL); //found LITERAL
            case 41:
                get_next_char_and_go_to(37);
                break;
            case 42:
                switch (currentInput)
                {
                    case '\'':
                        get_next_char_and_go_to(98);
                        break;
                    case EOF:
                    case ENDOFFILE:
                        go_to_state(17);
                        break;
                    default:
                        go_to_state(96);
                        break;
                }
                break;
            case 43:
                if (currentInput=='|')
                    get_next_char_and_go_to(44);
                else
                    go_to_state(45);
                break;
            case 44:
                return found_token_and_restart(OR);
            case 45:
                return found_token_and_restart(PIPE);
            case 46:
                if (currentInput=='&')
                    get_next_char_and_go_to(47);
                else
                    go_to_state(48);
                break;
            case 47:
                return found_token_and_restart(AND);
            case 48:
                return found_token_and_restart(ADDRESS);
            case 49:
                switch (currentInput)
                {
                    case '*':
                        get_next_char_and_go_to(51);
                        break;
                    default:
                        go_to_state(50);
                        break;
                }
                break;
            case 50:
                return found_token_and_restart(SLASH);
            case 51:
                switch (currentInput)
                {
                    case '*':
                        get_next_char_and_go_to(52);
                        break;
                    case EOF:
                    case ENDOFFILE:
                        go_to_state(17);
                        break;
                    case '\n':
                        handle_next_line();
                    default:
                        get_next_char_and_go_to(51);
                        break;
                }
                break;
            case 52:
                switch (currentInput)
                {
                    case '/':
                        get_next_char_and_go_to(INITIAL_STATE);
                        break;
                    case EOF:
                    case ENDOFFILE:
                        go_to_state(17);
                        break;
                    default:
                        get_next_char_and_go_to(51);
                        break;
                }
                break;
            case 53:
                return found_token_and_restart(DOT); //found DOT
            case 54:
                return found_token_and_restart(ENDOFFILE); //found EOF
            case 55:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(55);
                else if (currentInput=='e' || currentInput=='E')
                    get_next_char_and_go_to(20);
                else if (is_letter(currentInput))
                {
                    go_to_state(18);
                }
                else
                    go_to_state(23);
                break;
            case 56:
                return found_token_and_restart(POINTER); //found POINTER
            case 57:
                return found_token_and_restart(MINUS); //found MINUS
            case 91: //Unexpected float     number before the ID = fail state
                fail("The ID can't start with float numbers");
                go_to_state(INITIAL_STATE);
                break;
            case 92: //Unexpected char after the E or e = fail state
                fail("Missing the power function value on the scientific notation");
                go_to_state(INITIAL_STATE);
                break;
            case 93: //Unexpected char after the E or e = fail state
                fail("Unexpected character after the scientific notation");
                go_to_state(INITIAL_STATE);
                break;
            case 94: //Unexpected int number before the ID = fail state
                fail("The ID can't start with int numbers");
                go_to_state(INITIAL_STATE);
                break;
//            case 95: //Number fail state : change ',' to '.'
//                fail("Missing . character. Maybe change ',' to '.' ");
//                go_to_state(INITIAL_STATE);
//                break;
            case 96: //Missing ' character fail state
                fail("Missing \' character");
                go_to_state(INITIAL_STATE);
                break;
            case 97:
                return found_token_and_restart(COLON);
            case 98:
                return found_literal_and_restart(LITERALCHAR); //found LITERAL
            case 99:
                get_next_char_and_go_to(42);
                break;
        }

    }
}

char *lexical_analyzer_last_lexeme()
{
    return lastLexemeFound;
}

/**
 * Convert token numerical IDs to a textual represent
 * @param id Kind of token
 * @return Textual value of token
 */
char const *token_id_to_name(int id)
{

    return tokens[id];
}