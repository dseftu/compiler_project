// David M Allen, Dallas Seroski 
// Parser
// COP 3402 - Systems Software

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "parser.h"
#include "errorCodes.h"

lexeme lexemeList[MAX_SYMBOL_TABLE_SIZE];
symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
instruction code[MAX_CODE_LENGTH];

int lexemeListIndex = 0;

// represents the current lexi level
int level = -1;

// a halt flag.  certain errors will raise this flag and allows the code to stop gracefully.
extern int halt;

// represents the stack pointer (data allocation)
int dx = 4;

// the current max index in the symbol table
int symbolTableIndex = 0;

// the max index of the lexemeList
int maxLexemeListIndex = -1;

// the current index of the object code
int codeIndex = 0;

// the current available register
int regIndex = 0;

// the current token
int* token;

// the value of the current token, if the current token had a value
int val;

// the current line of code that the token corresponds to
int currentLoc = 1;

// entry point for the parser
// grabs the reference to the lexeme list and starts program();
void parse(lexeme* _lexemeList, int _maxIndex)
{
	// don't continue if we are already halted
	if (halt == TRUE) exit(0);

	// assign the lexemeList to the local pointer
	*lexemeList = *_lexemeList;

	// get the maxindex
	maxLexemeListIndex = _maxIndex;

	program();
}


// the beginning of the actual parsing.
void program()
{
	getToken();	
	
	block();

	// There can be no error recovery here since this is the end of the program
	// If there happened to be valid code past this, it would not get parsed
	//if (*token != periodsym)
	//	error(MISSINGPERIOD);
	testSingle(periodsym, MISSINGPERIOD);
	//if (halt == TRUE) exit(0);

	// If we made it here, that means the parser worked
	if (halt != TRUE) printf("No errors, program is syntactically correct.\n\n");

	// End of the program
	emit(SIO_E, 0, 0, 3); 

	// this actually generates the object code file for the VM to read
	for(int i = 0; i < codeIndex; i++)
		genCode(code[i].op, code[i].r, code[i].l, code[i].m);
}

// contains the initial declarations and starts the code statements
void block()
{	

	// increment the level
	level++;
	
	// save the symboltable index so that it can be reverted after the block is finished
	int originalSymbolTableIndex = symbolTableIndex;

	// set the base data allocation to 4 (for a new activation record)
	dx = 4;
	
	// save the next code index so we can update this jump command later
	int cx = codeIndex;
	emit(JMP, 0, 0, 0);	

	// Const Declarations
	if(*token == constsym) constDeclaration();
	
	// Variable Declarations
	if(*token == varsym) dx += varDeclaration();
	
	// Procedure Declaration
	while(*token == procsym) procDeclaration();
	
	// Update that JMP command from earlier to point to the now next code index
	code[cx].m = codeIndex;
	emit(INC, 0, 0, dx);
	
	// handles the actual code inside block:
	statement();
	
	// A return statement to return to the calling function
	emit(RTN, 0, 0, 0);	
	
	// restore the symbol table index (in effect deleting out of scope symbols)
	symbolTableIndex = originalSymbolTableIndex;
	
	// decrement lexigraphical level
	level--;

}

// member of block
void constDeclaration()
{
	do
	{
		getToken();

		// constant must be followed by ident
		testSingle(identsym, MISSINGIDENTIFIER);
		//if(*token != identsym)
		//	error(MISSINGIDENTIFIER);
		//if (halt == TRUE) exit(0);

		// create the newSymbol object
		symbol newSymbol;
		newSymbol.kind = constsym;
		newSymbol.level = level;			

		// grab the symbol unique name;
		strcpy(newSymbol.name, lexemeList[lexemeListIndex-1].name);

		// const ident must be followed by =
		getToken();
		//if(*token != eqlsym)
		//	error(MISSINGCONSTASSIGNMENTSYMBOL);			
		//if (halt == TRUE) exit(0);
		testSingle(eqlsym, MISSINGCONSTASSIGNMENTSYMBOL);

		getToken();

		// const must be numeric
		//if(*token != numbersym)
		//	error(CONSTASSIGNMENTMISSING);
		//if (halt == TRUE) exit(0);
		testSingle(numbersym, CONSTASSIGNMENTMISSING);

		newSymbol.val = val; // grab the const value
		newSymbol.addr = 0;

		// add symbol to symbol table
		if (enter(newSymbol) == FALSE)
			error(AMBIGUOUSVARIABLE);
		//if (halt == TRUE) exit(0);

		getToken();			
	}
	while(*token == commasym); // continue checking for consts if comma 

	// ; expected
	//if(*token != semicolonsym)
	//	error(MISSINGSEMICOLONORBRACKET);
	//if (halt == TRUE) exit(0);
	test(statementFollowSym, MISSINGSEMICOLONORBRACKET);
	getToken();
}

