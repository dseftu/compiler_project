// David M Allen
// Scanner
// COP 3402 - Systems Software

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define nulsym 1
#define identsym 2
#define numbersym 3
#define plussym 4
#define minussym 5
#define multsym 6
#define slashsym 7
#define oddsym 8
#define eqlsym 9
#define neqsym 10
#define lessym 11
#define leqsym 12
#define gtrsym 13
#define geqsym 14
#define lparentsym 15
#define rparentsym 16
#define commasym 17
#define semicolonsym 18
#define periodsym 19
#define becomessym 20
#define beginsym 21
#define endsym 22
#define ifsym 23
#define thensym 24
#define whilesym 25
#define dosym 26
#define callsym 27
#define constsym 28
#define varsym 29
#define procsym 30
#define writesym 31
#define readsym 32
#define elsesym 33

#define const 1
#define var 2
#define proc 3

#define MAX_IDENTIFIER_LENGTH 11
#define MAX_NUMBER_LENGTH 5
#define MAX_SYMBOL_TABLE_SIZE 100

// the halt flag.  When set to TRUE, the program ends
int halt = FALSE;

/*** structure of the symbol table record ***/
typedef struct  
{ 
    int kind; 		// const = 1, var = 2, proc = 3.
    char name[MAX_IDENTIFIER_LENGTH];	// name up to 11 chars
    int val; 		// number (ASCII value) 
    int level; 		// L  level
    int adr; 		// M  address
} namerecord_t; 

namerecord_t symbol_table[MAX_SYMBOL_TABLE_SIZE];
int symbol_table_index = 0;

// returns TRUE if c is a special symbol
int isSpecialSymbols(char c);

// determines if this is an invis char
int isInvisible(char c);

// is word is a reserved word, this attempts to add it to the symbol table.
// returns true if the word was a reserved word.  Sets halt to true
// if the add fails.
int isReservedWord(char* word);

// performs the processing of the input file
void readInput(char *filename);

// makes a copy of the next char in the file
char peekC(FILE *fid);

// generically adds to the symbol table.  returns FALSE if fails
int addNewSymbol(int kind, char* name, int val, int level, int adr);

// handles situations with two consec. symbols.  raises halt if fails
void handleSpecialSymbolPair(char* word, FILE*fid);

// prints data output
void printLexemeTable();
void printLexemeList();

int main(int argc, char *argv[])
{
    // checks to make sure we got the right input
    if (argc != 2)
    {
        printf("Invalid number of arguments given.\n\nExpected:\nP-machine <input file>\n");
        return 0;
    }

    // read in the input file
    readInput(argv[1]);

    printf("\n");
    printLexemeTable();

    printf("\n");
    printLexemeList();

    return 0;
}

// determines if this is a special symbol
int isSpecialSymbols(char c)
{
    if (c == '+' ||
        c == '-' ||
        c == '*' ||
        c == '/' ||
        c == '(' ||
        c == ')' ||
        c == '=' ||
        c == ',' ||
        c == '.' ||
        c == '#' ||
        c == '<' ||
        c == '>' ||
        c == '$' ||
        c == '%' ||
        c == ':' ||
        c == ';')
        return TRUE;

    return FALSE;
}

// determines if this is a char to ignore
int isInvisible(char c)
{
    if (c == ' ' ||
        c == '\r'||
        c == '\n'||
        c == '\t')
        return TRUE;
    return FALSE;
}

// is word is a reserved word, this attempts to add it to the symbol table.
// returns true if the word was a reserved word.  Sets halt to true
// if the add fails.
int isReservedWord(char* word)
{
    int currentSym = -1;

    if (strcmp(word, "const") == 0) currentSym = constsym;
    else if (strcmp(word, "var") == 0) currentSym = varsym;
    else if (strcmp(word, "proecedure") == 0) currentSym = procsym;
    else if (strcmp(word, "call") == 0) currentSym = callsym;
    else if (strcmp(word, "begin") == 0) currentSym = beginsym;
    else if (strcmp(word, "end") == 0) currentSym = endsym;
    else if (strcmp(word, "if") == 0) currentSym = ifsym;
    else if (strcmp(word, "then") == 0) currentSym = thensym;
    else if (strcmp(word, "else") == 0) currentSym = elsesym;
    else if (strcmp(word, "while") == 0) currentSym = whilesym;
    else if (strcmp(word, "do") == 0) currentSym = dosym;
    else if (strcmp(word, "read") == 0) currentSym = readsym;
    else if (strcmp(word, "write") == 0) currentSym = writesym;
    else if (strcmp(word, "null") == 0) currentSym = nulsym;
    else if (strcmp(word, "odd") == 0) currentSym = oddsym;

    // if we matched, currentSym != -1
    if (currentSym != -1)
    {
        halt = !addNewSymbol(currentSym, word, 0, 0, 0);
        return TRUE;
    }

    return FALSE;


}

