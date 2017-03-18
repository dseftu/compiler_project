// David M Allen, Dallas Seroski 
// Parser
// COP 3402 - Systems Software

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char nulsym[3] = "1", identsym[3] = "2", numbersym[3] = "3", plussym[3] = "4",
	minussym[3] = "5", multsym[3] = "6", slashsym[3] = "7", oddsym[3] = "8", eqlsym[3] = "9",
	neqsym[3] = "10", lessym[3] = "11", leqsym[3] = "12", gtrsym[3] = "13", geqsym[3] = "14",
	lparentsym[3] = "15", rparentsym[3] = "16", commasym[3] = "17", semicolonsym[3] = "18",
	periodsym[3] = "19", becomessym[3] = "20", beginsym[3] = "21", endsym[3] = "22",
	ifsym[3] = "23", thensym[3] = "24", whilesym[3] = "25", dosym[3] = "26", callsym[3] = "27",
	constsym[3] = "28", varsym[3] = "29", procsym[3] = "30", writesym[3] = "31",
	readsym[3] = "32", elsesym[3] = "33";

#define MAX_SYMBOL_TABLE_SIZE 100

typedef struct
{
	int kind;		// const = 1, var = 2, proc = 3
	char name[10];	// name up to 11 chars
	int val;		// number (ASCII value)
	int level;		// L level
	int addr;		// M address
} symbol;

void error(int error);
void program();
void block();
void statement();
int type();
void condition();
void expression();
void term();
void factor();
int relop();

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
int table = 0;
int level = -1;
int ident;
int procadd = 0;
char str[12];
FILE* lex;

void main()
{
	//program();
}

void program()
{
	lex = fopen("lexemelist.txt", "r");
	fscanf(lex, "%s", str);
	// prints our lexemelist
	while(!feof(lex))
	{
		printf("%s ", str);
		fscanf(lex, "%s", str);
	}
	printf("\n\n");

	// resets pointer for lex
	rewind(lex);
	block();
	error(0);
	fclose(lex);
}

void block()
{
	int val;
	int var = 0, cons = 0, proc = 0;
	level++;
	procadd++;
	// Declarations
	// Constant Declarations
	fscanf(lex, "%s", str);
	if(strcmp(str, constsym) == 0)
	{
		do
		{
			fscanf(lex, "%s", str);
			// constant must be followed by ident
			if(strcmp(str, identsym) != 0)
				error(4);
			fscanf(lex, "%s", str);
			symbol_table[table].kind = 1;
			strcpy(symbol_table[table].name, str);
			fscanf(lex, "%s", str);
			// checks for :=
			if(strcmp(str, eqlsym) != 0)
			{
				// used = instead of :=
				if(strcmp(str, becomessym) == 0)
					error(1);
				else
					error(3);
			}
			fscanf(lex, "%s", str);
			// = must be followed by number
			if(strcmp(str, numbersym) != 0)
				error(2);
			fscanf(lex, "%s", str);
			symbol_table[table].val = atoi(str);
			table++;
			fscanf(lex, "%s", str);
		}
		while(strcmp(str, commasym) == 0);
		// ; exepected
		if(strcmp(str, semicolonsym) != 0)
			error(17);
		fscanf(lex, "%s", str);
	}

	// Variable Declarations
	if(strcmp(str, varsym) == 0)
	{
		do
		{
			fscanf(lex, "%s", str);
			// Varaible must be followed by an identifier
			if(strcmp(str, identsym) != 0)
				error(4);
			fscanf(lex, "%s", str);
			symbol_table[table].kind = 2;
			strcpy(symbol_table[table].name, str);
			symbol_table[table].level = level;
			symbol_table[table].addr = 4+var;
			table++;
			var++;
			fscanf(lex, "%s", str);
		}
		while(strcmp(str, commasym) == 0);
		// ; expected
		if(strcmp(str, semicolonsym) != 0)
			error(17);
		fscanf(lex, "%s", str);
	}

	// Procedure Declaration
	while(strcmp(str, procsym) == 0)
	{
		fscanf(lex, "%s", str);
		// Procedure must be followed by an identifier
		if(strcmp(str, identsym) != 0)
			error(4);
		fscanf(lex, "%s", str);
		symbol_table[table].kind = 3;
		strcpy(symbol_table[table].name, str);
		symbol_table[table].addr = procadd;
		symbol_table[table].level = level;
		table++;
		proc++;
		fscanf(lex, "%s", str);
		// ; expected
		if(strcmp(str, semicolonsym) != 0)
			error(5);
		fscanf(lex, "%s", str);
		block();
		fscanf(lex, "%s", str);
	}
	statement();
	level--;
}

