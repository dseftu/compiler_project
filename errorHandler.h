// David M Allen
// Error Messages Header
// COP 3402 - Systems Software

#define TRUE 1
#define FALSE 0

const char* BADCONSTASSIGNMENTSYMBOLMSG = "Use = instead of :=.";

const char* CONSTASSIGNMENTMISSINGMSG = "= must be followed by a number.";
const char* MISSINGCONSTASSIGNMENTSYMBOLMSG = "Identifier must be followed by =.";
const char* MISSINGIDENTIFIERMSG = "const, var, procedure must be followed by identifier.";
const char* MISSINGSEMICOLONORCOMMAMSG = "Semicolon or comma missing.";
const char* BADSYMBOLAFTERPROCDECLMSG = "Incorrect symbol after procedure declaration.";
const char* MISSINGSTATEMENTMSG = "Statement expected.";
const char* BADSYMBOLAFTERSTMTINBLOCKMSG = "Incorrect symbol after statement part in block.";
const char* MISSINGPERIODMSG = "Period expected.";
const char* MISSINGSEMICOLONMSG = "Semicolon between statements missing.";
const char* UNDECLAREDIDENTMSG = "Undeclared identifier.";
const char* INVALIDASSIGNMENTMSG = "Assignment to constant or procedure is not allowed.";
const char* MISSINGOPERATORMSG = "Assignment operator expected.";
const char* MISSINGIDENTAFTERCALLMSG = "call must be followed by an identifier.";
const char* INVALIDCALLMSG = "Call of a constant or variable is meaningless.";
const char* MISSINGTHENAFTERIFMSG = "then expected.";
const char* MISSINGSEMICOLONORBRACKETMSG = "Semicolon or } expected.";
const char* MISSINGDOMSG = "do expected.";
const char* BADSYMBOLAFTERSTMTMSG = "Incorrect symbol following statement.";
const char* MISSINGRELATIONALOPERATORMSG = "Relational operator expected.";
const char* BADUSEOFPROCIDENTMSG = "Expression must not contain a procedure identifier.";
const char* MISSINGRIGHTPARENMSG = "Right parenthesis missing.";
const char* INVALIDSTARTTOFACTORMSG = "The preceding factor cannot begin with this symbol.";
const char* INVALIDSTARTTOEXPRESSIONMSG = "An expression cannot begin with this symbol.";
const char* TOOLARGENUMBERMSG = "This number is too large.";

// prints the error for the specific error code.
void error(int errorCode);
