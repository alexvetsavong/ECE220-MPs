; mp2.asm
; Implement a stack calculator that can do the following operations:
; ADD, SUBTRACT, MULTIPLY, DIVIDE

; Inputs:
;   Console - postfix expression

; Outputs:
;   Console - [0 - F]{4}, evaluation of postfix expression
;   Register R5 - [0 - F]{4}, evaluation of postfix expression

.ORIG x3000

MAIN

; INSERT CODE HERE!
GETC
OUT
JSR EVALUATE
BRnzp MAIN

INVALID_EXPRESSION
  LEA R0, errorMessage
  PUTS
  HALT

DONE
  JSR PRINT_HEX
  HALT

errorMessage .STRINGZ " Invalid Expression"

; PRINT_HEX
;   Description: prints out the value in R3 to the console
;   Inputs: R3 - value in register to be printed out
;   Outputs: Value printed to the console
PRINT_HEX

  ; INSERT CODE HERE!
  ST R7, PRINT_SAVER7
  JSR POP                           ; pop result off stack
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION            ; check for underflow
  ST R0, SAVE_RESULT                ; save result in memory
  ADD R3, R0, #0                    ; move result into R3 for printing

  JSR POP                           ; pop result off stack
  ADD R5, R5, #0
  BRz INVALID_EXPRESSION            ; check to see if stack is empty
                                    ; if not empty after last pop, invalid expression

  AND R0, R0, #0
  AND R1, R1, #0
  ADD R1, R1, #4

  printCharacter
      LD R6, INPUT_MASK             ; This allows us to look at only the Most-Significant hex value.
      AND R2, R3, R6                ; Takes MSB of Hex-value and puts it in R0
      JSR CONVERT                   ; Jumps to function that converts MSB to equivalent ASCII value
      OUT                           ; Print character

      JSR VALUE_SHIFT_LEFT          ; Shift value in R3 to look at next group.

      ADD R1, R1, #-1               ; Decrement byte counter
      BRp printCharacter            ; If bytes being printed remain, repeat function until done.
      LD R0, ASCII_SPACE
      OUT

      LD R7, PRINT_SAVER7
      LD R5, SAVE_RESULT
      RET

  VALUE_SHIFT_LEFT                ; Will shift the entire value stored in R3 by a whole 4 bits
      ADD R3, R3, R3
      ADD R3, R3, R3
      ADD R3, R3, R3
      ADD R3, R3, R3
      RET

  CONVERT                     ; The operation that converts the hex value to a corresponding ASCII character
      AND R5, R5, #0
      AND R4, R4, #0
      ADD R4, R4, #4

  BITSHIFT_LOOP
      LD R6, BIT_SHIFT_MASK     ; Load mask vector into R6
      AND R0, R0, #0            ; Initialize R0 again
      ADD R0, R2, R0            ; Load current bits to be checked into R0
      AND R0, R0, R6            ; Check MSB
      BRn MSB_ONE

  loopContinue
      ADD R4, R4, #-1           ; Decrement bit counter
      BRnz checkNumber          ; Repeat if still checking all four bits
      ADD R2, R2, R2            ; Load next bit to be checked into MSB
      ADD R5, R5, R5            ; Shift bits being temporarily stored in R5
      BRnzp BITSHIFT_LOOP

  checkNumber
      AND R2, R2, #0            ; This will check if the value being printed is a number of letter.
      ADD R2, R5, #0            ;
      ADD R2, R2, #-9           ; If the value is a number, it will
      BRnz numberPrint          ; avoid loading the value #55 into R2.
      LD R2, FIFTY_FIVE
      BRnzp loopExit

  numberPrint
      LD R2, FORTY_EIGHT        ; If the value is a letter, this line will be skipped.

  loopExit
      ADD R5, R5, R2
      AND R0, R0, #0
      ADD R0, R5, #0

      RET

  MSB_ONE
      ADD R5, R5, #1            ; Add the 1 that was shifted out
      BRnzp loopContinue

SAVE_RESULT .BLKW #1
PRINT_SAVER7 .BLKW #1

HEX_VALUE .FILL x1CD9
INPUT_MASK .FILL xF000
BIT_SHIFT_MASK .FILL x8000
FORTY_EIGHT   .FILL #48
FIFTY_FIVE    .FILL #55

; EVALUATE
;   Description: handles input from console
;   Inputs: Console - input
;   Outputs: Console - evaluation of expression or error
;	     R5 - evaluation of expression
EVALUATE

  ; INSERT CODE HERE!
  ST R7, EVALUATE_SAVER7

  ADD R2, R0, #0                ; put ASCII value into R2 for checking

  LD R1, ASCII_EQUAL            ; load ASCII value of '=' into R1
  NOT R1, R1
  ADD R1, R1, #1
  ADD R3, R1, R2                ; compare to input value
  BRz DONE                      ; if equal sign, go to end of main program

  LD R1, ASCII_SPACE            ; load ASCII value of '(space)' into R1
  NOT R1, R1
  ADD R1, R1, #1
  ADD R3, R1, R2                ; compare to input value
  BRz MAIN                      ; if space, go back to start; get more inputs.

  LD R1, ASCII_NUM              ; load starting hexadecimal for ASCII 0
  NOT R1, R1
  ADD R1, R1, #1                ; two's complement of ASCII 0

  ADD R3, R1, R2                ; See if input value is a number or operation
  BRn CHECK_OPERATION           ; if negative, input is an operation

  ADD R0, R0, R1                ; if character is a number, change to hex value
  JSR PUSH                      ; push number onto stack
  BRnzp EVALUATE_DONE           ; return to main program for more inputs

  CHECK_OPERATION               ; perform operation corresponding to the input
    LD R1, ASCII_STAR
    NOT R1, R1
    ADD R1, R1, #1
    JSR MUL

    LD R1, ASCII_PLUS
    NOT R1, R1
    ADD R1, R1, #1
    JSR PLUS

    LD R1, ASCII_MIN
    NOT R1, R1
    ADD R1, R1, #1
    JSR MIN

    LD R1, ASCII_DIV
    NOT R1, R1
    ADD R1, R1, #1
    JSR DIV

  EVALUATE_DONE
  LD R7, EVALUATE_SAVER7
  RET