// member of block
int varDeclaration()
{		
	do
	{
		
		getToken(); // this is the identifier

		// Varaible must be followed by an identifier
		//if(*token != identsym)
		//	error(MISSINGIDENTIFIER);
		//if (halt == TRUE) exit(0);
		testSingle(identsym, MISSINGIDENTIFIER);

		// create the newSymbol object
		symbol newSymbol;
		newSymbol.kind = varsym;
		newSymbol.addr = ++dx;
		newSymbol.level = level;

		// grab the symbol unique name;
		strcpy(newSymbol.name, lexemeList[lexemeListIndex-1].name);
		
		newSymbol.val = 0; // default value for a new var			

		// add symbol to symbol table
		if (enter(newSymbol) == FALSE)
			error(AMBIGUOUSVARIABLE);
		//if (halt == TRUE) exit(0);

		getToken();
	}
	while(*token == commasym);

	// ; expected
	//if(*token != semicolonsym)
	//	error(MISSINGSEMICOLONORBRACKET);
	testSingle(semicolonsym, MISSINGSEMICOLONORBRACKET);
	getToken();

	return 0;
}

//member of block
void procDeclaration()
{	

	getToken();
	//if(*token != identsym)
	//	error(MISSINGIDENTIFIER);
	testSingle(identsym, MISSINGIDENTIFIER);

	// create the newSymbol object
	symbol newSymbol;
	newSymbol.kind = procsym;
	newSymbol.addr = codeIndex;
	newSymbol.level = level;

	// grab the symbol unique name;
	int saveAddress = lexemeListIndex-1;
	strcpy(newSymbol.name, lexemeList[lexemeListIndex-1].name);		
	newSymbol.val = 0; // default value for a new var			

	// add symbol to symbol table
	if (enter(newSymbol) == FALSE)
		error(AMBIGUOUSVARIABLE);
	//if (halt == TRUE) exit(0);

	getToken();

	// ; expected
	//if(*token != semicolonsym)
	//	error(MISSINGSEMICOLONORBRACKET);
	testSingle(semicolonsym, MISSINGSEMICOLONORBRACKET);

	getToken();
	int origDx = dx;
	block();
	//dx = origDx;
	
	// ; expected
	//if(*token != semicolonsym)
	//	error(MISSINGSEMICOLONORBRACKET);
	test(blockFollowSym, MISSINGSEMICOLONORBRACKET);


	getToken();
}

// handles the various types of statements
void statement()
{
	// ident
	if(*token == identsym) identstatement();

	// Call
	else if(*token == callsym) callstatement();

	// Begin
	else if(*token == beginsym) beginstatement();

	// If
	else if(*token == ifsym) ifstatement();

	// while
	else if(*token == whilesym) whilestatement();

	// read
	else if(*token == readsym) readstatement();

	// Write
	else if(*token == writesym) writestatement();

}

// member of statement()
void callstatement()
{
	getToken();
	//if(*token != identsym)
	//	error(MISSINGIDENTAFTERCALL);
	//if (halt == TRUE) exit(0);
	testSingle(identsym, MISSINGIDENTIFIER);

	// make sure this exists in the symbol table
	int saveAddress = find(lexemeList[lexemeListIndex-1].name);
	if(saveAddress == -1)
		error(UNDECLAREDIDENT);
	if(halt == TRUE) exit(0);

	// make sure this is an actual proc
	if (symbolTable[saveAddress].kind != procsym)
		error(INVALIDCALL);
	if(halt == TRUE) exit(0);

	// need to generate the code
	
	emit(CAL, 0, level - symbolTable[saveAddress].level, symbolTable[saveAddress].addr);
	
	getToken();
}

