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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm',  /* Multiplicative mode (constant increment to be multiplied) */
	MODE_TOTAL = 't', /* Total mode (max size allocation) */
};

/* Constants about controls (not need to change) */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX		-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */ // 250 orginal
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

#define NCHAR						128			/* Chars from 0 to 127 */

#define EOF					(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* Flag masks */
#define READER_SET_FLAG_EMP 0X01 /* (0b00000001) = (0x01)_16 = (001)_10 */
#define READER_SET_FLAG_FUL 0X02 /* (0b00000010) = (0x02)_16 = (002)_10 */
#define READER_SET_FLAG_END 0X04 /* (0b00000100) = (0x04)_16 = (004)_10 */
#define READER_SET_FLAG_REL 0X08 /* (0b00001000) = (0x08)_16 = (008)_10 */

/* Offset declaration */
typedef struct flag {
	bol isEmpty;			/* indicates if the buffer is empty */
	bol isFull;			/* indicates if the buffer is full */
	bol isRead;			/* indicates if the buffer was completely read */
	bol isMoved;			/* indicates if the buffer memory was changed */
} Flag;

/* Offset declaration */
typedef struct position {
	num_i wrte;			/* the offset to the add chars (in chars) */
	num_i read;			/* the offset to the get a char position (in chars) */
	num_i mark;			/* the offset to the mark position (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	word_s	content;			/* pointer to the beginning of character array (character buffer) */
	num_i		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	num_i		increment;			/* character array increment factor */
	word_c		mode;				/* operational mode indicator */
	bol			flagbytes[4];
	//Flag			flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		positions;			/* Offset / position field */
	num_i		histogram[NCHAR];	/* Statistics of chars */
	num_i		numReaderErrors;	/* Number of errors from Reader */
	num_u		checksum;
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate		(num_i, num_i, word_c);
BufferPointer	readerAddChar		(BufferPointer const, word_c);
bol		readerClear		    (BufferPointer const);
bol		readerFree		    (BufferPointer const);
bol		readerIsFull		(BufferPointer const);
bol		readerIsEmpty		(BufferPointer const);
bol		readerSetMark		(BufferPointer const, num_i);
num_i		readerPrint		    (BufferPointer const);
num_i		readerLoad			(BufferPointer const, FILE* const);
bol		readerRecover		(BufferPointer const);
bol		readerRetract		(BufferPointer const);
bol		readerRestore		(BufferPointer const);
voids		readerCalcChecksum	(BufferPointer const);
bol		readerPrintFlags	(BufferPointer const);
voids		readerPrintStat     (BufferPointer const);
/* Getters */
word_c		readerGetChar		(BufferPointer const);
word_s	readerGetContent	(BufferPointer const, num_i);
num_i		readerGetPosRead	(BufferPointer const);
num_i		readerGetPosWrte	(BufferPointer const);
num_i		readerGetPosMark	(BufferPointer const);
num_i		readerGetSize		(BufferPointer const);
num_i		readerGetInc		(BufferPointer const);
word_c		readerGetMode		(BufferPointer const);
num_i		readerGetNumErrors	(BufferPointer const);

#endif
