; Name: Alex N. Vetsavong
; NetID: avetsa2
; Date: 9/20/2018
;
; mp2.asm
;
; This project uses stacks in order to perform four arithmetic operations:
; addition, multiplication, subtraction, and division. Given an input from keyboard,
; any operand would be pushed to the stack and any operator defined by the
; ASCII characters corresponding to '+', '-', '*', '/', and '=' would either
; pop two operands off the stack and perform the appropriate operation before
; pushing the result onto the stack, or the '=' would pop the result of the
; entire expression and print it to the console.
;
; This project works by defining each operation as a subroutine which is called
; by the MAIN program through an evaluation subroutine that parses each ASCII input.
; The calculator assumes that the only valid inputs are single-digit integers (0-9)
; and the operators.
;
; _______________________________REGISTER TABLE_________________________________
; Register___|_____Purpose______________________________________________________
; R0         | Contains outputs of subroutine operations and ASCII character
;            | being evaluated from console
;            |
; R1         | Used as a comparative register for checking current ASCII
;            | character in R0
;            |
; R2         | Used as a temporary register for looking at ASCII value to
;            | prevent tampering with R0
;            |
; R3         | Temporary register in main program and EVALUATE subroutine;
;            | operand for arithmetic subroutines; input for PRINT_HEX subroutine.
;            |
; R4         | Other operand for arithmetic subroutines
;            |
; R5         | Holds the evaluated value of the expression. Also stored in R3
;            | during PRINT_HEX for printing to console.
;            |
; R6         | Used to keep track of the expected signs of evaluations of
;            | multiply and divide operations.

.ORIG x3000

MAIN
  GETC                     ; get input from keyboard
  OUT                      ; echo to console
  JSR EVALUATE             ; evaluate the input character
  BRnzp MAIN               ; keep repeating until '=' is entered

  INVALID_EXPRESSION       ; outputs error message if underflow occurs
    LEA R0, errorMessage   ; or expression is invalid
    PUTS
    HALT

  DONE
    JSR PRINT_HEX          ; echo evaluated value to console
    HALT                   ; exit program

errorMessage .STRINGZ " Invalid Expression"

; PRINT_HEX
;   Description: prints out the value in R3 to the console
;   Inputs: R3 - value in register to be printed out
;   Outputs: Value printed to the console
PRINT_HEX

  ST R7, PRINT_SAVER7
  JSR POP                       ; pop result off stack
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION        ; check for underflow
  ST R0, SAVE_RESULT            ; save result in memory
  ADD R3, R0, #0                ; move result into R3 for printing

  JSR POP                       ; pop item off stack
  ADD R5, R5, #0
  BRz INVALID_EXPRESSION        ; check to see if stack is empty
                                ; if not empty after last pop, invalid expression

  AND R0, R0, #0
  AND R1, R1, #0
  ADD R1, R1, #4

  LD R0, ASCII_SPACE            ; Load ASCII_SPACE into R0
  OUT                           ; print space char for readability.

  printCharacter
      LD R6, INPUT_MASK         ; This allows us to look at only the Most-Significant hex value.
      AND R2, R3, R6            ; Takes MSB of Hex-value and puts it in R0
      JSR CONVERT               ; Jumps to function that converts MSB to equivalent ASCII value
      OUT                       ; Print character

      JSR VALUE_SHIFT_LEFT      ; Shift value in R3 to look at next group.

      ADD R1, R1, #-1           ; Decrement byte counter
      BRp printCharacter        ; If bytes being printed remain, repeat function until done.
      LD R0, ASCII_SPACE
      OUT

      LD R7, PRINT_SAVER7
      LD R5, SAVE_RESULT
      RET

  VALUE_SHIFT_LEFT              ; Will shift the entire value stored in R3 by a whole 4 bits
      ADD R3, R3, R3
      ADD R3, R3, R3
      ADD R3, R3, R3
      ADD R3, R3, R3
      RET

  CONVERT                       ; The operation that converts the hex value to a corresponding ASCII character
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
  ADD R3, R1, R2                ; compare to input valueR5
  BRz MAIN                      ; if space, go back to start; get more inputs.

  LD R1, ASCII_NUM              ; load starting hexadecimal for ASCII 0
  NOT R1, R1
  ADD R1, R1, #1                ; two's complement of ASCII 0

  ADD R3, R1, R2                ; See if input value is a number or operation
  BRn CHECK_OPERATION           ; if negative, input is an operation

  ADD R0, R0, R1                ; if character is a number, change to hex value
  JSR PUSH                      ; push number onto stack
  BRnzp EVALUATE_DONE           ; return to main program for more inputs

  CHECK_OPERATION               ; call operation subroutine corresponding to input
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

  ST R7, MUL_SAVER7                  ; save location of where we were before
  ADD R3, R1, R2                     ; compare this operation to input operation
  BRz PERFORM_MUL                    ; if not right operation, go back to
  RET                                ; previous instruction

  PERFORM_MUL
    JSR POP
    ADD R5, R5, #0
    BRp INVALID_EXPRESSION           ; if underflow error, invalid expression
    ADD R4, R0, #0
    JSR CHECK_R4_MUL
    JSR POP
    ADD R5, R5, #0
    BRp INVALID_EXPRESSION           ; if underflow error, invalid expression
    ADD R3, R0, #0
    JSR CHECK_R3_MUL

    AND R0, R0, #0
    MUL_LOOP_TOP
      ADD R0, R0, R3
      ADD R4, R4, #-1
      BRnz MUL_FIX_SIGN
      BRnzp MUL_LOOP_TOP
    MUL_FIX_SIGN                     ; Compares sign of R0 to the expected sign
    ADD R6, R6, #0                   ; Stored in R6
    BRp MUL_CHECK_R0_NEG
    BRz MUL_CHECK_R0_POS

    NEGATE_R0_MUL                    ; if the sign in R0 is not correct
    NOT R0, R0                       ; take it's 2's complement
    ADD R0, R0, #1
    BRnzp MUL_DONE

    MUL_CHECK_R0_NEG                 ; necessary to check twice to avoid uncovered cases
    ADD R0, R0, #0
    BRn MUL_DONE
    BRzp NEGATE_R0_MUL

    MUL_CHECK_R0_POS
    ADD R0, R0, #0
    BRp MUL_DONE
    BRnz NEGATE_R0_MUL
  MUL_DONE
    JSR PUSH
    LD R7, MUL_SAVER7
    RET

