// David M Allen
// Scanner
// COP 3402 - Systems Software

#include "common.h"
#include "scanner.h"
#include "errorCodes.h"

int line = 1;

// Scans an input file and returns the lexeme table
lexeme* scan(char* filename, int* _lexemeListEnd)
{
    scanInput(filename);
    *_lexemeListEnd = lexemeTableIndex;

    return lexemeList;
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
        c == '\t'){
            if (c == '\n') line++;
            return TRUE;
        } 
        
    return FALSE;
}

// is word is a reserved word, this attempts to add it to the symbol table.
// returns true if the word was a reserved word.  Sets halt to true
// if the add fails.
int isReservedWord(char* word)
{
    int currentSym = -1;

    if (strcmp(word, "const") == 0) currentSym = constsym;
    //else if (strcmp(word, "var") == 0 || strcmp(word, "int") == 0 ) currentSym = varsym;
    else if (strcmp(word, "var") == 0) currentSym = varsym; // using var only.  line above uses either
    else if (strcmp(word, "procedure") == 0) currentSym = procsym;
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
        halt = !addNewSymbol(currentSym, word, 0);
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
        error(UNKSYMBOL);
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
        halt = !addNewSymbol(currentSym, word, 0);
        return;
    }

    // is this our super special pair for comments?
    if (strcmp(word, "/*") == 0)
    {
        // handle comment string
        int lastC = getc(fid);

        if (lastC == EOF) return; // unexpected end, but who cares

        int commentsEnded = FALSE;
        while (!commentsEnded && !halt)
        {
            int c = getc(fid);

            if (c == EOF) return; // unexpected end, but who cares

            if ((char)lastC == '*' && (char)c == '/') commentsEnded = TRUE;
            else lastC = c;
        }
    }
    else
    {
        // unrecognized sequence
        error(UNKSYMBOL);
        return;
    }
}

// a second attempt at doing this whole thing...
void scanInput(char *filename)
{
    // opens the file
    FILE *fid;

    fid = fopen(filename, "r");

    if (fid == NULL)
    {
        // can't open file
        error(FILEIO);
        return;
    }

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
                    halt = !addNewSymbol(numbersym, nextWord, value);
                    digitEnded = TRUE;
                }
                else if (isdigit(nextC))
                {
                    // more digit to get.
                    c = getc(fid);

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
                    error(INVALIDIDENTIFIER);
                    break;
                }

            }

            if (i > MAX_NUMBER_LENGTH)
            {
                // digit was too long!
                error(TOOLARGENUMBER);
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
                        halt = !addNewSymbol(identsym, nextWord, 0);
                    }
                    alphaEnded = TRUE;
                }
                else if (isalpha(nextC) || isdigit(nextC))
                {
                    // more stuff to get.
                    c = getc(fid);

                    nextWord[i] = (char)c;
                    nextWord[++i] = '\0';
                }
            }

            if (i > MAX_IDENTIFIER_LENGTH)
            {
                // ident was too long!
                error(IDENTIFIERTOOLONG);
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
            if ((char)c == '+')
            {
                nextWord[0] = (char)c;
                nextWord[1] = '\0';
                halt = !addNewSymbol(plussym, nextWord, 0);

            }
            else if ((char)c == '-')
            {
                nextWord[0] = (char)c;
                nextWord[1] = '\0';
                halt = !addNewSymbol(minussym, nextWord, 0);

            }
            else if ((char)c == '*' && (char)nextC != '/')
            {
                nextWord[0] = (char)c;
                nextWord[1] = '\0';
                halt = !addNewSymbol(multsym, nextWord, 0);

            }
            else if ((char)c == '/' && (char)nextC != '*')
            {
                nextWord[0] = (char)c;
                nextWord[1] = '\0';
                halt = !addNewSymbol(slashsym, nextWord, 0);
            }
            else if (!isSpecialSymbols(nextC) || ((char)c == ')' && (
                (char)nextC == ';' || 
                (char)nextC == '+' || 
                (char)nextC == '-' || 
                (char)nextC == '*' || 
                (char)nextC == '/' )))
            {
                // we're done let's pack it up
                // this means we found a single char special symbol
                int currentSym = -1;
                if ((char)c == '(') currentSym = lparentsym;
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
                    error(UNKSYMBOL);
                }
                else
                {
                    nextWord[0] = (char)c;
                    nextWord[1] = '\0';
                    halt = !addNewSymbol(currentSym, nextWord, 0);
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
                nextWord[1] = (char)c;
                nextWord[2] = '\0';

                handleSpecialSymbolPair(nextWord, fid);

            }
            else
            {
                // this means we found an invalid pair.
                error(UNKSYMBOL);
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
            error(UNKSYMBOL);
            break;
        }
    }

}

// attempt to add new token
int addNewSymbol(int kind, char* name, int val)
{
    if (lexemeTableIndex + 1 >= MAX_SYMBOL_TABLE_SIZE)
    {
        error(SYMBOLTABLEFULL);
        return FALSE;
    }

    lexeme newRec;
    newRec.kind = kind;
    strcpy(newRec.name, name);
    newRec.val = val;
    newRec.line = line;

    lexemeList[lexemeTableIndex++] = newRec;

    return TRUE;
}

// print the lexeme table
void printLexemeTable(lexeme* lexemeList, int maxIndex)
{
    // header
    printf("\nLexeme Table:\n");
    printf("lexeme\ttoken type\n");

    // iterate through the table, printing the name and token value
    int i = 0;
    for (i = 0; i < maxIndex; i++)
    {
        printf("%s\t%d\n", lexemeList[i].name, lexemeList[i].kind);
    }

}

// the problem statement included two different representations of the lexeme table.
// here is the other variant.
// print the lexeme list
void printLexemeList(lexeme* lexemeList, int maxIndex)
{
    // header
    printf("\nLexeme List:\n");

    // iterate through the table, printing the name and token value
    int i = 0;
    for (i = 0; i < maxIndex; i++)
    {
        printf("%d ", lexemeList[i].kind);
            if (lexemeList[i].kind == identsym)
                printf("%s ", lexemeList[i].name);
            if (lexemeList[i].kind == numbersym)
                printf("%d ", lexemeList[i].val);
    }

    printf("\n\n");
}
