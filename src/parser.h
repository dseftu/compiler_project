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
void constDeclaration();
int varDeclaration();
void procDeclaration();
void callstatement();
void beginstatement();
void identstatement();
void ifstatement();
void whilestatement();
void readstatement();
void writestatement();
void print();


extern int halt;

extern void genCode(int op, int r, int l, int m);
void buildTestObjectCode();
void test(int* validset, int errorcode);
void testSingle(int validsym, int errorcode);
int memberOf(int sym, int* set);


int blockFirstSym[] = {constsym, varsym, procsym, identsym, ifsym, callsym, beginsym, whilesym, setendsymbol};
int statementFirstSym[] = {identsym, callsym, beginsym, ifsym, whilesym, setendsymbol};
int conditionFirstSym[] = {oddsym, plussym, minussym, lparentsym, identsym, numbersym, setendsymbol};
int expressionFirstSym[] = {plussym, minussym, lparentsym, identsym, numbersym, setendsymbol};
int termFirstSym[] = {identsym, numbersym, lparentsym, setendsymbol};
int factFirstSym[] = {identsym, numbersym, lparentsym, setendsymbol};

int conditionalsSym[] = {eqlsym, neqsym, lessym, leqsym, gtrsym, geqsym};

int blockFollowSym[] = {periodsym, semicolonsym, setendsymbol};
int statementFollowSym[] = {periodsym, semicolonsym, endsym, setendsymbol};
int conditionFollowSym[] = {thensym, dosym, setendsymbol};
int expressionFollowSym[] = {periodsym, semicolonsym, rparentsym, endsym, thensym, dosym, setendsymbol};
int termFollowSym[] = {periodsym, semicolonsym, rparentsym, plussym, minussym, endsym, thensym, dosym, setendsymbol};
int factFollowSym[] = {periodsym, semicolonsym, rparentsym, plussym, minussym, multsym, slashsym, endsym, thensym, dosym, setendsymbol};