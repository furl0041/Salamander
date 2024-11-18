/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: Elijah Furlonge
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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Svillen Ranev / Paulo Sousa
 * History/Versions: S22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to reader)
 * Algorithm: Allocation of memory according to inicial (default) values.
 * TODO ......................................................
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */


BufferPointer readerCreate(num_i size, num_i increment, word_c mode) {
	//printf("* readerCreate\n"); /* Debug message */
	BufferPointer readerPointer;
	num_i count = 0;
	/* Defensive programming */
	if (size <= 0 || size > READER_MAX_SIZE) {
		size = READER_DEFAULT_SIZE;
	}
	if (increment <= 0) {
		increment = READER_DEFAULT_INCREMENT;
	}
	if (mode != 'f' && mode != 'a' && mode != 'm' && mode != 't') {
		mode = MODE_FIXED; // Defaults to Fixed mode
	}
	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer) {
		printf("Failed to allocate memory for Buffer");
		return INVALID;
	}
	readerPointer->content = (word_s)malloc(size * sizeof(word_c));
	/* Defensive programming */
	if (!readerPointer->content) {
		free(readerPointer);
		printf("Failed to allocate memory for buffer content");
		return INVALID;
	}
	/* Initialize the histogram */
	for (int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;
	}
	/* Initialize errors */
	readerPointer->numReaderErrors = 0;
	readerPointer->mode = mode;
	readerPointer->size = size;
	readerPointer->increment = increment;

	/* Initialize flags */
	//readerPointer->flags.isEmpty = TRUE;
	//readerPointer->flags.isFull = FALSE;
	//readerPointer->flags.isRead = FALSE;
	//readerPointer->flags.isMoved = FALSE;

	readerPointer->flagbytes[0] |= READER_SET_FLAG_EMP;
	readerPointer->flagbytes[1] |= READER_SET_FLAG_FUL;
	readerPointer->flagbytes[2] |= READER_SET_FLAG_END;
	readerPointer->flagbytes[3] |= READER_SET_FLAG_REL;
	
	/* Default checksum */
	readerPointer->checksum = 0;

	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/


BufferPointer readerAddChar(BufferPointer readerPointer, word_c ch) {
	word_s tempReader = INVALID;
	num_i newSize = 0;
	word_c tempChar = ' ';

	// Check if valid pointer
	if (!readerPointer) {
		printf("Error: Invalid BufferPointer (RAC)\n"); /* Debug message */
		return INVALID;
	}

	tempReader = INVALID; // set to INVALID for every call of this function

	// Check if buffer still has space for more characters
	if (readerPointer->positions.wrte * (num_i)sizeof(word_c) < readerPointer->size) {
		//printf("buffer not full\n"); /* Debug message */
		// Set flags
		//readerPointer->flags.isEmpty = FALSE;
		//readerPointer->flags.isFull = FALSE;
		readerPointer->flagbytes[0] &= ~READER_SET_FLAG_EMP; // Clear empty flag
		readerPointer->flagbytes[1] &= ~READER_SET_FLAG_FUL; // Clear full flag
	}
	else {
		/* Reset Full flag */
		//printf("reset full flag\n"); /* Debug message */
		//readerPointer->flags.isFull = TRUE;
		//readerPointer->flagbytes
		readerPointer->flagbytes[1] |= ~READER_SET_FLAG_FUL; // Set full flag

		switch (readerPointer->mode) {
		case MODE_FIXED:
			//printf("Fixed\n"); /* Debug message */
			/* Update the last position with Terminator */
			if (readerPointer->positions.wrte >= readerPointer->size) {
				readerPointer->content[readerPointer->positions.wrte - 1] = READER_TERMINATOR;
				return readerPointer;
			}
			break;
		case MODE_ADDIT:
			//printf("Addit\n"); /* Debug message */
			/* Update size for Additive mode */
			newSize = readerPointer->size + readerPointer->increment;
			/* Defensive programming */
			break;
		case MODE_MULTI:
			//printf("Multi\n"); /* Debug message */
			/* Update size for Additive mode */
			if (readerPointer->increment <= 0) {
				printf("Error: Increment mus be greater than zero.\n");
				return INVALID;
			}
			newSize = readerPointer->size * readerPointer->increment;
			/* Defensive programming */
			if (readerPointer->increment <= 0) {
				printf("Error: Increment must be greater than zero.\n");
				return INVALID;
			}
			if (readerPointer->size > INT_MAX / readerPointer->increment) {
				printf("Error: Size overflow detected.\n");
				return INVALID;
			}
			break;
		case MODE_TOTAL:
			if (readerPointer->size < READER_MAX_SIZE) {
				newSize = READER_MAX_SIZE;
			}
			else {
				printf("Buffer already at max size in total mode.\n");
				return INVALID;
			}
			break;
		default:
			return INVALID;
		}
		/* Reallocate */
		tempReader = realloc(readerPointer->content, newSize * sizeof(word_c));
		if (tempReader != NULL) {
			readerPointer->content = tempReader; // Update content pointer
			readerPointer->size = newSize; // Update the buffer size
		}
		else if (tempReader == INVALID) {
			printf("Error: Memory reallocation failed\n");
			return INVALID; // Handle allocation failure
		}
	}

	if (readerPointer->positions.wrte < readerPointer->size) {
		readerPointer->content[readerPointer->positions.wrte++] = ch; // Add character to the buffer
	}
	else {
		printf("Buffer overflow attempt.\n");
		return INVALID;
	}

	/* Update the flags */
	//readerPointer->flags.isEmpty = FALSE; // Buffer is not empty after adding
	//readerPointer->flags.isFull = (readerPointer->positions.wrte == readerPointer->size); // Check if buffer is full
	readerPointer->flagbytes[0] &= ~READER_SET_FLAG_EMP; // Clear empty flag
	if (readerPointer->positions.wrte == readerPointer->size) {
		readerPointer->flagbytes[1] |= READER_SET_FLAG_FUL; // Set full flag
	}
	
	/* Updates histogram */
	if (ch >= 0 && ch < NCHAR) {
		readerPointer->histogram[(num_u)ch]++; // Increment the count for the character
	}

	return readerPointer;
}


