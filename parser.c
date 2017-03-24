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
int registers[REG];
int table = 0;
int level = -1;
int ident;
int procadd = 0;
extern int halt;

int symbolTableIndex = 0;
int codeIndex = 0;
int regIndex = 0;
// max index of the lexemeList.  When getToken() is called
// the index is checked against this to ensure we don't go off the wheels
int maxIndex = -1;

// the current token
int* token;

void getToken()
{
	// ensure that we aren't over the maxIndex
	token = &lexemeList[table++].kind;
}

void parse(lexeme* _lexemeList, int _maxIndex)
{
	if (halt == TRUE) exit(0);
	*lexemeList = *_lexemeList;
	maxIndex = _maxIndex;
	program();

	// TODO: Remove this once real object code is being generated
	//buildTestObjectCode();
}

// TODO: Remove this once real object code is being generated
// this is builds an example object code since we don't have that implemented
// in the parser yet
/*
void buildTestObjectCode()
{
    genCode(INC, 0, 0, 6);
	genCode(LIT, 0, 0, 6);
	genCode(STO, 0, 0, 4);
	genCode(LOD, 0, 0, 4);
	genCode(LIT, 1, 0, 0);
	genCode(GTR, 0, 0, 1);
	genCode(JPC, 0, 0, 17);
	genCode(LOD, 0, 0, 4);
	genCode(MUL, 0, 0, 0);
	genCode(STO, 0, 0, 5);
	genCode(LOD, 0, 0, 4);
	genCode(LIT, 1, 0, 1);
	genCode(SUB, 0, 0, 1);
	genCode(STO, 0, 0, 4);
	genCode(LOD, 0, 0, 5);
	genCode(SIO_O, 0, 0, 1);
	genCode(JMP, 0, 0, 3);
	genCode(LOD, 0, 0, 4);
	genCode(SIO_O, 0, 0, 1);
	genCode(LOD, 0, 0, 5);
	genCode(SIO_O, 0, 0, 1);
	genCode(RTN, 0, 0, 0);
}
*/
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
		   && s.val == symbolTable[i].val && s.addr == symbolTable[i].addr)
			return TRUE;
	}
	return FALSE;
}

int enter(symbol s)
{
	// if this symbol already exists, then don't add it
	if (exist(s) == TRUE) return FALSE;
	symbolTable[symbolTableIndex++] = s;
	return TRUE;
} 

// finds a symbol by name
// returns location of symbol in table if found
// returns -1 if not founds 
int find(char* name)
{
	//TODO this
	int i;
	for(i = 0; i < symbolTableIndex; i++)
	{
		if(strcmp(symbolTable[i].name, name) == 0)
			return i;
	}
	return -1;

}

