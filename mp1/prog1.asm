; Alex Vetsavong
; 09/06/2018
; ECE 220
; MP1
;
; The code given to you here implements the histogram calculation that
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of
; code to print the histogram to the monitor.
;
; If you finish your program,
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string
; terminated by a NUL character.  Lower case and upper case should
; be counted together, and a count also kept of all non-alphabetic
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance)
; should be stored starting at x3F00, with the non-alphabetic count
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram

	; fill the histogram with zeroes
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop



PRINT_HIST

; you will need to insert your code to print the histogram here

; do not forget to write a brief description of the approach/algorithm
; for your implementation, list registers used in this part of the code,
; and provide sufficient comments

; This code prints out the histogram for the frequency of characters in a string.
; It does so by loading the frequency stored in memory starting at x3F00,
; using the memory address register value to print the bin labels. This is
; done by masking the first two characters and applying the appropriate
; offset to the hexadecimal value to print the corresponding ASCII.
; The value stored in MDR is printed as an ASCII string by circularly left-
; shifting the bits through the use of a series of ADDs and conditional branches.
; Every four bits, the character is identified as either a number or letter.
; The appropriate offset is applied depending on which it is, and then it is
; output to the screen using the OUT subroutine. This is done for every four
; bits, until all 27 bins are filled.


;	R0 holds the ASCII character being printed
;	R1 is a pointer keeping track of what letter is being looked at.
;	R2 is used as a temporary register
;	R3 is used to manipulate the data being printed
; R4 is used as a temporary register
;	R5 is a counter used to keep track of which bit is being looked at.
;	R6 is a counter to keep track of which bin is being printed

BIN_LABELS
	LD R6, NUM_BINS					; load the number of bins into R6 for counting
	LD R1, HIST_ADDR				; load the starting address into R1
	PRINT_LOOP_TOP
	AND R0, R0, #0					; clear R0 before pointing to next location
	ADD R0, R1, #0					;	place the location into R0 for manipulating
	LD R2, ADDRESS_MASK 		; load the mask stored at label into R2
	AND R0, R0, R2					;	look only at the least two hexadecimal characters
	LD R2, HEX_ASCII_DIFF 	; add necessary offset to print the correct character
	ADD R0, R0, R2					; set R0 to the ASCII value of current character
													; being printed
	OUT
	LD R0, SPACE_CHAR				; get R0 ready to print a space.
	OUT

BIN_DATA
	AND R2, R2, #0								; clear R2 to hold shifted bits
	LD R5, FOUR										; initialize counter for bits of data being printed
	LDR R3, R1, #0								; take the value stored at the location defined by
																; R1
CHECK_LOOP
	AND R2, R2, #0
	ADD R4, R3, #0								; Check first bit
	JSR MSB_CHECK
	ADD R4, R3, R3								; check next three bits
	JSR MSB_CHECK
	ADD R4, R4, R4
	JSR MSB_CHECK
	ADD R4, R4, R4
	JSR MSB_CHECK
	ADD R0, R2, #0								; load character into R0 from temporary register, R2
	JSR CHAR_CHECK
	ADD R0, R0, R2								; set correct offset for value being printed.
	OUT														; prints current character
	JSR CHAR_SHIFT								; moves on to the next character
	ADD R5, R5, #-1								; check if there are any more characters to be printed
	BRp CHECK_LOOP								; if not, go back to the top of the loop and check again.

NEXT_BIN
	LD R0, NEW_LINE
	OUT
	ADD R1, R1, #1								; move on to the next bin
	ADD R6, R6, #-1								; decrement bin counter by 1
	BRnz DONE											; if there are no more bins to print, then halt
	BRnzp PRINT_LOOP_TOP					; else, go back to top of printing loop.


MSB_CHECK
	BRn MSB_ONE										; if MSB was one, bitshift R2 before adding 1.
	ADD R2, R2, R2
	RET

	MSB_ONE ADD R2, R2, R2				; this branch prevents excessive shifting
	ADD R2, R2, #1
	RET

CHAR_CHECK
	ADD R2, R0, #-9	; checks if the value is a number or letter
	BRnz NUMBER_CHAR	; if the character is a number, add correct offset to R0
	LD R2, HEX_TO_LETTER	; loads proper offset for letter
	RET
	NUMBER_CHAR LD R2, HEX_TO_NUMBER ; loads proper offset for number
	RET

CHAR_SHIFT  					; looks at the next character being printed by bitshifting four times.
	ADD R3, R3, R3
	ADD R3, R3, R3
	ADD R3, R3, R3
	ADD R3, R3, R3
	RET

DONE	HALT			; done


; the data needed by the program
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00     ; histogram starting address
STR_START	.FILL x4000	; string starting address

;	data needed for printing the histogram
SPACE_CHAR .FILL x0020			; the ASCII value of the space characters
HEX_ASCII_DIFF .FILL x0040 	; the difference between 0x00 and the first
														; ASCII symbol, 0x40.
ADDRESS_MASK		.FILL x00FF	;	used to ignore the first two hexadecimal
														; characters in the memory address
NEW_LINE				.FILL x000A	; value to print a new line.
FOUR						.FILL #4		; constant for bit counter
HEX_TO_LETTER		.FILL #55		; decimal difference between 0x0A and 0x41
HEX_TO_NUMBER		.FILL #48		; decimal difference between 0x00 and 0x30

; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END