/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
bol readerClear(BufferPointer const readerPointer) {
	//printf("* readerClear\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 3");
		return FALSE;
	}
	/* Adjust the write, mark and read to zero */
	readerPointer->positions.wrte = 0;
	readerPointer->positions.mark = 0;
	readerPointer->positions.read = 0;
	/* Adjust flags */
	//readerPointer->flags.isEmpty = TRUE;
	//readerPointer->flags.isFull = FALSE;
	//readerPointer->flags.isRead = FALSE;
	//readerPointer->flags.isMoved = FALSE;

	readerPointer->flagbytes[0] |= READER_SET_FLAG_EMP; // Set empty flag
	readerPointer->flagbytes[1] &= ~READER_SET_FLAG_FUL; // Clear full flag
	readerPointer->flagbytes[2] &= ~READER_SET_FLAG_END;
	readerPointer->flagbytes[3] &= ~READER_SET_FLAG_REL;
	return TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
bol readerFree(BufferPointer const readerPointer) {
	//printf("* readerFree\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 4");
		return FALSE;
	}
	/* Free pointers */
	if (readerPointer->content) {
		free(readerPointer->content); // Free character buffer (contents)
	}
	free(readerPointer); // Free pointer itself
	return TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
bol readerIsFull(BufferPointer const readerPointer) {
	//printf("* readerIsFull\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 5");
		return FALSE;
	}
	/* Check flag if buffer is FUL */
	if (readerPointer->flagbytes[1] & READER_SET_FLAG_FUL) {
		return TRUE;
	}
	return FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
bol readerIsEmpty(BufferPointer const readerPointer) {
	//printf("* readerIsEmpty\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 6");
		return FALSE;
	}
	/* Check flag if buffer is EMP */
	if (readerPointer->flagbytes[0] & READER_SET_FLAG_EMP) {
		return TRUE;
	}
	return FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
bol readerSetMark(BufferPointer const readerPointer, num_i mark) {
	printf("* readerSetMark\n");
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 7");
		return FALSE;
	}
	if (mark < 0 || mark >= readerPointer->size) {
		return FALSE;
	}
	/* Adjust mark */
	readerPointer->positions.mark = mark;
	return TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
