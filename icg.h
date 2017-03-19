// David M Allen
// Intermediate Code Generator header
// COP 3402 - Systems Software

// output filename
char* objectCodeFileName;

// puts a new line into the object code file
void genCode(int op, int r, int l, int m);

// creats a new file if one doesn't already exist
void initObjectCodeFile();

