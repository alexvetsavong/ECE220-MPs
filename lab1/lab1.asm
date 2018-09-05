.ORIG x3000
; Write your code here

; Alex Vetsavong
; 08/31/2018
; ECE220 Lab 1
;
; This program will print out the ASCII characters representing the Hexadecimal value
; Stored in a register, R3. This is accomplished by masking the most signficant byte,
; and then checking the parity of its most significant bit, which is circularly left-shifted.
; This is stored in another register until all four bits in the byte are shifted. From there,
; the proper offset is used to convert a hexadecimal byte to its representative ASCII character,
; stored in R0, and then printed using the OUT subroutine of the TRAP operation.

; R0 contains the Hex value of the ASCII character being printed
; R1 is used as a counter for the number of bytes remaining in the value being printed.
; R2 is used for checking the parity of a hexadecimal byte
; R3 holds the hexadecimal value to be printed to monitor as ASCII characters
; R4 used as a counter for the number of bits left to be checked in a byte
; R5 temporarily holds the shifted values of R0
; R6 will be used for any vector masks that are needed

LD R3, HEX_VALUE
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

                HALT

VALUE_SHIFT_LEFT                              ; Will shift the entire value stored in R3 by a whole 4 bits
                ADD R3, R3, R3
                ADD R3, R3, R3
                ADD R3, R3, R3
                ADD R3, R3, R3
                RET

CONVERT                                   ; The operation that converts the hex value to a corresponding ASCII character
                AND R5, R5, #0
                AND R4, R4, #0
                ADD R4, R4, #4
BITSHIFT_LOOP   LD R6, BIT_SHIFT_MASK     ; Load mask vector into R6
                AND R0, R0, #0            ; Initialize R0 again
                ADD R0, R2, R0            ; Load current bits to be checked into R0
                AND R0, R0, R6            ; Check MSB
                BRn MSB_ONE
loopContinue    ADD R4, R4, #-1           ; Decrement bit counter
                BRnz checkNumber          ; Repeat if still checking all four bits
                ADD R2, R2, R2            ; Load next bit to be checked into MSB
                ADD R5, R5, R5            ; Shift bits being temporarily stored in R5
                BRnzp BITSHIFT_LOOP



checkNumber     AND R2, R2, #0            ; This will check if the value being printed is a number of letter.
                ADD R2, R5, #0            ;
                ADD R2, R2, #-9           ; If the value is a number, it will
                BRnz numberPrint          ; avoid loading the value #55 into R2.
                LD R2, FIFTY_FIVE
                BRnzp loopExit
numberPrint     LD R2, FORTY_EIGHT        ; If the value is a letter, this line will be skipped.

loopExit        ADD R5, R5, R2
                AND R0, R0, #0
                ADD R0, R5, #0
                RET

MSB_ONE         ADD R5, R5, #1            ; Add the 1 that was shifted out
                BRnzp loopContinue

HEX_VALUE .FILL x1CD9
INPUT_MASK .FILL xF000
BIT_SHIFT_MASK .FILL x8000
FORTY_EIGHT   .FILL #48
FIFTY_FIVE    .FILL #55

.END