void statement()
{
	int i, j, add1, add2;

	// identifier
	if(strcmp(str, identsym) == 0)
	{
		fscanf(lex, "%s", str);
		//if constant or procedure
		j = type();
		// can't assign to constant or variable
		if(j == 1 || j == 3)
			error(12);
		// undeclared ident
		else if(j != 2)
			error(11);
		for(i = 0; i < table; i++)
		{
			if(strcmp(str, symbol_table[i].name) == 0)
				ident = i;
		}
		fscanf(lex, "%s", str);
		// need to assign to operator
		if(strcmp(str, becomessym) != 0)
			error(13);
		fscanf(lex, "%s", str);
		expression();
		// missing ;
		if(strcmp(str, semicolonsym) != 0)
			error(10);
	}

	// Call
	else if(strcmp(str, callsym) == 0)
	{
		fscanf(lex, "%s", str);
		// call must be followed by an identifier
		if(strcmp(str, identsym) != 0)
			error(14);
		//checks type
		fscanf(lex, "%s", str);
		j = type();
		// call of a constant or variable meaningless
		if(j == 1 || j == 2)
			error(15);
		// undeclared identifier
		else if(j != 3)
			error(11);
		for(i = 0; i < table; i++)
		{
			if(strcmp(str, symbol_table[i].name) == 0)
				ident = i;
		}
		fscanf(lex, "%s", str);
		// ; missing
		if(strcmp(str, semicolonsym) != 0)
			error(10);
	}

	// Begin
	else if(strcmp(str, beginsym) == 0)
	{
		fscanf(lex, "%s", str);
		statement();
		while(strcmp(str, semicolonsym) == 0)
		{
			fscanf(lex, "%s", str);
			statement();
		}
		// statement expected
		if(strcmp(str, endsym) != 0)
			error(7);
		fscanf(lex, "%s", str);
	}

	// If
	else if(strcmp(str, ifsym) == 0)
	{
		fscanf(lex, "%s", str);
		condition();
		// then expected after condition
		if(strcmp(str, thensym) != 0)
			error(16);
		fscanf(lex, "%s", str);
		statement();
		// ; missing
		if(strcmp(str, semicolonsym) != 0)
			error(10);
		fscanf(lex, "%s", str);
		if(strcmp(str, elsesym) == 0)
		{
			fscanf(lex, "%s", str);
			statement();
			// ; mising
			if(strcmp(str, semicolonsym) != 0)
				error(10);
		}
		else
			fseek(lex, -(strlen(str) + 1), SEEK_CUR);
	}

	// while
	else if(strcmp(str, whilesym) == 0)
	{
		fscanf(lex, "%s", str);
		condition();
		// do exepected
		if(strcmp(str, dosym) != 0)
			error(18);
		fscanf(lex, "%s", str);
		statement();
		// ; missing
		if(strcmp(str, semicolonsym) != 0)
			error(10);
	}

	// read
	else if(strcmp(str, readsym) == 0)
	{
		fscanf(lex, "%s", str);
		// needs to be identifier
		if(strcmp(str, identsym) != 0)
			error(25);
		fscanf(lex, "%s", str);
		j = type();
		// can't assign to a const or proc
		if(j == 1 || j == 3)
			error(12);
		// undeclared ident
		else if(j != 2)
			error(11);
		for(i = 0; i < table; i++)
		{
			if(strcmp(str, symbol_table[i].name) == 0)
				ident = i;
		}

		fscanf(lex, "%s", str);
		// ; missing
		if(strcmp(str, semicolonsym) != 0)
			error(10);
	}

	// Write
	else if(strcmp(str, writesym) == 0)
	{
		fscanf(lex, "%s", str);
		if(strcmp(str, identsym) != 0)
			error(25);
		fscanf(lex, "%s", str);
		j = type();
		// undeclared identifier
		if(j > 3 || j < 1)
			error(11);
		fscanf(lex, "%s", str);
		// ; missing
		if(strcmp(str, semicolonsym) != 0)
			error(10);
	}
}

void condition()
{
	int op;
	if(strcmp(str, oddsym) == 0)
	{
		fscanf(lex, "%s", str);
		expression();
	}
	else
	{
		expression();
		// rel-op expected
		if(!relop())
			error(20);
		fscanf(lex, "%s", str);
		expression();
	}
}

void expression()
{
	if(strcmp(str, plussym) == 0 || strcmp(str, minussym) == 0)
		fscanf(lex, "%s", str);
	term();
	while(strcmp(str, plussym) == 0 || strcmp(str, minussym) == 0)
	{
		fscanf(lex, "%s", str);
		term();
	}
}


