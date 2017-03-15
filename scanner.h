// David M Allen
// Scanner Header
// COP 3402 - Systems Software

#ifndef SCANNER_H
#define SCANNER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

extern int halt;
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
void scanInput(char *filename);
namerecord_t* scan(char* filename, int* symbolTableEnd);

// makes a copy of the next char in the file
char peekC(FILE *fid);

// generically adds to the symbol table.  returns FALSE if fails
int addNewSymbol(int kind, char* name, int val, int level, int adr);

// handles situations with two consec. symbols.  raises halt if fails
void handleSpecialSymbolPair(char* word, FILE*fid);

// prints data output
void printLexemeTable(namerecord_t* lexemeList, int maxIndex);
void printLexemeList(namerecord_t* lexemeList, int maxIndex);

#endif