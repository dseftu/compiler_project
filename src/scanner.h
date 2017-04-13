// David M Allen
// Scanner Header
// COP 3402 - Systems Software

#ifndef SCANNER_H
#define SCANNER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

extern int halt;
int loc = 1; // line of code that the lexeme appears on

lexeme lexemeList[MAX_SYMBOL_TABLE_SIZE];
int lexemeTableIndex = 0;

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
lexeme* scan(char* filename, int* _lexemeListEnd);

// makes a copy of the next char in the file
char peekC(FILE *fid);

// generically adds to the symbol table.  returns FALSE if fails
int addNewSymbol(int kind, char* name, int val);

// handles situations with two consec. symbols.  raises halt if fails
void handleSpecialSymbolPair(char* word, FILE*fid);

// prints data output
void printLexemeTable(lexeme* lexemeList, int maxIndex);
void printLexemeList(lexeme* lexemeList, int maxIndex);

// This function is called whenever there is an error.  It raises the halt flag.
extern void error(int errorCode);

#endif