void term()
{
	factor();
	while(strcmp(str, multsym) == 0 || strcmp(str, slashsym) == 0)
	{
		fscanf(lex, "%s", str);
		factor();
	}
}

void factor()
{
	int i, j;
	// identifier
	if(strcmp(str, identsym) == 0)
	{
		fscanf(lex, "%s", str);
		j = type();
		// expression can't contain a procedure ident
		if(j == 3)
			error(21);
		// undeclared ident
		else if(j != 1 && j != 2)
			error(11);
		fscanf(lex, "%s", str);
	}
	else if(strcmp(str, numbersym) == 0)
	{
		fscanf(lex, "%s", str);
	}
	else if(strcmp(str, lparentsym) == 0)
	{
		fscanf(lex, "%s", str);
		expression();
		// ) missing
		if(strcmp(str, rparentsym) != 0)
			error(22);
		fscanf(lex, "%s", str);
	}
	//can't begin with this symbol
	else
		error(23);
}

int relop()
{
	// if its relational return 1, 0 if not
	if(strcmp(str, eqlsym) == 0 || strcmp(str, neqsym) == 0 ||
	   strcmp(str, lessym) == 0 || strcmp(str, leqsym) == 0 ||
	   strcmp(str, gtrsym) == 0 || strcmp(str, geqsym) == 0)
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
		if (strcmp(str, symbol_table[i].name) == 0 && symbol_table[i].kind == 1)
			return 1;
		if (strcmp(str, symbol_table[i].name) == 0 && symbol_table[i].kind == 2)
			return 2;
		if (strcmp(str, symbol_table[i].name) == 0 && symbol_table[i].kind == 3)
			return 3;
	}
	return 0;
}

void error(int error)
{
	switch(error)
	{
		case 0:
			printf(" No errors, program is syntactically correct\n");
			exit(0);
			break;
		case 1:
			printf("Error number 1, Use = instead of :=.\n");
			exit(0);
			break;
		case 2:
			printf("Error number 2, = must be followed by a number.\n");
			exit(0);
			break;
		case 3:
			printf("Error number 3, indentifier must be followed by =.\n");
			exit(0);
			break;
		case 4:
			printf("Error number 4, cons, var, procedure must be followed by identifier.\n");
			exit(0);
			break;
		case 5:
			printf("Error number 5, Semicolon or comma missing.\n");
			exit(0);
			break;
		case 6:
			printf("Error number 6. Incorrect symbol after procedure declaration.\n");
			exit(0);
			break;
		case 7:
			printf("Error number 7, Statement expected.\n");
			exit(0);
			break;
		case 8:
			printf("Error number 8, Incorrect symbol after statement part in block.\n");
			exit(0);
			break;
		case 9:
			printf("Error number 9, Period exepected.\n");
			exit(0);
			break;
		case 10:
			printf("Error number 10, Semicolon between statements missing.\n");
			exit(0);
			break;
		case 11:
			printf("Error number 11, Undeclared identifier.\n");
			break;
		case 12:
			printf("Error number 12, Assignment to constant or procedure is not allowed.\n");
			exit(0);
			break;
		case 13:
			printf("Error number 13, Assignment operator expected.\n");
			exit(0);
			break;
		case 14:
			printf("Error number 14, call must be followed by an identifier.\n");
			exit(0);
			break;
		case 15:
			printf("Error number 15, Call of a constant or variable is meaningless.\n");
			exit(0);
			break;
		case 16:
			printf("Error number 16, then expected.\n");
			exit(0);
			break;
		case 17:
			printf("Error number 17, Semicolon or } expected.\n");
			exit(0);
			break;
		case 18:
			printf("Error number 18, do expected.\n");
			exit(0);
			break;
		case 19:
			printf("Error number 19, Incorrect symbol following statement.\n");
			exit(0);
			break;
		case 20:
			printf("Error number 20, Relational operator expected.\n");
			exit(0);
			break;
		case 21:
			printf("Error number 21, Expression must not contain a procedure identifier.\n");
			exit(0);
			break;
		case 22: 
			printf("Error number 22, Right parentheis missing.\n");
			exit(0);
			break;
		case 23:
			printf("Error number 23, The preceding factor cannot begin with this symbol.\n");
			exit(0);
			break;
		case 24:
			printf("Error number 24, An expression canot begin with this symbol.\n");
			exit(0);
			break;
		case 25:
			printf("Error number 25, This number is too large.\n");
			exit(0);
			break;
		default:
			exit(0);
			break;
	}
}
