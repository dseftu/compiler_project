// David M Allen
// Scanner
// COP 3402 - Systems Software

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    char name[10];	// name up to 11 chars
    int val; 		// number (ASCII value) 
    int level; 		// L  level
    int adr; 		// M  address
} namerecord_t; 

namerecord_t symbol_table[MAX_SYMBOL_TABLE_SIZE];
void prepareSpecialSymbols();
void readInput(char *filename);

int main(int argc, char *argv[])
{
    // TODO: EVERYTHING
    if (argc != 2)
    {
        printf("Invalid number of arguments given.\n\nExpected:\nP-machine <input file>\n");
        return 0;
    }

    // prepare the Special Symbol table
    prepareSpecialSymbols();

    // read in the input file
    readInput(argv[1]);

    return 0;
}

void prepareSpecialSymbols()
{
    ssym['+']=plussym;	
    ssym['-']=minussym;
    ssym['*']=multsym; 	
    ssym['/']=slashsym;
    ssym['(']=lparentsym;
    ssym[')']=rparentsym; 
    ssym['=']=eqlsym;
    ssym[',']=commasym;
    ssym['.']=periodsym; 	
    ssym['#']=neqsym;
    ssym['<']=lessym;
    ssym['>']=gtrsym; 
    ssym['$']=leqsym;
    ssym['%']=geqsym;
    ssym[';']=semicolonsym; 
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
    }
    else
    {
        int c;
        while (( c = getc(fid)) != EOF)
        {
            putchar(c);
        }
        fclose(fid);
    }
}