// member of statement()
void beginstatement()
{
	getToken();
	statement();
	while(*token == semicolonsym)
	{
		getToken();
		statement();
	}
	// statement expected
	testSingle(endsym, MISSINGSTATEMENT);
	//if(*token != endsym)
	//	error(MISSINGSTATEMENT);
	//f (halt == TRUE) exit(0);
	getToken();
}

// member of statement()
void identstatement()
{
	int saveAddress = find(lexemeList[lexemeListIndex-1].name);
	if(saveAddress == -1)
		error(UNDECLAREDIDENT);
	if(halt == TRUE) exit(0);
	if (symbolTable[saveAddress].kind != varsym)
		error(INVALIDASSIGNMENT);
	if(halt == TRUE) exit(0);
	
	getToken();
	//if(*token != becomessym)
	//	error(MISSINGOPERATOR);
	//if (halt == TRUE) exit(0);
	testSingle(becomessym, MISSINGOPERATOR);

	getToken(); // variable value
	expression();
	emit(LOD, 0, 0, dx);
	emit(STO, 0, level - symbolTable[saveAddress].level, symbolTable[saveAddress].addr);		
}

// member of statement()
void ifstatement()
{
	getToken();
	condition();
	// then expected after condition
	if(*token != thensym)
		error(MISSINGTHENAFTERIF);
	//if (halt == TRUE) exit(0);
	getToken();
	int saveIndex = codeIndex;
	emit(JPC, regIndex, 0, 0);
	statement();
	code[saveIndex].m = codeIndex;
	
	if(*token == elsesym)
	{
		code[saveIndex].m = codeIndex+1;
		getToken();
		saveIndex = codeIndex;
		emit(JMP, 0, 0, 0);
		statement();
		code[saveIndex].m = codeIndex;
	}		
}

// member of statement()
void whilestatement()
{
	int index1 = codeIndex; // saves address to jump to check condition for while
	
	getToken();
	condition();
	int index2 = codeIndex;
	emit(JPC, regIndex, 0, 0);
	// do expected
	if(*token != dosym)
		error(MISSINGDO);
	//if (halt == TRUE) exit(0);
	
	getToken();
	statement();
			
	// ; missing
	if(*token !=  semicolonsym)
		error(MISSINGSEMICOLON);
	//if (halt == TRUE) exit(0);

	emit(JMP, 0, 0, index1);
	code[index2].m = codeIndex;
	
}

// member of statement()
void readstatement()
{
	getToken();	
	int i = find(lexemeList[lexemeListIndex-1].name);
	if(i == -1)
		error(UNDECLAREDIDENT);
	if(halt == TRUE) exit(0);

	if (symbolTable[i].kind != varsym)
		error(INVALIDASSIGNMENT);
	//if (halt == TRUE) exit(0);

	// read(R(0))
	// store r(0) at addr
	emit(SIO_I, 0, 0, 2);
	emit(STO, 0, level - symbolTable[i].level, symbolTable[i].addr);
	
	getToken();	
}

// member of statement()
void writestatement()
{
	getToken();
	int i = find(lexemeList[lexemeListIndex-1].name);
	if(i == -1)
		error(UNDECLAREDIDENT);
	if(halt == TRUE) exit(0);

	// put stack addr in r0
	// print r0
	emit(LOD, 0, level - symbolTable[i].level, symbolTable[i].addr);
	emit(SIO_O, 0, 0, 1);
	getToken();
}