EVALUATE_SAVER7 .BLKW #1

  ; values needed for program to evaluate inputs
  ASCII_NUM   .FILL x0030

  ASCII_STAR  .FILL x002A
  ASCII_PLUS  .FILL x002B
  ASCII_MIN   .FILL x002D
  ASCII_DIV   .FILL x002F
  ASCII_EQUAL .FILL x003D
  ASCII_SPACE .FILL x0020

; PLUS
;   Description: adds two numbers (R0 = R3 + R4)
;   Inputs: R3 - addend
;	    R4 - addend
;   Outputs: R0 - sum
PLUS

  ; INSERT CODE HERE!
  ST R7, PLUS_SAVER7                   ; save location of where we were before
  ADD R3, R1, R2                       ; compare this operation to input operation
  BRz PERFORM_PLUS                     ; if not right operation, go back to
  RET                                  ; previous instruction

  PERFORM_PLUS
  JSR POP
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION               ; check for underflow when popping
  ADD R3, R0, #0                       ; move popped value into R3
  JSR POP
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION               ; check for underflow when popping
  ADD R4, R0, #0                       ; move popped value into R4
  ADD R0, R3, R4
  JSR PUSH                             ; push result onto stack
  LD R7, PLUS_SAVER7
  RET
PLUS_SAVER7 .BLKW #1

; MIN
;   Description: subtracts two numbers (R0 = R3 - R4)
;   Inputs: R3 - minuend
;	    R4 - subtrahend
;   Outputs: R0 - difference
MIN

  ; INSERT CODE HERE!

  ST R7, MIN_SAVER7                    ; save location of where we were before
  ADD R3, R1, R2                       ; compare this operation to input operation
  BRz PERFORM_MIN                      ; if not right operation, go back to
  RET                                  ; previous instruction

  PERFORM_MIN
  JSR POP
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION               ; if underflow, invalid expression
  ADD R4, R0, #0
  JSR POP
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION               ; if underflow, invalid expression
  ADD R3, R0, #0

  NOT R4, R4
  ADD R4, R4, #1

  ADD R0, R3, R4
  JSR PUSH
  LD R7, MIN_SAVER7
  RET
MIN_SAVER7 .BLKW #1

; MUL
;   Description: multiplies two numbers (R0 = R3 * R4)
;   Inputs: R3 - factor
;	    R4 - factor
;   Outputs: R0 - product
MUL

  ; INSERT CODE HERE!


  ST R7, MUL_SAVER7                    ; save location of where we were before
  ADD R3, R1, R2                       ; compare this operation to input operation
  BRz PERFORM_MUL                      ; if not right operation, go back to
  RET                                  ; previous instruction

  PERFORM_MUL
  JSR POP
  ADD R4, R0, #0
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION               ; if underflow error, invalid expression
  JSR POP
  ADD R3, R0, #0
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION               ; if underflow error, invalid expression


  MUL_LOOP_TOP
    AND R0, R0, #0
    ADD R0, R3, #0
    ADD R4, R4, #-1
    BRnz MUL_DONE
    BRnzp MUL_LOOP_TOP

  MUL_DONE
    JSR PUSH
    LD R7, MUL_SAVER7
    RET
MUL_SAVER7 .BLKW #1

; DIV
;   Description: divides two numbers (R0 = R3 / R4)
;   Inputs: R3 - numerator
;	    R4 - denominator
;   Outputs: R0 - quotient
DIV

  ; INSERT CODE HERE!

  ST R7, DIV_SAVER7                    ; save location of where we were before
  ADD R3, R1, R2                       ; compare this operation to input operation
  BRz PERFORM_DIV                      ; if not right operation, go back to
  RET                                  ; previous instruction

  PERFORM_DIV
  ADD R4, R0, #0
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION               ; if underflow error, invalid expression
  JSR POP
  ADD R3, R0, #0
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION               ; if underflow error, invalid expression

  NOT R4, R4
  ADD R4, R4, #1

  DIV_LOOP_TOP
    ADD R0, R0, #1
    ADD R3, R4, #0
    BRnz DIV_DONE
    BRnzp DIV_LOOP_TOP

  DIV_DONE
    LD R7, DIV_SAVER7
    RET
DIV_SAVER7 .BLKW #1

; PUSH
;   Description: Pushes a charcter unto the stack
;   Inputs: R0 - character to push unto the stack
;   Outputs: R5 - 0 (success) or 1 (failure/overflow)
;   Registers: R3 - stores STACK_END
;	       R4 - stores STACK_TOP
PUSH
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW			;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


; POP
;   Description: Pops a character off the stack
;   Inputs:
;   Outputs: R0 - character popped off the stack
;	     R5 - 0 (success) or 1 (failure/underflow)
;   Registers: R3 - stores STACK_END
;	       R4 - stores STACK_TOP

POP
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;


.END
