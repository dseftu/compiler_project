// David M Allen
// Compiler General Header
// COP 3402 - Systems Software

// This function is called whenever there is an error.  It raises the halt flag.
extern void error(int errorCode);

// This function generates the lexeme list from the code 
extern namerecord_t* scan(char* filename, int* symbolTableEnd);

// prints data output from lexeme list
extern void printLexemeTable(namerecord_t* lexemeList, int maxIndex);
extern void printLexemeList(namerecord_t* lexemeList, int maxIndex);