void program()
{
	getToken();
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
	int var = 0, cons = 0, proc = 0;
	level++;
	procadd++;
	// This if statement handles constants
	if(*token == constsym)
	{
		do
		{
			getToken();

			// create the newSymbol object
			symbol newSymbol;
			newSymbol.kind = constsym;
			newSymbol.addr = 4+var;
			var++;
			newSymbol.level = level;

			// constant must be followed by ident
			if(*token != identsym)
				error(MISSINGIDENTIFIER);
			if (halt == TRUE) exit(0);

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

			newSymbol.val = *token; // grab the const value

			if (enter(newSymbol) == FALSE)
				error(-1); // TODO does this need an error here?
			if (halt == TRUE) exit(0);
			emit(STO, regIndex++, 0, *token); // Puts constant in register
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

			// create the newSymbol object
			symbol newSymbol;
			newSymbol.kind = varsym;

			// grab the symbol unique name;
			strcpy(newSymbol.name, lexemeList[table-1].name);
			newSymbol.addr = 4+var;
			newSymbol.level = level;
			newSymbol.val = 0; // default value for a new var
			var++;

			if (enter(newSymbol) == FALSE)
				error(-1); // TODO does this need an error here?
			if (halt == TRUE) exit(0);


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
		// Procedure must be followed by an identifier
		getToken();
		if(*token != identsym)
			error(MISSINGIDENTIFIER);
		if (halt == TRUE) exit(0);

		// create the newSymbol object
		symbol newSymbol;
		newSymbol.kind = procsym;

		// grab the symbol unique name;
		strcpy(newSymbol.name, lexemeList[table-1].name);
		newSymbol.addr = procadd;
		newSymbol.level = level;
		newSymbol.val = 0; // default value for a new var
		proc++;

		if (enter(newSymbol) == FALSE)
			error(-1); // TODO does this need an error here?
		if (halt == TRUE) exit(0);

		getToken();

		// ; expected
		if(*token != semicolonsym)
			error(MISSINGSEMICOLONORCOMMA);
		if (halt == TRUE) exit(0);
		getToken();
		block();
		// ; expected
		if(*token != semicolonsym)
			error(MISSINGSEMICOLONORCOMMA);
		if (halt == TRUE) exit(0);
		getToken();
	}

	statement();
	level--;
}

// TODO
void statement()
{
	int i, j, add1, add2;

	// identifier
	if(*token == identsym)
	{
		getToken();

		if(*token != becomessym)
			error(MISSINGOPERATOR);
		if (halt == TRUE) exit(0);
	
		getToken(); // varabile value
		expression();
		
		// TODO CHECK FOR ASSIGNMENT TO CONSTANT?
	}

	// Call
	else if(*token == callsym)
	{
		getToken();
		// call must be followed by an identifier
		if(*token != identsym)
			error(MISSINGIDENTAFTERCALL);
		if (halt == TRUE) exit(0);
		getToken();

		// Procedure calling not implemented in this version

		// TODO Check for valid call: 
		/*
		//checks type
		j = type();
		// call of a constant or variable meaningless
		if(j == 1 || j == 2)
			error(INVALIDCALL);
		// undeclared identifier
		else if(j != 3)
			error(UNDECLAREDIDENT);
		for(i = 0; i < table; i++)
		{
			if(strcmp(str, lexemeList[i].name) == 0)
				ident = i;
		}
		getToken();
		// ; missing
		if(*token != semicolonsym)
			error(MISSINGSEMICOLON);
		*/
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
		statement();

		/* Might not need this
		// ; missing
		if(*token != semicolonsym)
			error(MISSINGSEMICOLON);
		getToken()
		*/
		
		/* else not implemented in this version
		if(*token == elsesym)
		{
			getToken();
			statement();
			// ; mising
			if(*token != semicolonsym)
				error(MISSINGSEMICOLON);
		}
		else
			//fseek(lex, -(strlen(str) + 1), SEEK_CUR);
		*/
	}

	// while
	else if(*token == whilesym)
	{
		add1 = codeIndex; // saves address to jump to check condition for while
		getToken();
		condition();
		add2 = codeIndex; // saves address for inside the while loop
		emit(JPC, 0, 0, 0);
		// do exepected
		if(*token != dosym)
			error(MISSINGDO);
		if (halt == TRUE) exit(0);
		getToken();
		statement();
		emit(JMP, 0, 0, add1);
		
		// ; missing
		if(*token !=  semicolonsym)
			error(MISSINGSEMICOLON);
		// loop finishes get address for next line
		code[add2].m = codeIndex;
	}

//TODO implement read/write
	// read
	else if(*token == readsym)
	{
		getToken();
		// needs to be identifier
		if(*token != identsym)
			error(TOOLARGENUMBER);
		getToken();

		j = type();
		// can't assign to a const or proc
		if(j == 1 || j == 3)
			error(INVALIDASSIGNMENT);
		// undeclared ident
		else if(j != 2)
			error(UNDECLAREDIDENT);
		for(i = 0; i < table; i++)
		{
			if(*token == lexemeList[i].val)
				ident = i;
		}
		emit(SIO_I, 0, 0, 2);
		emit(STO, regIndex, 0, symbolTable[ident].val);
		regIndex--;
		getToken();
		// ; missing
		if(*token != semicolonsym)
			error(MISSINGSEMICOLON);
	}

	// Write
	else if(*token == writesym)
	{
		getToken();
		if(*token != identsym)
			error(TOOLARGENUMBER);
		getToken();
		j = type();
		// undeclared identifier
		if(j > 3 || j < 1)
			error(UNDECLAREDIDENT);
		emit(LOD, regIndex, 0, symbolTable[ident].val);
		emit(SIO_O, regIndex, 0, 1);
		getToken();
		// ; missing
		if(*token != semicolonsym)
			error(MISSINGSEMICOLON);
	}
}




// TODO
void condition()
{
	int op;
	if(*token == oddsym)
	{
		getToken();
		expression();
	}
	else
	{
		expression();
		if (*token < eqlsym || *token > geqsym)
			error(MISSINGRELATIONALOPERATOR);
		if (halt == TRUE) exit(0);
		getToken();
		expression();
	}
}

// TODO
void expression()
{
	if ( (*token == plussym) || (*token == minussym) )
	{
		getToken();
	}
	term();
	while ( (*token == plussym) || (*token == minussym) )
	{	
		if(*token == plussym)
			emit(ADD, registers[regIndex], registers[regIndex], registers[regIndex-1]);
		if(*token == minussym)
			emit(SUB, registers[regIndex], registers[regIndex], registers[regIndex-1]);
		getToken();
		term();
	}
}

// TODO
void term()
{
	factor();
	while ( (*token == multsym) || (*token == slashsym) )
	{
		getToken();
		factor();

		if(*token == multsym)
			emit(MUL, regIndex, regIndex, regIndex-1);
		if(*token == slashsym)
			emit(DIV, regIndex, regIndex, regIndex-1);
	}
}

// TODO
void factor()
{
	int i, j;
	// identifier
	if(*token == identsym)
	{
		getToken();

		/*
		j = type();
		// expression can't contain a procedure ident
		if(j == 3)
			error(BADUSEOFPROCIDENT);
		// undeclared ident
		else if(j != 1 && j != 2)
			error(UNDECLAREDIDENT);
		getToken();

		*/
	}
	else if(*token == numbersym)
	{
		getToken(); // number retrieved
	}
	else if(*token == lparentsym)
	{
		getToken();
		expression();
		// ) missing
		if(*token != rparentsym)
			error(MISSINGRIGHTPAREN);
		if (halt == TRUE) exit(0);
		getToken();
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