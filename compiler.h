// David M Allen
// Compiler General Header
// COP 3402 - Systems Software

// This function is called whenever there is an error.  It raises the halt flag.
extern void error(int errorCode);

// This function generates the lexeme list from the code 
extern lexeme* scan(char* filename, int* symbolTableEnd);

// prints data output from lexeme list
extern void printLexemeTable(lexeme* lexemeList, int maxIndex);
extern void printLexemeList(lexeme* lexemeList, int maxIndex);

// parser commands
extern void parse(lexeme* _lexemeList, int _maxIndex);
    
// output filename
extern char* objectCodeFileName;

// inits a new object code file
extern void initObjectCodeFile();

void setObjectCodeFileName(char* inputFile);

// processes the input arguments and verifies that they are valid
void processInputArguments(int argc, char *argv[]);

// runs the VM with the given object code
extern void run(char *objectCodeFileName);
