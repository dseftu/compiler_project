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

/* list of reserved word names */
char *word [ ] = {  "null", "begin", "call", "const", "do", "else", "end", "if", 
	             "odd", "procedure", "read", "then", "var", "while", "write"}; 
                	       
/* internal representation  of reserved words */
int  wsym [ ] =  { nulsym, beginsym, callsym, constsym, dosym, elsesym, endsym, ifsym,
                          oddsym, procsym, readsym, thensym, varsym, whilesym, writesym};

/* list of special symbols */
int ssym[256];

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

int isSpecialSymbols(char c);
void readInput(char *filename);
void readInputBetterer(char *filename);
char peekc(FILE *fid);

int main(int argc, char *argv[])
{
    // TODO: EVERYTHING
    if (argc != 2)
    {
        printf("Invalid number of arguments given.\n\nExpected:\nP-machine <input file>\n");
        return 0;
    }


    // read in the input file
    readInputBetterer(argv[1]);

    return 0;
}

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

int isInvisible(char c)
{
    if (c == ' ' ||
        c == '\r'||
        c == '\n')
        return TRUE;
    return FALSE;
}
char peekc(FILE *fid)
{
    int nextC = getc(fid);
    ungetc(nextC, fid);
    return (char)nextC;
}

// reads in the pl0 file and loads the Lexeme Table.
// Processing will occur as it reads in the file.
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
    else
    {
        // print header
        printf("Source Program:\n");

        char nextWord[MAX_IDENTIFIER_LENGTH];
        nextWord[0] = '\0';
        int i = 0;
        int c = getc(fid);
        int stillReading = TRUE;
        while (halt == FALSE && stillReading == TRUE)
        {
            char ch = (char)c;
            if (c == EOF) 
            {
                stillReading = FALSE;
            }
            else
            {
                // display this char and ready the next one
                putchar(c);
                c = getc(fid);
            }

            // check to see if the symbol is a special symbol
            if (isSpecialSymbols(c))
            {
                // special symbol handling
                if (ch == ';')
                {
                    // halt if next symbol is another special symbol
                    if (isSpecialSymbols(peekC(fid))) halt == true;

                    namerecord_t newRec;
                    newRec.kind = semicolonsym;
                    newRec.name[0] = ';';
                    newRec.name[1] = '\0';
                    newRec.val = 0;
                    newRec.level = 0;
                    newRec.adr = 0;

                    symbol_table[symbol_table_index++] = newRec;
                }
                else if (ch == ',')
                {
                    // halt if next symbol is another special symbol
                    if (isSpecialSymbols(peekC(fid))) halt == true;
                    
                    namerecord_t newRec;
                    newRec.kind = commasym;
                    newRec.name[0] = ',';
                    newRec.name[1] = '\0';
                    newRec.val = 0;
                    newRec.level = 0;
                    newRec.adr = 0;

                    symbol_table[symbol_table_index++] = newRec;
                }
                else if (ch == '+')
                {
                    // halt if next symbol is another special symbol
                    if (isSpecialSymbols(peekC(fid))) halt == true;
                    
                    namerecord_t newRec;
                    newRec.kind = plussym;
                    newRec.name[0] = '+';
                    newRec.name[1] = '\0';
                    newRec.val = 0;
                    newRec.level = 0;
                    newRec.adr = 0;

                    symbol_table[symbol_table_index++] = newRec;
                }
                else if (ch == '-')
                {
                    // halt if next symbol is another special symbol
                    if (isSpecialSymbols(peekC(fid))) halt == true;
                    
                    namerecord_t newRec;
                    newRec.kind = minussym;
                    newRec.name[0] = '-';
                    newRec.name[1] = '\0';
                    newRec.val = 0;
                    newRec.level = 0;
                    newRec.adr = 0;

                    symbol_table[symbol_table_index++] = newRec;
                }
                else if (ch == '*')
                {
                    // halt if next symbol is another special symbol
                    if (isSpecialSymbols(peekC(fid))) halt == true;
                    
                    namerecord_t newRec;
                    newRec.kind = multsym;
                    newRec.name[0] = '*';
                    newRec.name[1] = '\0';
                    newRec.val = 0;
                    newRec.level = 0;
                    newRec.adr = 0;

                    symbol_table[symbol_table_index++] = newRec;
                }
                else if (ch == '(')
                {
                    // halt if next symbol is another special symbol
                    if (isSpecialSymbols(peekC(fid))) halt == true;
                    
                    namerecord_t newRec;
                    newRec.kind = lparentsym;
                    newRec.name[0] = '(';
                    newRec.name[1] = '\0';
                    newRec.val = 0;
                    newRec.level = 0;
                    newRec.adr = 0;

                    symbol_table[symbol_table_index++] = newRec;
                }
                else if (ch == ')')
                {
                    // halt if next symbol is another special symbol
                    if (isSpecialSymbols(peekC(fid)) && peekC(fid) != ';') halt == true;
                    
                    namerecord_t newRec;
                    newRec.kind = rparentsym;
                    newRec.name[0] = ')';
                    newRec.name[1] = '\0';
                    newRec.val = 0;
                    newRec.level = 0;
                    newRec.adr = 0;

                    symbol_table[symbol_table_index++] = newRec;
                }
                else if (ch == '=')
                {
                    // halt if next symbol is another special symbol
                    if (isSpecialSymbols(peekC(fid))) halt == true;
                    
                    namerecord_t newRec;
                    newRec.kind = eqlsym;
                    newRec.name[0] = '=';
                    newRec.name[1] = '\0';
                    newRec.val = 0;
                    newRec.level = 0;
                    newRec.adr = 0;

                    symbol_table[symbol_table_index++] = newRec;
                }
                else if (ch == '.')
                {
                    // halt if next symbol is another special symbol
                    if (isSpecialSymbols(peekC(fid))) halt == true;
                    
                    namerecord_t newRec;
                    newRec.kind = periodsym;
                    newRec.name[0] = '.';
                    newRec.name[1] = '\0';
                    newRec.val = 0;
                    newRec.level = 0;
                    newRec.adr = 0;

                    symbol_table[symbol_table_index++] = newRec;
                }
                else if (ch == '<')
                {   
                    // check to see if next symbol is '>, ='
                    int tempC = getc(fid);
                    if ((char)tempC == '>' || (char)tempC == '=')
                    {
                        // print it out and prep for the next iteration
                        putchar(c);
                        c = tempC;

                        // halt if next symbol is another special symbol
                        if (isSpecialSymbols(peekC(fid))) halt == true;
    
                        // if it was neqsym
                        if ((char)tempC == '>')
                        {
                            namerecord_t newRec;
                            newRec.kind = neqsym;
                            newRec.name[0] = '<';
                            newRec.name[1] = '>';
                            newRec.name[2] = '\0';
                            newRec.val = 0;
                            newRec.level = 0;
                            newRec.adr = 0;

                            symbol_table[symbol_table_index++] = newRec;

                        }
                        else
                        {
                            namerecord_t newRec;
                            newRec.kind = leqsym;
                            newRec.name[0] = '<';
                            newRec.name[1] = '=';
                            newRec.name[2] = '\0';
                            newRec.val = 0;
                            newRec.level = 0;
                            newRec.adr = 0;

                            symbol_table[symbol_table_index++] = newRec;
                        }
                    }
                    else
                    {
                        // halt if next symbol is another special symbol
                        if (isSpecialSymbols(peekC(fid))) halt == true;

                        ungetc(tempC, fid);
                        namerecord_t newRec;
                        newRec.kind = lessym;
                        newRec.name[0] = '<';
                        newRec.name[1] = '\0';
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                }
                else if (ch == '>')
                {
                    // check to see if next symbol is '='
                    int tempC = getc(fid);
                    if ((char)tempC == '=')
                    {
                        // print it out and prep for the next iteration
                        putchar(c);
                        c = tempC;

                        // halt if next symbol is another special symbol
                        if (isSpecialSymbols(peekC(fid))) halt == true;

                        namerecord_t newRec;
                        newRec.kind = geqsym;
                        newRec.name[0] = '>';
                        newRec.name[1] = '=';
                        newRec.name[2] = '\0';
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else
                    {
                        ungetc(tempC, fid);
                        namerecord_t newRec;
                        newRec.kind = gtrsym;
                        newRec.name[0] = ',';
                        newRec.name[1] = '\0';
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                }                
                else if (ch == ':')
                {
                    // check to see if next symbol is '='
                    int tempC = getc(fid);
                    if ((char)tempC == '=')
                    {
                        // print it out and prep for the next iteration
                        putchar(c);
                        c = tempC;

                        namerecord_t newRec;
                        newRec.kind = becomessym;
                        newRec.name[0] = ':';
                        newRec.name[1] = '=';
                        newRec.name[2] = '\0';
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else
                    {
                        // invalid symbol
                        halt = TRUE;
                    }
                }
                else if (ch == '/')
                {
                    int nextC = getc(fid);
                    // see if we are at the end
                    if (nextC == EOF) stillReading = FALSE;
                    else
                    {
                        // put it back so we can play nice later
                        char nextCh = (char)nextC;
                        ungetc(nextC, fid);

                        // this means we are in a comment thread.
                        if (nextCh == '*')
                        {
                            putchar(c);
                            int lastC = getc(fid);
                            if (lastC == EOF) 
                            {
                                stillReading = FALSE;
                            }
                            else
                            {
                                putchar(lastC);
                                int inComment = TRUE;

                                while (inComment)
                                {
                                    c = getc(fid);
                                    

                                    if (c == EOF)
                                    {
                                        stillReading = FALSE;
                                        inComment = FALSE;
                                    }
                                    else
                                    {
                                        if ((char)c == '/' && (char)lastC == '*')
                                        {
                                            inComment = FALSE;
                                        }
                                        else
                                        {
                                            putchar(c);
                                            lastC = c;
                                        }
                                    }
                                }
                            }
                        }
                        // not a comment but a slashsym instead
                        else
                        {
                            namerecord_t newRec;
                            newRec.kind = slashsym;
                            newRec.name[0] = '/';
                            newRec.name[1] = '\0';
                            newRec.val = 0;
                            newRec.level = 0;
                            newRec.adr = 0;

                            symbol_table[symbol_table_index++] = newRec;
                        }
                        
                    }

                // clean out the nextWord for the next symbol
                nextWord[0] = '\0';
                i = 0;

            }
            // this means it wasn't a special symbol and might be some other thing, but we are at the end of
            // the thing so we need to tokenize it
            else if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || !stillReading)
            {
                if (i>0)
                {
                    // these are whole words that always have a break after them
                    if (strcmp(nextWord, "const") == 0)
                    {
                        namerecord_t newRec;
                        newRec.kind = constsym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else if (strcmp(nextWord, "var") == 0)
                    {   
                        namerecord_t newRec;
                        newRec.kind = varsym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else if (strcmp(nextWord, "proecedure") == 0)
                    {   
                        namerecord_t newRec;
                        newRec.kind = procsym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else if (strcmp(nextWord, "call") == 0)
                    {   
                        namerecord_t newRec;
                        newRec.kind = callsym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else if (strcmp(nextWord, "begin") == 0)
                    {   
                        namerecord_t newRec;
                        newRec.kind = beginsym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else if (strcmp(nextWord, "if") == 0)
                    {   
                        namerecord_t newRec;
                        newRec.kind = ifsym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else if (strcmp(nextWord, "then") == 0)
                    {   
                        namerecord_t newRec;
                        newRec.kind = thensym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else if (strcmp(nextWord, "const") == 0)
                    {   
                        namerecord_t newRec;
                        newRec.kind = varsym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else if (strcmp(nextWord, "const") == 0)
                    {   
                        namerecord_t newRec;
                        newRec.kind = varsym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else if (strcmp(nextWord, "const") == 0)
                    {   
                        namerecord_t newRec;
                        newRec.kind = varsym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    else if (strcmp(nextWord, "const") == 0)
                    {   
                        namerecord_t newRec;
                        newRec.kind = varsym;
                        strcpy(newRec.name, nextWord);
                        newRec.val = 0;
                        newRec.level = 0;
                        newRec.adr = 0;

                        symbol_table[symbol_table_index++] = newRec;
                    }
                    
                }

                nextWord[0] = '\0';
                i = 0;
            }

            // this means we still have more consecutive chars for this thing
            else
            {
                if (i > MAX_IDENTIFIER_LENGTH)
                    halt = TRUE;
                nextWord[i] = ch;
                nextWord[++i] = '\0';

                
            }
        }
        fclose(fid);
    }

}

// a second attempt at doing this whole thing...
void readInputBetterer(char *filename)
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
        }
        else if (isalpha(c))
        {
            // alpha path.  can be either digit or alpha but no special.
        }
    }

}