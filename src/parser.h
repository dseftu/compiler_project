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
int type();
void initcode();
void emit(int op, int r, int l, int m);
void doMath(int opcode);
void push(int value);
void move(int from, int to);
int enter(symbol s);
int find(char* name);
int constDeclaration();
int varDeclaration();
void procDeclaration();
void callstatement();
void beginstatement();
void identstatement();
void ifstatement();
void whilestatement();
void readstatement();
void writestatement();


extern int halt;

extern void genCode(int op, int r, int l, int m);
void buildTestObjectCode();