MUL_SAVER7 .BLKW #1

CHECK_R4_MUL
  BRzp CHECK_R4_MUL_DONE                ; if this first operand is a negative
  NOT R4, R4                            ; find 2's complement and set R6 to 1
  ADD R4, R4, #1                        ; to show expected answer is negative
  LD R6, EXPECTED_NEG
  CHECK_R4_MUL_DONE
    RET

CHECK_R3_MUL
  BRzp CHECK_R3_MUL_DONE                ; if second operand is a negative,
  ADD R6, R6, #0                        ; and expected answer is currently positive
  BRz CHECK_R3_MUL_NEGATIVE_ANS         ; set expected answer to be negative
  CHECK_R3_MUL_POSITIVE_ANS             ; else if current expected answer is negative
    LD R6, EXPECTED_POS                 ; set expected answer to be positive
    BRnzp CHECK_R3_MUL_DONE
  CHECK_R3_MUL_NEGATIVE_ANS
    LD R6, EXPECTED_NEG
  CHECK_R3_MUL_DONE
    RET

; DIV
;   Description: divides two numbers (R0 = R3 / R4)
;   Inputs: R3 - numerator
;	    R4 - denominator
;   Outputs: R0 - quotient
DIV

  ST R7, DIV_SAVER7                    ; save location of where we were before
  ADD R3, R1, R2                       ; compare this operation to input operation
  BRz PERFORM_DIV                      ; if not right operation, go back to
  RET                                  ; previous instruction

  PERFORM_DIV
  JSR POP
  ADD R4, R0, #0
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION               ; if underflow error, invalid expression
  JSR POP
  ADD R3, R0, #0
  ADD R5, R5, #0
  BRp INVALID_EXPRESSION               ; if underflow error, invalid expression

  AND R6, R6, #0                       ; make sure R6 initializes as zero
  AND R0, R0, #0                       ; initialize quotient accumulator

  DIV_CHECK_R4
    ADD R4, R4, #0                     ; see if R4 is negative
    BRn DIV_R4_NEGATIVE                ; if R4 is negative, currently expect ans to be negative
    NOT R4, R4                         ; if R4 is not negative, negate it
    ADD R4, R4, #1
    BRnzp DIV_CHECK_R3

    DIV_R4_NEGATIVE
      LD R6, EXPECTED_NEG

  DIV_CHECK_R3                         ; check if R3 is a negative number
    ADD R3, R3, #0
    BRzp DIV_LOOP_TOP                  ; if R3 is positive, skip finding inverse

    NOT R3, R3
    ADD R3, R3, #1

    ADD R6, R6, #0                     ; if R3 is negative, and current expected quotient is positive
                                       ; change expected quotient to negative

    BRz EXPECTED_NEG_QUOTIENT          ; if R3 is negative, and current expected quotient is negative
    LD R6, EXPECTED_POS                ; change expected quotient to positive

    BRnzp DIV_LOOP_TOP

    EXPECTED_NEG_QUOTIENT
    LD R6, EXPECTED_NEG

  DIV_LOOP_TOP                         ; check quotient through repeated subtraction
    ADD R0, R0, #1
    ADD R3, R3, R4
    BRn DIV_CHECK_SIGN
    BRnzp DIV_LOOP_TOP                 ; if not done, keep subtracting until done

  DIV_CHECK_SIGN                       ; Check for the final sign of the quotient
    ADD R0, R0, #-1
    ADD R6, R6, #0
    BRz DIV_DONE                       ; if quotient is expected to be positive,
    NOT R0, R0                         ; skip negating R0
    ADD R0, R0, #1

  DIV_DONE
    JSR PUSH
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

EXPECTED_POS .FILL #0
EXPECTED_NEG .FILL #1

.END
