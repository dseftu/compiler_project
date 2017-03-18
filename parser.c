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
int table = 0;
int level = -1;
int ident;
int procadd = 0;
extern int halt;

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
}

// TODO
// enter a new symbol in the symbol table
//void enter(...)
//{}

// Updated
void program()
{
	getToken();
	block();
	if (*token != periodsym)
		error(MISSINGPERIOD);
	if (halt == TRUE) exit(0);
	printf("\nNo errors, program is syntactically correct.\n");
}

// Updated
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

			// constant must be followed by ident
			if(*token != identsym)
				error(MISSINGIDENTIFIER);
			if (halt == TRUE) exit(0);	
			getToken();

			// const ident must be followed by =
			if(*token != eqlsym)
				error(MISSINGCONSTASSIGNMENTSYMBOL);			
			if (halt == TRUE) exit(0);
			getToken();

			// const must be numeric
			if(*token != numbersym)
				error(CONSTASSIGNMENTMISSING);
			if (halt == TRUE) exit(0);
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
			getToken();

			// Varaible must be followed by an identifier
			if(*token != identsym)
				error(MISSINGIDENTIFIER);
			getToken();

			// not sure if this should be on table or table - 1
			//lexemeList[table].level = level;
			//lexemeList[table].adr = 4+var;
			//var++;			
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
		getToken();

		// Procedure must be followed by an identifier
		if(*token != identsym)
			error(MISSINGIDENTIFIER);
		if (halt == TRUE) exit(0);
		getToken();
		
		// not sure if this should be on table or table-1
		// this should take place in enter()
		//lexemeList[table].adr = procadd;
		//lexemeList[table].level = level;
		//proc++;

		getToken();
		// ; expected
		if(*token != semicolonsym)
			error(MISSINGSEMICOLONORCOMMA);
		if (halt == TRUE) exit(0);
		getToken();
		block();
		getToken();
	}

	statement();
	//level--;
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

		getToken();
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
		getToken();
		condition();
		// do exepected
		if(*token != dosym)
			error(MISSINGDO);
		if (halt == TRUE) exit(0);
		getToken();
		statement();

		/* not sure if we need this
		// ; missing
		if(strcmp(str, semicolonsym) != 0)
			error(MISSINGSEMICOLON);
		*/
	}

/* TODO implement read/write
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
			if(strcmp(str, lexemeList[i].name) == 0)
				ident = i;
		}

		getToken();
		// ; missing
		if(strcmp(str, semicolonsym) != 0)
			error(MISSINGSEMICOLON);
	}

	// Write
	else if(*token == writesym)
	{
		getToken();
		if(strcmp(str, identsym) != 0)
			error(TOOLARGENUMBER);
		getToken();
		j = type();
		// undeclared identifier
		if(j > 3 || j < 1)
			error(UNDECLAREDIDENT);
		getToken();
		// ; missing
		if(strcmp(str, semicolonsym) != 0)
			error(MISSINGSEMICOLON);
	}

*/
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
		getToken();
		term();
	while ( (*token == plussym) || (*token == minussym) )
	{
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
		getToken();
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
/*
// Might be removing these
int relop()
{
	// if its relational return 1, 0 if not
	if(*token >= eqlsym && *token <= geqsym)
		return 1;
	else
		return 0;
}
// Check if token is a constant, variable, or procedure
// Returns 1 if Const, 2 if var, 3 is proc, 0 if non
int type()
{
	int i;
	for (i = 0; i < table; i++)
	{
		if (strcmp(str, lexemeList[i].name) == 0 && lexemeList[i].kind == 1)
			return 1;
		if (strcmp(str, lexemeList[i].name) == 0 && lexemeList[i].kind == 2)
			return 2;
		if (strcmp(str, lexemeList[i].name) == 0 && lexemeList[i].kind == 3)
			return 3;
	}
	return 0;
}
*/