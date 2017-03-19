// David M Allen, Dallas Seroski 
// Parser Header
// COP 3402 - Systems Software

// This function is called whenever there is an error.  It raises the halt flag.
extern void error(int errorCode);

void getToken();
void parse(lexeme* _lexemeList, int _maxIndex);
void program();
void block();
void statement();
int type();
void condition();
void expression();
void term();
void factor();
//int relop();

extern int halt;

extern void genCode(int op, int r, int l, int m);
void buildTestObjectCode();