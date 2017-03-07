// David M Allen
// Error Messages Handler
// COP 3402 - Systems Software

#include "errorHandler.h"

void error(int errorCode)
{
    
    halt = TRUE; // might make this optional to raise in a future version
    switch (errorCode)
    {
        case BADCONSTASSIGNMENTSYMBOL:
        {
            printf("%s\n", "test2");
            printf("%s\n", BADCONSTASSIGNMENTSYMBOLMSG);
            return;
        }
        case CONSTASSIGNMENTMISSING:
        {
            printf("%s\n", CONSTASSIGNMENTMISSINGMSG);
            return;
        }
        case MISSINGCONSTASSIGNMENTSYMBOL:
        {
            printf("%s\n", MISSINGCONSTASSIGNMENTSYMBOLMSG);
            return;
        }
        case MISSINGIDENTIFIER:
        {
            printf("%s\n", MISSINGIDENTIFIERMSG);
            return;
        }
        case MISSINGSEMICOLONORCOMMA:
        {
            printf("%s\n", MISSINGSEMICOLONORCOMMAMSG);
            return;
        }
        case BADSYMBOLAFTERPROCDECL:
        {
            printf("%s\n", BADSYMBOLAFTERPROCDECLMSG);
            return;
        }
        case MISSINGSTATEMENT:
        {
            printf("%s\n", MISSINGSTATEMENTMSG);
            return;
        }
        case BADSYMBOLAFTERSTMTINBLOCK:
        {
            printf("%s\n", BADSYMBOLAFTERSTMTINBLOCKMSG);
            return;
        }
        case MISSINGPERIOD:
        {
            printf("%s\n", MISSINGPERIODMSG);
            return;
        }
        case MISSINGSEMICOLON:
        {
            printf("%s\n", MISSINGSEMICOLONMSG);
            return;
        }
        case UNDECLAREDIDENT:
        {
            printf("%s\n", UNDECLAREDIDENTMSG);
            return;
        }
        case INVALIDASSIGNMENT:
        {
            printf("%s\n", INVALIDASSIGNMENTMSG);
            return;
        }
        case MISSINGOPERATOR:
        {
            printf("%s\n", MISSINGOPERATORMSG);
            return;
        }
        case MISSINGIDENTAFTERCALL:
        {
            printf("%s\n", MISSINGIDENTAFTERCALLMSG);
            return;
        }
        case INVALIDCALL:
        {
            printf("%s\n", INVALIDCALLMSG);
            return;
        }
        case MISSINGTHENAFTERIF:
        {
            printf("%s\n", MISSINGTHENAFTERIFMSG);
            return;
        }
        case MISSINGSEMICOLONORBRACKET:
        {
            printf("%s\n", MISSINGSEMICOLONORBRACKETMSG);
            return;
        }
        case MISSINGDO:
        {
            printf("%s\n", MISSINGDOMSG);
            return;
        }
        case BADSYMBOLAFTERSTMT:
        {
            printf("%s\n", BADSYMBOLAFTERSTMTMSG);
            return;
        }
        case MISSINGRELATIONALOPERATOR:
        {
            printf("%s\n", MISSINGRELATIONALOPERATORMSG);
            return;
        }
        case BADUSEOFPROCIDENT:
        {
            printf("%s\n", BADUSEOFPROCIDENTMSG);
            return;
        }
        case MISSINGRIGHTPAREN:
        {
            printf("%s\n", MISSINGRIGHTPARENMSG);
            return;
        }
        case INVALIDSTARTTOFACTOR:
        {
            printf("%s\n", INVALIDSTARTTOFACTORMSG);
            return;
        }
        case INVALIDSTARTTOEXPRESSION:
        {
            printf("%s\n", INVALIDSTARTTOEXPRESSIONMSG);
            return;
        }
        case TOOLARGENUMBER:
        {
            printf("%s\n", TOOLARGENUMBERMSG);
            return;
        }
        default:
        {
            printf("%s\n", "UNKNOWN ERROR CODE");
            return;
        }
        
    }
}