// gets a copy of the next char in the file
char peekC(FILE *fid)
{
    int nextC = getc(fid);
    ungetc(nextC, fid);
    return (char)nextC;
}

// handles a special symbol pair and adds to the symbol table if needed
void handleSpecialSymbolPair(char* word, FILE*fid)
{
    // see if the next char is a special symbol because that is right out
    char nextC = peekC(fid);
    if (isSpecialSymbols(nextC))
    {
        printf("\nInvalid symbol!  Halting!\n");
        halt = TRUE;
        return;
    }

    int currentSym = -1;

    // is this one of our valid pairs?
    if (strcmp(word, "<=") == 0) currentSym = leqsym;
    else if (strcmp(word, "<>") == 0) currentSym = neqsym;
    else if (strcmp(word, ">=") == 0) currentSym = geqsym;
    else if (strcmp(word, ":=") == 0) currentSym = becomessym;

    // if it was, let's handle it
    if (currentSym != -1)
    {
        halt = !addNewSymbol(currentSym, word, 0, 0, 0);
        return;
    }

    // is this our super special pair for comments?
    if (strcmp(word, "/*") == 0)
    {
        // handle comment string        
        int lastC = getc(fid);
        if (lastC == EOF) return; // unexpected end, but who cares
        putchar(lastC);

        int commentsEnded = FALSE;
        while (!commentsEnded && !halt)
        {
            int c = getc(fid);
            if (c == EOF) return; // unexpected end, but who cares
            putchar(c);

            if ((char)lastC == '*' && (char)c == '/') commentsEnded = TRUE;
            else lastC = c;
        }
    }
    else
    {
        // unrecognized sequence
        printf("\nUnrecognized sequence! Halting!\n");
        halt = TRUE;
        return;
    }

}