num_i readerPrint(BufferPointer const readerPointer) {
	num_i cont = 0;
	word_c c;
	/* Defensive programming (including invalid chars) */
	if (!readerPointer) {
		printf("Invalid Pointer 8");
		return FALSE; // return if pointer is null
	}
	//printf("Getting character at position %d\n", readerPointer->positions.read); /* Check character position */
	c = readerGetChar(readerPointer);
	while (cont < readerPointer->positions.wrte) {
		cont++;
		printf("%c", c); 
		c = readerGetChar(readerPointer);
	}
	//printf("\nGetting character at position %d\n", readerPointer->positions.read); /* Check character position */
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
num_i readerLoad(BufferPointer readerPointer, FILE* const fileDescriptor) {
	num_i size = 0;
	word_c c;
	/* Defensive programming */
	if (fileDescriptor == NULL) {
		printf("Error: Invalid file descriptor (NULL)\n");
		return ERROR; // Indicate an error
	}

	if (readerPointer == NULL) {
		printf("Error: Invalid BufferPointer (NULL)\n");
		return ERROR; // Indicate an error
	}
	while (!feof(fileDescriptor)) {
		c = (word_c)fgetc(fileDescriptor);
		readerPointer = readerAddChar(readerPointer, c);
		size++;
	}
	/* Defensive programming */
	if (size < 0) {
		printf("Warning: Size is negative, something went wrong\n");
	}
	return size;
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
bol readerRecover(BufferPointer const readerPointer) {
	//printf("* readerRecover\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) { // Checks if pointer is valid, returns FALSE
		printf("Invalid Pointer 9");
		return FALSE;
	}

	/* Recover positions: read and mark must be zero */
	readerPointer->positions.read = 0;
	readerPointer->positions.mark = 0;
	/* Update flags */
	//readerPointer->flags.isRead = FALSE;
	//readerPointer->flags.isMoved = FALSE;
	//readerPointer->flags.isEmpty = FALSE;
	readerPointer->flagbytes[0] &= ~READER_SET_FLAG_EMP;
	readerPointer->flagbytes[2] &= ~READER_SET_FLAG_END;
	readerPointer->flagbytes[3] &= ~READER_SET_FLAG_REL;

	return TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
bol readerRetract(BufferPointer const readerPointer) {
	//printf("* readerRetract\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) { // Checks if pointer is valid, returns FALSE
		printf("Invalid Pointer 10");
		return FALSE;
	}
	/* Retract (return 1 pos read) */
	if (readerPointer->positions.read > 0) {
		readerPointer->positions.read--;
		return TRUE;
	}
	return TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
bol readerRestore(BufferPointer const readerPointer) {
	//printf("* readerRestore\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) { // Checks if pointer is valid, returns FALSE
		printf("Invalid Pointer 11");
		return FALSE;
	}
	if (readerPointer->positions.mark < 0 || readerPointer->positions.mark >= readerPointer->positions.wrte) {
		return FALSE;
	}
	/* Restore positions (read to mark) */
	readerPointer->positions.read = readerPointer->positions.mark;
	return TRUE;
}


/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
word_c readerGetChar(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		printf("Error: Invalid BufferPointer (NULL)\n");
		return EOF; // Return EOF or a sentinel value for error
	}
	/* Check condition to read/wrte */
	if (readerPointer->positions.read >= readerPointer->positions.wrte) {
		// No more characters to read
		return EOF; // or any value indicating the end of the buffer
	}
	return readerPointer->content[readerPointer->positions.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
word_s readerGetContent(BufferPointer const readerPointer, num_i pos) {
	//printf("* readerGetContent\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 12");
		return NULL; // return if pointer is null
	}
	if (pos < 0 || pos >= readerPointer->positions.wrte) {
		return NULL; // if out of bounds
	}
	return readerPointer->content + pos;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
num_i readerGetPosRead(BufferPointer const readerPointer) {
	//printf("* readerGetPosRead\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 13");
		return ERROR;
	}
	/* Return read */
	return readerPointer->positions.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
num_i readerGetPosWrte(BufferPointer const readerPointer) {
	//printf("* readerGetPosWrte\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 14");
		return ERROR;
	}
	/* Return wrte */
	return readerPointer->positions.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
num_i readerGetPosMark(BufferPointer const readerPointer) {
	//printf("* readerGetPosMark\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 15");
		return ERROR;
	}
	/* Return mark */
	return readerPointer->positions.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
num_i readerGetSize(BufferPointer const readerPointer) {
	//printf("* readerGetSize\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 16");
		return ERROR;
	}
	/* Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
num_i readerGetInc(BufferPointer const readerPointer) {
	//printf("* readerGetInc\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 17");
		return ERROR;
	}
	/* Return increment */
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
word_c readerGetMode(BufferPointer const readerPointer) {
	//printf("* readerGetMode\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 18");
		return ERROR;
	}
	/* Return mode */
	return readerPointer->mode;
}

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
voids readerPrintStat(BufferPointer const readerPointer) {
	//printf("* readerPrintStat\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 19.1");
		printf("Invalid reader pointer.\n");
		return;
	}

	if (!readerPointer->content) {
		printf("Invalid Pointer 19.2");
		printf("No content available.\n");
		return;
	}
	/* Updates the histogram */
	for (int i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] > 0) {
			printf("char '%c' : %d occurences\n", i, readerPointer->histogram[i]);
		}
	}
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
num_i readerGetNumErrors(BufferPointer const readerPointer) {
	//printf("* readerGetNumErrors\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 20");
		return ERROR;
	}
	/* Returns the number of errors */
	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerCalcChecksum
* Purpose: Calculates the checksum of the reader (8 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

voids readerCalcChecksum(BufferPointer readerPointer) {
	//printf("* readerCalcChecksum\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 21");
		return; // if readerPointer is invalid
	}
	/* Calculate checksum */
	for (num_i i = 0; i < readerPointer->positions.wrte; i++) {
		readerPointer->checksum ^= readerPointer->content[i];
	}
}

/*
***********************************************************
* Function name: readerPrintFlags
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

bol readerPrintFlags(BufferPointer readerPointer) {
	//printf("* readerPrintFlags\n"); /* Debug message */
	/* Defensive programming */
	if (!readerPointer) {
		printf("Invalid Pointer 22");
		return FALSE; // false if readerPointer is invalid
	}
	/* Shows flags */
	printf("isEmpty: %s\n", readerPointer->flagbytes[0] & READER_SET_FLAG_EMP ? "True" : "False");
	printf("isFull: %s\n", readerPointer->flagbytes[1] & READER_SET_FLAG_FUL ? "True" : "False");
	printf("isRead: %s\n", readerPointer->flagbytes[2] & READER_SET_FLAG_END ? "True" : "False");
	printf("isMoved: %s\n", readerPointer->flagbytes[2] & READER_SET_FLAG_REL ? "True" : "False");

	return TRUE;
}