void condition()
{
	test(conditionFirstSym, MISSINGRELATIONALOPERATOR);

	int op;
	if(*token == oddsym)
	{
		getToken();
		expression();
		emit(ODD, regIndex, 0, 0);
	}
	else
	{
		expression();
		//if (*token < eqlsym || *token > geqsym)
		//	error(MISSINGRELATIONALOPERATOR);
		test(conditionalsSym, MISSINGRELATIONALOPERATOR);
		//if (halt == TRUE) exit(0);

		int opcode;		
		if (*token == eqlsym) opcode = EQL;
		else if (*token == neqsym) opcode = NEQ;
		else if (*token == lessym) opcode = LES;
		else if (*token == leqsym) opcode = LEQ;
		else if (*token == gtrsym) opcode = GTR;
		else if (*token == geqsym) opcode = GEQ;

		getToken();
		expression();
		doMath(opcode);
	}

	test(conditionFollowSym, MISSINGRELATIONALOPERATOR);
}

void expression()
{
	test(expressionFirstSym, INVALIDSTARTTOEXPRESSION);
	if ( (*token == plussym) || (*token == minussym) )
	{
		int negate = FALSE;
		if(*token == minussym)
			negate = TRUE;
		getToken();
		if (negate == TRUE) val = -val;
	}
	term();
	int origRegIndex = regIndex;
	while ( (*token == plussym) || (*token == minussym) )
	{	
		if(*token == plussym)
		{
			getToken();
			term();
			// add the top two items on the stack and put the
			// value back ontop
			doMath(ADD);			
		}
		if(*token == minussym)
		{
			getToken();
			term();

			// sub the top two items on the stack
			doMath(SUB);
		}
	}
	regIndex = origRegIndex;

}

void term()
{
	test(termFirstSym, INVALIDSTARTTOEXPRESSION);
	int origRegIndex = regIndex;
	int mulop;
	factor();

	while ( (*token == multsym) || (*token == slashsym) )
	{
		mulop = *token;
		getToken();
		factor();

		if(mulop == multsym)
		{
			// multiply the top two items on the stack
			doMath(MUL);
		}
		else
		{
			// divide the top two items on the stack
			doMath(DIV);
		}
	}
	regIndex = origRegIndex;

}

void factor()
{
	test(factFirstSym, INVALIDSTARTTOFACTOR);
	// identifier
	if(*token == identsym)
	{
		int i = find(lexemeList[lexemeListIndex-1].name);
		if(i == -1)
			error(UNDECLAREDIDENT);
		if(halt == TRUE) exit(0);

		if (symbolTable[i].kind != constsym && symbolTable[i].kind != varsym)
			error(BADUSEOFPROCIDENT);

		// put the value (either constant or var) into reg 0
		if (symbolTable[i].kind == constsym)
			emit(LIT, 0, 0, symbolTable[i].val);			
		else
			emit(LOD, 0, level - symbolTable[i].level, symbolTable[i].addr);

		// put the contents of reg 0 onto the top of the stack	
		emit(INC, 0, 0, 1);	
		emit(STO, 0, 0, ++dx);		

		getToken();
	}
	else if(*token == numbersym)
	{
		getToken(); // number retrieved		
		emit(LIT, 0, 0, val); // number loaded into register 0		
		emit(INC, 0, 0, 1); // incremenets the  stack by 1
		emit(STO, 0, 0, dx+1); // moves it onto the stack
		dx++;
	}
	else if(*token == lparentsym)
	{
		getToken();
		expression();

		// ) missing
		if(*token != rparentsym)
			error(MISSINGRIGHTPAREN);
		//if (halt == TRUE) exit(0);
		getToken();

		move(dx-1, dx-2);
	}

	//can't begin with this symbol
	//else
	//	error(INVALIDSTARTTOFACTOR);
	//if (halt == TRUE) exit(0);
}

// Check if token is a constant, variable, or procedure
// Returns 1 if Const, 2 if var, 3 is proc, 0 if non
int type(char* str)
{
	int i;
	for (i = lexemeListIndex-1; i >= 0; i--)
	{
		if (strcmp(str, lexemeList[i].name) == 0 && lexemeList[i].kind == constsym)
			return constsym;
		if (strcmp(str, lexemeList[i].name) == 0 && lexemeList[i].kind == varsym)
			return varsym;
		if (strcmp(str, lexemeList[i].name) == 0 && lexemeList[i].kind == procsym)
			return procsym;
	}
	return 0;
}

