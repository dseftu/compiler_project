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
int table = 0;
int level = -1;
int ident;
int procadd = 0;
extern int halt;
int sp = 3;

int symbolTableIndex = 0;
int codeIndex = 0;
int regIndex = 0;
// max index of the lexemeList.  When getToken() is called
// the index is checked against this to ensure we don't go off the wheels
int maxIndex = -1;

// the current token
int* token;
int val; 

void getToken()
{
	// ensure that we aren't over the maxIndex
	token = &lexemeList[table].kind;
	if(*token == numbersym)
		val = lexemeList[table].val;
	table++;
}

void parse(lexeme* _lexemeList, int _maxIndex)
{
	if (halt == TRUE) exit(0);
	*lexemeList = *_lexemeList;
	maxIndex = _maxIndex;
	program();
}

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
	for(int i = 0; i < symbolTableIndex; i++)
	{
		if(strcmp(symbolTable[i].name, name) == 0)
			return i;
	}
	return -1;
}

int cons = 0, proc = 0;
void program()
{
	getToken();
	emit(INC, 0, 0, sp);
	block();
	if (*token != periodsym)
		error(MISSINGPERIOD);
	if (halt == TRUE) exit(0);
	printf("No errors, program is syntactically correct.\n\n");
	emit(SIO_E, 0, 0, 3); // End of the program
	int i;
	for(i = 0; i < codeIndex; i++)
	{
		genCode(code[i].op, code[i].r, code[i].l, code[i].m);
	}

}


void block()
{	
	level++;
	procadd++;
	// This if statement handles constants
	if(*token == constsym)
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
			strcpy(newSymbol.name, lexemeList[table-1].name);

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

	// Variable Declarations
	if(*token == varsym)
	{		
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
			strcpy(newSymbol.name, lexemeList[table-1].name);
			
			newSymbol.val = 0; // default value for a new var			

			// add symbol to symbol table
			if (enter(newSymbol) == FALSE)
				error(AMBIGUOUSVARIABLE);
			if (halt == TRUE) exit(0);

			emit(INC, 0, 0, 1); // increment the stack
			emit(LIT, 0, 0, 0); // Puts default value in register (use reg 0 for this)
			emit(STO, 0, 0, sp); // store on stack and increment			
			sp++;
			getToken();		
		}
		while(*token == commasym);

		// ; expected
		if(*token != semicolonsym)
			error(MISSINGSEMICOLONORBRACKET);
		getToken();
	}

	// Procedure Declaration
	while(*token == procsym)
	{
		error(NOTIMPLEMENTED);
		exit(0);
	}
	
	statement();
	level--;
}

void statement()
{
	int i, j, add1, add2, add3, add4;

	// identifier
	// Assigning value to variables
	if(*token == identsym)
	{
		int saveAddress = find(lexemeList[table-1].name);
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
		emit(LOD, 0, 0, sp-1);
		emit(STO, 0, 0, symbolTable[saveAddress].addr);		
	}

	// Call
	else if(*token == callsym)
	{
		// throws error because we are not implementing call
		error(NOTIMPLEMENTEDCALL);
	}

	// Begin
	else if(*token == beginsym)
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

	// If
	else if(*token == ifsym)
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
		getToken();
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

	// while
	else if(*token == whilesym)
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

	// read
	else if(*token == readsym)
	{
		getToken();	
		ident = find(lexemeList[table-1].name);
		if(ident == -1)
			error(UNDECLAREDIDENT);
		if(halt == TRUE) exit(0);

		if (symbolTable[ident].kind != varsym)
			error(INVALIDASSIGNMENT);
		if (halt == TRUE) exit(0);

		// read(R(0))
		// store r(0) at addr
		emit(SIO_I, 0, 0, 2);
		emit(STO, 0, 0, symbolTable[ident].addr);
		
		getToken();	
	}

	// Write
	else if(*token == writesym)
	{
		getToken();
		ident = find(lexemeList[table-1].name);
		if(ident == -1)
			error(UNDECLAREDIDENT);
		if(halt == TRUE) exit(0);

		// put stack addr in r0
		// print r0
		emit(LOD, 0, 0, symbolTable[ident].addr);
		emit(SIO_O, 0, 0, 1);
		getToken();
	}
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
	int i, j;
	// identifier
	if(*token == identsym)
	{
		ident = find(lexemeList[table-1].name);
		if(ident == -1)
			error(UNDECLAREDIDENT);
		if(halt == TRUE) exit(0);

		if (symbolTable[ident].kind != constsym && symbolTable[ident].kind != varsym)
			error(BADUSEOFPROCIDENT);

		emit(LOD, 0, 0, symbolTable[ident].addr);
		emit(STO, 0, 0, sp);
		emit(INC, 0, 0, 1);
		sp++;
		

		getToken();

	}
	else if(*token == numbersym)
	{
		getToken(); // number retrieved
		
		emit(LIT, 0, 0, val); // number loaded into stack
		emit(STO, 0, 0, sp++);
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
	for (i = 0; i < table; i++)
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

void emit(int op, int r, int l, int m)
{
	code[codeIndex].op = op;
	code[codeIndex].r = r;
	code[codeIndex].l = l;
	code[codeIndex].m = m;
	codeIndex++;
}

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

	emit(LOD, topTermReg, 0, topTermStackLoc); // this is the most recent item
	emit(LOD, btmTermReg, 0, btmTermStackLoc); // this should be the item under that
	emit(opcode, btmTermReg, btmTermReg, topTermReg);
	emit(STO, btmTermReg, 0, topTermStackLoc);
}

// negates
void push(int value)
{
	emit(INC, 0, 0, 1); // increment the stack
	emit(LIT, 0, 0, value); // Puts default value in register (use reg 0 for this)
	emit(STO, 0, 0, sp++); // store on stack and increment			
}

void move(int from, int to)
{
	emit(LOD, 0, 0, from);
	emit(STO, 0, 0, to);
}