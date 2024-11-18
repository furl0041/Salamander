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
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "    @@                             @@    �
# ECHO "    @@           %&@@@@@@@@@@@     @@    �
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    �
# ECHO "    @@      @& @   @ @       @     @@    �
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    �
# ECHO "    @@      & @ @  @@              @@    �
# ECHO "    @@       @/ @*@ @ @   @        @@    �
# ECHO "    @@           @@@@  @@ @ @      @@    �
# ECHO "    @@            /@@    @@@ @     @@    �
# ECHO "    @@     @      / /     @@ @     @@    �
# ECHO "    @@     @ @@   /@/   @@@ @      @@    �
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@         S O F I A           @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

  /* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
num_i line;								/* Current line number of the source code */
extern num_i errorNumber;				/* Defined in platy_st.c - run-time error number */

extern num_i stateType[NUM_STATES];
extern word_s keywordTable[KWT_SIZE];

extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern num_i transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

num_i startScanner(BufferPointer psc_buf) {
	/* TO_DO: Start histogram */
	for (num_i i = 0; i < NUM_TOKENS; i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(voids) {

	/* TO_DO: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	word_c c;			/* input symbol */
	num_i state = 0;	/* initial state of the FSM */
	num_i lexStart;	/* start offset of a lexeme in the input char buffer (array) */
	num_i lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	num_i lexLength;	/* token length */
	num_i i;			/* counter */
	///sofia_char newc;		// new char

	/* Starting lexeme */
	word_s lexeme;	/* lexeme (to check the function) */
	lexeme = (word_s)malloc(VID_LEN * sizeof(word_c));
	if (!lexeme)
		return currentToken;
	lexeme[0] = EOS_CHR;

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

		// TO_DO: Defensive programming
		if (c < 0 || c >= NCHAR)
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
		switch (c) {

			/* Cases for spaces */
		case SPC_CHR:
		case TAB_CHR:
			break;
		case NWL_CHR:
			line++;
			break;

			/* Cases for symbols */
		case SCL_CHR:
			currentToken.code = END_OF_STATEMENT;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LPR_CHR:
			currentToken.code = LEFT_PARENTHESIS;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RPR_CHR:
			currentToken.code = RIGHT_PARENTHESIS;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LBR_CHR:
			currentToken.code = LEFT_BRACE;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RBR_CHR:
			currentToken.code = RIGHT_BRACE;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
			/* Cases for END OF FILE */
		case EOS_CHR:
			currentToken.code = SOURCE_END_OF_FILE;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case EOF_CHR:
			currentToken.code = SOURCE_END_OF_FILE;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		// MY CASES
		case COM_CHR:
			currentToken.code = COMMA;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;

			/* Handle numeric literals */
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			i = 0;
			lexeme[i++] = c;

			c = readerGetChar(sourceBuffer);
			while (isdigit(c)) {
				if (i < VID_LEN) {
					lexeme[i++] = c;
				}
				c = readerGetChar(sourceBuffer);
			}

			// Handle floating-point part
			if (c == '.') {
				lexeme[i++] = c;
				c = readerGetChar(sourceBuffer);
				while (isdigit(c)) {
					if (i < VID_LEN) {
						lexeme[i++] = c;
					}
					c = readerGetChar(sourceBuffer);
				}
				lexeme[i] = EOS_CHR;
				readerRetract(sourceBuffer);
				currentToken.code = FLOAT_LITERAL;
				currentToken.attribute.floatValue = atof(lexeme);
			}
			else {
				lexeme[i] = EOS_CHR;
				readerRetract(sourceBuffer);
				currentToken.code = INTEGER_LITERAL;
				currentToken.attribute.intValue = atoi(lexeme);
			}

			scData.scanHistogram[currentToken.code]++;
			free(lexeme);
			return currentToken;

			/* Handle identifiers (keywords, method names, etc.) */
		case VAR_CHR: // Variable identifier starting with `$`
			i = 0;
			lexeme[i++] = c;
			c = readerGetChar(sourceBuffer);

			while (isalpha(c) || isdigit(c) || c == UND_CHR) {
				if (i < VID_LEN) {
					lexeme[i++] = c;
				}
				c = readerGetChar(sourceBuffer);
			}

			lexeme[i] = EOS_CHR;
			readerRetract(sourceBuffer);

			currentToken.code = VARIABLE_TOKEN;
			strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
			currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;

			scData.scanHistogram[currentToken.code]++;
			free(lexeme);
			return currentToken;

		case ASG_CHR: // '=' Assignment operator
			currentToken.code = ASSIGNMENT;
			scData.scanHistogram[currentToken.code]++;
			free(lexeme);
			return currentToken;

		/* Arithmetic Operators */
		case ADD_CHR:
			currentToken.code = ARITHMETIC_OPERATOR;
			currentToken.attribute.arithmeticOperator = OP_ADD;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case MIN_CHR:
			currentToken.code = ARITHMETIC_OPERATOR;
			currentToken.attribute.arithmeticOperator = OP_SUB;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case MUL_CHR:
			currentToken.code = ARITHMETIC_OPERATOR;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case DIV_CHR:
			c = readerGetChar(sourceBuffer);
			if (c == BSL_CHR) {
				while (c != NWL_CHR && c != EOF_CHR) {
					c = readerGetChar(sourceBuffer);
				}
				currentToken.code = COMMENT;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken.code = ARITHMETIC_OPERATOR;
				currentToken.attribute.arithmeticOperator = OP_DIV;
				scData.scanHistogram[currentToken.code]++;
				free(lexeme);
				return currentToken;
			}

			/* ------------------------------------------------------------------------
				Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
				Note: Part 2 must follow Part 1 to catch the illegal symbols
				-----------------------------------------------------------------------
			*/

			/* TO_DO: Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOFS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == FSWR)
				readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((num_i)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);
			// TO_DO: Defensive programming
			if (!lexeme)
				return currentToken;
			currentToken = (*finalStateTable[state])(lexeme);
			readerRestore(lexemeBuffer);
			return currentToken;

		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

num_i nextState(num_i state, word_c c) {
	num_i col;
	num_i next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
 /* TO_DO: Use your column configuration */

 /* Adjust the logic to return next column in TT */
 /*    [A-z],[0-9],    _,    &,   \', SEOF,    #, other
		L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */

num_i nextClass(word_c c) {
	num_i val = -1;
	switch (c) {
	case UND_CHR:
		val = 2;
		break;
	case FUN_CHR:
		val = 3;
		break;
	case STR_CHR:
		val = 4;
		break;
	case BSL_CHR:
		val = 6;
		break;
	case EOS_CHR:
	case EOF_CHR:
		val = 5;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 7;
	}
	return val;
}

/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */
 /* TO_DO: Adjust the function for IL */

Token funcCMT(word_s lexeme) {
	Token currentToken = { 0 };
	num_i i = 0, len = (num_i)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
	}
	currentToken.code = COMMENT;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function IL
 *		Function responsible to identify IL (integer literals).
 * - It is necessary respect the limit (ex: 2-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually,
 *   additional three dots (...) should be put in the output.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for IL */

Token funcIL(word_s lexeme) {
	Token currentToken = { 0 };
	num_l tlong;
	if (lexeme[0] != EOS_CHR && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INTEGER_LITERAL;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (num_i)tlong;
		}
		else {
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */

Token funcID(word_s lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	word_c lastch = lexeme[length - 1];
	num_i isID = FALSE;
	switch (lastch) {
	case FUN_CHR:
		currentToken.code = METHOD_NAME_ID;
		scData.scanHistogram[currentToken.code]++;
		isID = TRUE;
		break;
	default:
		// Test Keyword
		///lexeme[length - 1] = EOS_CHR;
		currentToken = funcKEY(lexeme);
		break;
	}
	if (isID == TRUE) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for SL */

Token funcSL(word_s lexeme) {
	Token currentToken = { 0 };
	num_i i = 0, len = (num_i)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RUN_TIME_ERROR;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, EOS_CHR)) {
		currentToken.code = RUN_TIME_ERROR;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STRING_LITERAL;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(word_s lexeme) {
	Token currentToken = { 0 };
	num_i kwindex = -1, j = 0;
	num_i len = (num_i)strlen(lexeme);
	///lexeme[len - 1] = EOS_CHR;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KEYWORD;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(word_s lexeme) {
	Token currentToken = { 0 };
	num_i i = 0, len = (num_i)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == NWL_CHR)
			line++;
	currentToken.code = ERROR_TOKEN;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

voids printToken(Token t) {
	extern word_s keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RUN_TIME_ERROR:
		printf("RUN_TIME_ERROR\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERROR_TOKEN:
		printf("ERROR_TOKEN\t\t%s\n", t.attribute.errLexeme);
		break;
	case SOURCE_END_OF_FILE:
		printf("SOURCE_END_OF_FILE\t\t%d\t\n", t.attribute.seofType);
		break;
	case METHOD_NAME_ID:
		printf("METHOD_NAME_ID\t\t%s\n", t.attribute.idLexeme);
		break;
	case STRING_LITERAL:
		printf("STRING_LITERAL\t\t%d\t ", (num_i)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (num_i)t.attribute.codeType));
		break;
	case INTEGER_LITERAL:  // Add case for INTEGER_LITERAL here
		printf("INTEGER_LITERAL\t\t%d\n", t.attribute.intValue);
		break;
	case FLOAT_LITERAL:  // Add case for INTEGER_LITERAL here
		printf("FLOAT_LITERAL\t\t%f\n", t.attribute.floatValue);
		break;
	case LEFT_PARENTHESIS:
		printf("LEFT_PARENTHESIS\n");
		break;
	case RIGHT_PARENTHESIS:
		printf("RIGHT_PARENTHESIS\n");
		break;
	case LEFT_BRACE:
		printf("LEFT_BRACE\n");
		break;
	case RIGHT_BRACE:
		printf("RIGHT_BRACE\n");
		break;
	case KEYWORD:
		printf("KEYWORD\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case COMMENT:
		printf("COMMENT\n");
		break;
	case END_OF_STATEMENT:
		printf("END_OF_STATEMENT\n");
		break;
	case ASSIGNMENT:
		printf("ASSIGMENT\n");
		break;
	case VARIABLE_TOKEN:
		printf("VARIABLE_TOKEN\t\t%s\n", t.attribute.idLexeme);
		break;
	case ARITHMETIC_OPERATOR:
		printf("ARITHMETIC_OPERATOR\t\t%d\n", t.attribute.arithmeticOperator);
		break;
	case RELATIONAL_OPERATOR:
		printf("RELATIONAL_OPERATOR\t\t%d\n", t.attribute.relationalOperator);
		break;
	case LOGICAL_OPERATOR:
		printf("LOGICAL_OPERATOR\t\t%d\n", t.attribute.logicalOperator);
		break;
	case COMMA:
		printf("COMMA\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
voids printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/