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

// represents the stack pointer
int sp = 4;

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
int val;

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
	if (*token != periodsym)
		error(MISSINGPERIOD);
	if (halt == TRUE) exit(0);

	// If we made it here, that means the parser worked
	printf("No errors, program is syntactically correct.\n\n");

	// End of the program
	emit(SIO_E, 0, 0, 3); 

	// this actually generates the object code file for the VM to read
	for(int i = 0; i < codeIndex; i++)
		genCode(code[i].op, code[i].r, code[i].l, code[i].m);
}

void block()
{	
	// increment the level
	level++;
	int space = 4;
	int cx = codeIndex;
	emit(JMP, 0, 0, 0);

	
	
	// Handle the declarations:

	// This if statement handles constants
	if(*token == constsym) constDeclaration();

	// Variable Declarations
	if(*token == varsym) space += varDeclaration();

	// Procedure Declaration
	while(*token == procsym) procDeclaration();

	code[cx].m = codeIndex;

	emit(INC, 0, 0, space);
	sp+=space;

	// handles the actual code:
	statement();

	emit(RTN, 0, 0, 0);
	// decrement level
	level--;
}

void constDeclaration()
{
	do
	{
		getToken();

		// constant must be followed by ident
		if(*token != identsym)
			error(MISSINGIDENTIFIER);
		if (halt == TRUE) exit(0);

		// create the newSymbol object
		symbol newSymbol;
		newSymbol.kind = constsym;
		newSymbol.level = level;			

		// grab the symbol unique name;
		strcpy(newSymbol.name, lexemeList[lexemeListIndex-1].name);

		// const ident must be followed by =
		getToken();
		if(*token != eqlsym)
			error(MISSINGCONSTASSIGNMENTSYMBOL);			
		if (halt == TRUE) exit(0);
		getToken();

		// const must be numeric
		if(*token != numbersym)
			error(CONSTASSIGNMENTMISSING);
		if (halt == TRUE) exit(0);

		newSymbol.val = val; // grab the const value
		newSymbol.addr = sp;

		// add symbol to symbol table
		if (enter(newSymbol) == FALSE)
			error(AMBIGUOUSVARIABLE);
		if (halt == TRUE) exit(0);

		push(val); // pushes the constant on the stack
		getToken();			
	}
	while(*token == commasym); // continue checking for consts if comma 

	// ; expected
	if(*token != semicolonsym)
		error(MISSINGSEMICOLONORBRACKET);
	if (halt == TRUE) exit(0);
	getToken();
}

int varDeclaration()
{		
	int space = 0;
	do
	{
		
		getToken(); // this is the identifier

		// Varaible must be followed by an identifier
		if(*token != identsym)
			error(MISSINGIDENTIFIER);
		if (halt == TRUE) exit(0);

		// create the newSymbol object
		symbol newSymbol;
		newSymbol.kind = varsym;
		newSymbol.addr = sp;
		newSymbol.level = level;

		// grab the symbol unique name;
		strcpy(newSymbol.name, lexemeList[lexemeListIndex-1].name);
		
		newSymbol.val = 0; // default value for a new var			

		// add symbol to symbol table
		if (enter(newSymbol) == FALSE)
			error(AMBIGUOUSVARIABLE);
		if (halt == TRUE) exit(0);

		//emit(INC, 0, 0, 1); // increment the stack
		//emit(LIT, 0, 0, 0); // Puts default value in register (use reg 0 for this)
		//emit(STO, 0, level, sp); // store on stack and increment			
		//sp++;
		space++;
		getToken();		
	}
	while(*token == commasym);

	// ; expected
	if(*token != semicolonsym)
		error(MISSINGSEMICOLONORBRACKET);
	getToken();

	return space;
}

void procDeclaration()
{	

	getToken();
	if(*token != identsym)
		error(MISSINGIDENTIFIER);

	// create the newSymbol object
	symbol newSymbol;
	newSymbol.kind = procsym;
	newSymbol.addr = codeIndex+1;
	newSymbol.level = level;

	// grab the symbol unique name;
	int saveAddress = lexemeListIndex-1;
	strcpy(newSymbol.name, lexemeList[lexemeListIndex-1].name);		
	newSymbol.val = 0; // default value for a new var			

	// add symbol to symbol table
	if (enter(newSymbol) == FALSE)
		error(AMBIGUOUSVARIABLE);
	if (halt == TRUE) exit(0);

	getToken();

	// ; expected
	if(*token != semicolonsym)
		error(MISSINGSEMICOLONORBRACKET);

	getToken();
	block();

	

	// ; expected
	if(*token != semicolonsym)
		error(MISSINGSEMICOLONORBRACKET);

	getToken();
}

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