// adds a new line of code to the array
void emit(int op, int r, int l, int m)
{
	code[codeIndex].op = op;
	code[codeIndex].r = r;
	code[codeIndex].l = l;
	code[codeIndex].m = m;
	codeIndex++;
}

// similar to an ALU.  This generates the code for any arithmetic and logical binary operations.
void doMath(int opcode)
{
	
	// declaring this for clarity
	int topTermReg = regIndex++;
	int topTermStackLoc = dx;
	int btmTermReg = regIndex;
	int btmTermStackLoc = dx-1;

	if (regIndex>=MAX_REGISTER_SIZE)
		error(OUTOFREGISTERSPACE);
	//if (halt == TRUE) exit(0);

	emit(LOD, topTermReg, 0, topTermStackLoc); // this is the most recent item
	emit(LOD, btmTermReg, 0, btmTermStackLoc); // this should be the item under that
	emit(opcode, btmTermReg, btmTermReg, topTermReg);
	emit(STO, btmTermReg, 0, topTermStackLoc);
}

// gets an item from the stack and places it in another spot on the stack.  Leaves original intact.
void move(int from, int to)
{
	emit(LOD, 0, level, from);
	emit(STO, 0, level, to);
}

// retrieves the next token
void getToken()
{
	if (lexemeListIndex > maxLexemeListIndex) exit(0);

	token = &lexemeList[lexemeListIndex].kind;
	if(*token == numbersym)
		val = lexemeList[lexemeListIndex].val;
	currentLoc = lexemeList[lexemeListIndex].line;
	lexemeListIndex++;
}

// initializes the code array
void initcode()
{
	int i;
	for(i = 0; i < MAX_CODE_LENGTH; i++)
	{
		code[i].op = 0;
		code[i].r = 0;
		code[i].l = 0;
		code[i].m = 0;
	}
}

// Exist function for checking if symbol is already in table
int exist(symbol s)
{
	int i;
	for(i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++)
	{
		if(s.kind == symbolTable[i].kind && strcmp(s.name, symbolTable[i].name) == 0
		   && s.level == symbolTable[i].level)
			return TRUE;
	}
	return FALSE;
}

// adds a new symbol to the symbol table.  Returns TRUE on success.
int enter(symbol s)
{
	// if this symbol already exists, then don't add it
	if (exist(s) == TRUE) return FALSE;

	if (symbolTableIndex >= MAX_SYMBOL_TABLE_SIZE)
		error(SYMBOLTABLEFULL);
	if (halt == TRUE) exit(0);

	symbolTable[symbolTableIndex++] = s;
	return TRUE;
} 

// finds a symbol by name
// returns location of symbol in table if found
// returns -1 if not founds 
int find(char* name)
{
	for(int i = symbolTableIndex-1; i >= 0; i--)
		if(strcmp(symbolTable[i].name, name) == 0) return i;			
	
	return -1;
}

// prints the symbol table.  Useful in debugging.  Prints backwards.
void print()
{
	printf("Symbol table index = %d\n", symbolTableIndex);
	for(int i = symbolTableIndex-1; i >= 0; i--)
	{
		int a = symbolTable[i].addr;
		int k = symbolTable[i].kind;
		char* n = symbolTable[i].name;
		int l = symbolTable[i].level;
		int v = symbolTable[i].val;
		printf("Name = %s, addr = %d, kind = %d, level = %d, val = %d\n", n, a, k, l, v);
	}
}

void test(int* validset, int errorcode)
{
	
	if (!memberOf(*token, validset))
	{
		printf("\nError (%d)\n", errorcode);
		error(errorcode);
		while (!(memberOf(*token, validset))) getToken();
	}
}

void testSingle(int validsym, int errorcode)
{
	if (*token != validsym)
	{
		printf("\nError (%d)\n", errorcode);
		error(errorcode);
		while (*token != validsym) getToken();
	}
}

int memberOf(int sym, int* set)
{
	int i = 0;
	while (set[i] != setendsymbol)
		if (set[i++] == sym) return TRUE;
	
	return FALSE;
}