// a second attempt at doing this whole thing...
void readInput(char *filename)
{
    // opens the file
    FILE *fid;

    fid = fopen(filename, "r");

    if (fid == NULL)
    {
        // can't open file
        printf("Unable to open file!");
        halt = TRUE;
        return;
    }

    // print header
    printf("Source Program:\n");

    // this will be used to store identifiers as we read them
    char nextWord[MAX_IDENTIFIER_LENGTH];
    nextWord[0] = '\0';
    int i = 0;

    int stillReading = TRUE;
    while (stillReading == TRUE && halt == FALSE)
    {
        // read the next char and print it
        int c = getc(fid);
        if (c == EOF)
        {
            stillReading = FALSE;
            break;
        }
        putchar(c);

        // determine what to do with this c
        if (isdigit(c))
        {
            // digit path.  must continue to be a digit or we halt.
            int value = c - '0';
            int digitEnded = FALSE;
            i = 0;
            nextWord[i] = (char)c;
            nextWord[++i] = '\0';

            while (digitEnded == FALSE)
            {
                char nextC = peekC(fid);

                if (isInvisible(nextC) || isSpecialSymbols(nextC))
                {
                    // wrap up token here
                    halt = !addNewSymbol(numbersym, nextWord, value, 0, 0);
                    digitEnded = TRUE;
                }
                else if (isdigit(nextC))
                {
                    // more digit to get.
                    c = getc(fid);
                    putchar(c);
                    nextWord[i] = (char)c;
                    nextWord[++i] = '\0';
                    value = 10 * value + (c - '0');
                }
                else if (nextC == EOF)
                {
                    // unexpected end
                    halt = TRUE;
                    break;
                }
                else if (isalpha(nextC))
                {
                    // alpha in my digits.  halt processing.
                    printf("\nInvalid identifier!  Halting!\n");
                    halt = TRUE;
                    break;
                }

            }            

            if (i > MAX_NUMBER_LENGTH)
            {
                // digit was too long!
                printf("\nNumber was too long!  Halting!\n");
                halt = TRUE;
                break;
            }

            if (c == EOF)
            {
                stillReading = FALSE;
                break;
            }
        }
        else if (isalpha(c))
        {
            // alpha path.  can be either digit or alpha.  Special or invis ends.
            // can't be longer than MAX_IDENTIFIER_LENGTH'
            int alphaEnded = FALSE;
            i = 0;
            nextWord[i] = (char)c;
            nextWord[++i] = '\0';

            while (alphaEnded == FALSE)
            {
                char nextC = peekC(fid);

                if (isInvisible(nextC) || isSpecialSymbols(nextC) || nextC == EOF)
                {
                    // wrap up token here
                    // check to see if this was a reservedWord, if it is, add it as one
                    // otherwise add it as a identsym
                    if (!isReservedWord(nextWord))
                    {
                        halt = !addNewSymbol(identsym, nextWord, 0, 0, 0);
                    }
                    alphaEnded = TRUE;
                }
                else if (isalpha(nextC) || isdigit(nextC))
                {
                    // more stuff to get.
                    c = getc(fid);
                    putchar(c);
                    nextWord[i] = (char)c;
                    nextWord[++i] = '\0';
                }
            }            

            if (i > MAX_IDENTIFIER_LENGTH)
            {
                // ident was too long!
                printf("\nIdentifier too long!  Halting!\n");
                halt = TRUE;
                break;
            }

            if (c == EOF)
            {
                stillReading = FALSE;
                break;
            }
        }
        else if (isSpecialSymbols(c))
        {
            // special symbols path

            // take a looksee at the next char
            char nextC = peekC(fid);

            if (!isSpecialSymbols(nextC))
            {
                // we're done let's pack it up
                // this means we found a single char special symbol
                 int currentSym = -1;
                if ((char)c == '+') currentSym = plussym;
                else if ((char)c == '-') currentSym = minussym;
                else if ((char)c == '*') currentSym = multsym;
                else if ((char)c == '/') currentSym = slashsym;
                else if ((char)c == '(') currentSym = lparentsym;
                else if ((char)c == ')') currentSym = rparentsym;
                else if ((char)c == '=') currentSym = eqlsym;
                else if ((char)c == ',') currentSym = commasym;
                else if ((char)c == '.') currentSym = periodsym;
                else if ((char)c == '<') currentSym = lessym;
                else if ((char)c == '>') currentSym = gtrsym;
                else if ((char)c == ';') currentSym = semicolonsym;

                // if we didn't match, well bad things happened.
                if (currentSym == -1) 
                {
                    printf("\nInvalid symbol!  Halting!\n");
                    halt = TRUE;
                }
                else
                {
                    nextWord[0] = (char)c;
                    nextWord[1] = '\0';
                    halt = !addNewSymbol(currentSym, nextWord, 0, 0, 0);
                }
            }
            else if ((char)c == '/' ||
                     (char)c == '<' ||
                     (char)c == '>' ||
                     (char)c == ':')
            {
                // so we have two consecutive special symbols.
                // do we match one of our paired special symbols?
                nextWord[0] = (char)c;
                c = getc(fid);
                putchar(c);
                nextWord[1] = (char)c;
                nextWord[2] = '\0';

                handleSpecialSymbolPair(nextWord, fid);

            }
            else
            {
                // this means we found an invalid pair.
                printf("\nUnrecognized symbols! Halting!\n");
                halt = TRUE;
                break;
            }
        }
        else if (isInvisible(c))
        {
            // we are ignoring invis chars like newlines and whitespace.
        }
        else
        {
            // unrecognized character.
            printf("\nUnrecognized character (%d)! Halting!\n", c);
            halt = TRUE;
            break;
        }
    }

}

// attempt to add new token
int addNewSymbol(int kind, char* name, int val, int level, int adr)
{
    if (symbol_table_index + 1 >= MAX_SYMBOL_TABLE_SIZE) 
    {
        printf("\nSymbol table full!  Halting!\n");
        return FALSE;
    }

    //printf("Adding %d %s\n",kind, name);

    namerecord_t newRec;
    newRec.kind = kind;
    strcpy(newRec.name, name);
    newRec.val = val;
    newRec.level = level;
    newRec.adr = adr;

    symbol_table[symbol_table_index++] = newRec;

    return TRUE;
}

// print the lexeme table
void printLexemeTable()
{
    // header 
    printf("\nLexeme Table:\n");
    printf("lexeme\ttoken type\n");

    // iterate through the table, printing the name and token value
    int i = 0;
    for (i = 0; i < symbol_table_index; i++)
    {
        printf("%s\t%d\n", symbol_table[i].name, symbol_table[i].kind);
    }

}

// the problem statement included two different representations of the lexeme table.
// here is the other variant.
// print the lexeme list
void printLexemeList()
{
    // header 
    printf("\nLexeme List:\n");

    // iterate through the table, printing the name and token value
    int i = 0;
    for (i = 0; i < symbol_table_index; i++)
    {
        printf("%d ", symbol_table[i].kind);
            if (symbol_table[i].kind == identsym)
                printf("%s ", symbol_table[i].name);
            if (symbol_table[i].kind == numbersym)
                printf("%d ", symbol_table[i].val);
    }   
}