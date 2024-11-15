/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         S O F I A           @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 19

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERROR_TOKEN,		/*  0: Error token */
	METHOD_NAME_ID,		/*  1: Method name identifier token (start: &) */
	INTEGER_LITERAL,		/*  2: Integer literal token */
	STRING_LITERAL,		/*  3: String literal token */
	LEFT_PARENTHESIS,		/*  4: Left parenthesis token */
	RIGHT_PARENTHESIS,		/*  5: Right parenthesis token */
	LEFT_BRACE,		/*  6: Left brace token */
	RIGHT_BRACE,		/*  7: Right brace token */
	KEYWORD,		/*  8: Keyword token */
	END_OF_STATEMENT,		/*  9: End of statement (semicolon) */
	RUN_TIME_ERROR,		/* 10: Run-time error token */
	SOURCE_END_OF_FILE,		/* 11: Source end-of-file token */
	COMMENT,		/* 12: Comment token */
	// My tokens
	ASSIGNMENT,               /* 13: Assignment operator token (=) */
	COMMA,                    /* 14: Comma token (,) */
	ARITHMETIC_OPERATOR,      /* 15: Arithmetic operator token (+, -, *, /) */
	RELATIONAL_OPERATOR,      /* 16: Relational operator token (<, >, ==, !=) */
	LOGICAL_OPERATOR,          /* 17: Logical operator token (&&, ||, !) */
	VARIABLE_TOKEN			/* 18: Variable Token */
};

/* TO_DO: Define the list of keywords */
static word_s tokenStrTable[NUM_TOKENS] = {
	"ERROR_TOKEN",
	"METHOD_NAME_ID",
	"INTEGER_LITERAL",
	"STRING_LITERAL",
	"LEFT_PARENTHESIS",
	"RIGHT_PARENTHESIS",
	"LEFT_BRACE",
	"RIGHT_BRACE",
	"KEYWORD",
	"END_OF_STATEMENT",
	"RUN_TIME_ERROR",
	"SOURCE_END_OF_FILE",
	"COMMENT",
	// My tokens
	"ASSIGNMENT",
	"COMMA",
	"ARITHMETIC_OPERATOR",
	"RELATIONAL_OPERATOR",
	"LOGICAL_OPERATOR",
	"VARIABLE_TOKEN"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	num_i codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	num_i intValue;				/* integer literal attribute (value) */
	num_i keywordIndex;			/* keyword index in the keyword table */
	num_i contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	num_f floatValue;				/* floating-point literal attribute (value) */
	word_c idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	word_c errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	num_u flags;			/* Flags information */
	union {
		num_i intValue;				/* Integer value */
		num_f floatValue;			/* Float value */
		word_s stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	num_i code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	num_i scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* TO_DO: Define lexeme FIXED classes */
/* EOF definitions */
#define EOS_CHR '\0'	// CH00
#define EOF_CHR 0xFF	// CH01
#define UND_CHR '_'		// CH02
#define STR_CHR '\"'	// CH04
#define TAB_CHR '\t'	// CH06
#define SPC_CHR ' '		// CH07
#define NWL_CHR '\n'	// CH08
#define SCL_CHR ';'		// CH09
#define LPR_CHR '('		// CH10
#define RPR_CHR ')'		// CH11
#define LBR_CHR '{'		// CH12
#define RBR_CHR '}'		// CH13
// My lexemes
#define BSL_CHR '/'		// CH14
#define FUN_CHR 'F'		// CH16
#define COM_CHR ','		// CH17
// Arithematic Operators
#define ADD_CHR '+'		// CH18
#define MIN_CHR '-'		// CH19
#define MUL_CHR '*'		// CH20
#define DIV_CHR '/'		// CH21
// Relational Operators
#define LST_CHR '<'		// CH22
#define GRT_CHR '>'		// CH23
#define ASG_CHR '='		// CH24
#define NOT_CHR '!'		// CH25
// Logical Operator
#define AMP_CHR '&'		// CH26
#define OR_CHR '|'		// CH27

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_BRACE, RIGHT_BRACE, END_OF_STATEMENT, SOURCE_END_OF_FILE and special chars used for tokenis include _, & and ' */


/* TO_DO: Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		10		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		11
#define CHAR_CLASSES	8

/* TO_DO: Transition table - type of states defined in separate table */
static num_i transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],    _,    F,   \", SEOF,    //, other
	   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */
	{     1, ESNR, ESNR, ESNR,    4, ESWR,	  6, ESNR},	// S0: NOAS
	{     1,    1,    1,    2,	  3,    3,   3,    3},	// S1: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S2: ASNR (MVID)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S3: ASWR (KEY)
	{     4,    4,    4,    4,    5, ESWR,	  4,    4},	// S4: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S5: ASNR (SL)
	{     6,    6,    6,    6,    6, ESWR,	  7,    6},	// S6: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S7: ASNR (COM)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS},	// S8: ASNR (ES)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,   FS} // S9: ASWR (ER)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static num_i stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 (MID) - Methods */
	FSWR, /* 03 (KEY) */
	NOFS, /* 04 */
	FSNR, /* 05 (SL) */
	NOFS, /* 06 */
	FSNR, /* 07 (COM) */
	FSNR, /* 08 (Err1 - no retract) */
	FSWR  /* 09 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
num_i			startScanner(BufferPointer psc_buf);
static num_i	nextClass(word_c c);					/* character class function */
static num_i	nextState(num_i, word_c);		/* state machine function */
voids			printScannerData(ScannerData scData);
Token				tokenizer(voids);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(word_s lexeme);

/* Declare accepting states functions */
Token funcSL	(word_s lexeme);
Token funcIL	(word_s lexeme);
Token funcID	(word_s lexeme);
Token funcCMT   (word_s lexeme);
Token funcKEY	(word_s lexeme);
Token funcErr	(word_s lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* COM  [07] */
	funcErr,	/* ERR1 [06] */
	funcErr		/* ERR2 [07] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 16

/* TO_DO: Define the list of keywords */
static word_s keywordTable[KWT_SIZE] = {
	"null", "num_i", "num_l",
	"num_d", "num_f", "num_u", "bol",
	"word_c","word_s", "if", "else",
	"break", "for", "voids", "param", "logic"
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	word_c indentationCharType;
	num_i indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
num_i numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