void callstatement()
{
	getToken();
	if(*token != identsym)
		error(MISSINGIDENTAFTERCALL);
	if (halt == TRUE) exit(0);

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
	if(*token != endsym)
		error(MISSINGSTATEMENT);
	if (halt == TRUE) exit(0);
	getToken();
}

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
	if(*token != becomessym)
		error(MISSINGOPERATOR);
	if (halt == TRUE) exit(0);

	getToken(); // variable value
	expression();
	emit(LOD, 0, level, sp-1);
	emit(STO, 0, level - symbolTable[saveAddress].level, symbolTable[saveAddress].addr);		
}

void ifstatement()
{
	getToken();
	condition();
	// then expected after condition
	if(*token != thensym)
		error(MISSINGTHENAFTERIF);
	if (halt == TRUE) exit(0);
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
	if (halt == TRUE) exit(0);
	
	getToken();
	statement();
			
	// ; missing
	if(*token !=  semicolonsym)
		error(MISSINGSEMICOLON);
	if (halt == TRUE) exit(0);

	emit(JMP, 0, 0, index1);
	code[index2].m = codeIndex;
	
}

void readstatement()
{
	getToken();	
	int i = find(lexemeList[lexemeListIndex-1].name);
	if(i == -1)
		error(UNDECLAREDIDENT);
	if(halt == TRUE) exit(0);

	if (symbolTable[i].kind != varsym)
		error(INVALIDASSIGNMENT);
	if (halt == TRUE) exit(0);

	// read(R(0))
	// store r(0) at addr
	emit(SIO_I, 0, 0, 2);
	emit(STO, 0, level - symbolTable[i].level, symbolTable[i].addr);
	
	getToken();	
}

void writestatement()
{
	getToken();
	int i = find(lexemeList[lexemeListIndex-1].name);
	printf("LOOKING AT %s at %d\n", lexemeList[lexemeListIndex-1].name, i);
	if(i == -1)
		error(UNDECLAREDIDENT);
	if(halt == TRUE) exit(0);

	// put stack addr in r0
	// print r0
	emit(LOD, 0, symbolTable[i].level, symbolTable[i].addr);
	emit(SIO_O, 0, 0, 1);
	getToken();
}

void condition()
{
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
		if (*token < eqlsym || *token > geqsym)
			error(MISSINGRELATIONALOPERATOR);
		if (halt == TRUE) exit(0);

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
}

void expression()
{
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
	// identifier
	if(*token == identsym)
	{
		int i = find(lexemeList[lexemeListIndex-1].name);
		if(i == -1)
			error(UNDECLAREDIDENT);
		if(halt == TRUE) exit(0);

		if (symbolTable[i].kind != constsym && symbolTable[i].kind != varsym)
			error(BADUSEOFPROCIDENT);

		emit(LOD, 0, level - symbolTable[i].level, symbolTable[i].addr);
		emit(STO, 0, level - symbolTable[i].level, sp);
		emit(INC, 0, 0, 1);
		sp++;	

		getToken();

	}
	else if(*token == numbersym)
	{
		getToken(); // number retrieved
		
		emit(LIT, 0, 0, val); // number loaded into stack
		emit(STO, 0, level, sp++);
		emit(INC, 0, 0, 1);
	}
	else if(*token == lparentsym)
	{
		int saveSP = sp-1;
		getToken();
		expression();
		// ) missing
		if(*token != rparentsym)
			error(MISSINGRIGHTPAREN);
		if (halt == TRUE) exit(0);
		getToken();

		move(saveSP, sp-2);
	}

	//can't begin with this symbol
	else
		error(INVALIDSTARTTOFACTOR);
	if (halt == TRUE) exit(0);
}

// Check if token is a constant, variable, or procedure
// Returns 1 if Const, 2 if var, 3 is proc, 0 if non
int type(char* str)
{
	int i;
	for (i = 0; i < lexemeListIndex; i++)
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
	int topTermStackLoc = sp-1;
	int btmTermReg = regIndex;
	int btmTermStackLoc = sp-2;

	if (regIndex>=MAX_REGISTER_SIZE)
		error(OUTOFREGISTERSPACE);
	if (halt == TRUE) exit(0);

	emit(LOD, topTermReg, level, topTermStackLoc); // this is the most recent item
	emit(LOD, btmTermReg, level, btmTermStackLoc); // this should be the item under that
	emit(opcode, btmTermReg, btmTermReg, topTermReg);
	emit(STO, btmTermReg, level, topTermStackLoc);
}

// pushs a value on top of the stack.  Useful for const declarations.
void push(int value)
{
	emit(INC, 0, 0, 1); // increment the stack
	emit(LIT, 0, 0, value); // Puts default value in register (use reg 0 for this)
	emit(STO, 0, level, sp++); // store on stack and increment			
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
	token = &lexemeList[lexemeListIndex].kind;
	if(*token == numbersym)
		val = lexemeList[lexemeListIndex].val;
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
	if (exist(s) == TRUE) 
	{
		
		return FALSE;
	}
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
	{
		if(strcmp(symbolTable[i].name, name) == 0)
			return i;
	}
	return -1;
}