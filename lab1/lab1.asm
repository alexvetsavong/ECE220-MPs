.ORIG x3000
; Write your code here

; R0 contains the Hex value of the ASCII character being printed
; R1 is used as a counter for the number of bytes remaining in the value being printed.
; R2 used for conditional checks and temporary values
; R3 holds the hexadecimal value to be printed to monitor as ASCII characters
; R4 used as a counter for the number of bits left to be checked in a byte
; R5 temporarily holds the shifted values of R0
; R6 will be used for any vector masks that are needed


LD R3, HEX_VALUE
AND R0, R0, #0
ADD R0, R3, #0
AND R1, R1, #0
ADD R1, R1, #4

printCharacter
                LD R6, INPUT_MASK             ; This allows us to look at only the Most-Significant Byte
                AND R0, R3, R6                ; Takes MSB of Hex-value and puts it in R0
                JSR CONVERT                   ; Jumps to function that converts MSB to equivalent ASCII value
                OUT                           ; Print character

                JSR BYTE_SHIFT_LEFT           ; Shift value in R3 to look at next byte

                ADD R1, R1, #-1               ; Decrement byte counter
                BRp printCharacter            ; If bytes being printed remain, repeat function until done.

                HALT


BYTE_SHIFT_LEFT
                ADD R3, R3, R3
                ADD R3, R3, R3
                ADD R3, R3, R3
                ADD R3, R3, R3
                RET

CONVERT
                AND R4, R4, #0
                ADD R4, R4, #4
BITSHIFT_LOOP   LD R6, BIT_SHIFT_MASK     ; Load mask vector into R6
                AND R0, R0, R6            ; Look only at current MSB
                BRzp MSB_ZERO             ; If there is a zero in the MSB, skip adding to R5
                ADD R5, R5, #1            ; Add the 1 that was shifted out
MSB_ZERO        ADD R5, R5, R5            ; Shift left R5 over one bit
                ADD R0, R3, R3            ; Bitshift HEX byte by one bit
                ADD R4, R4, #-1
                BRp BITSHIFT_LOOP         ; Repeat if still checking all four bits

                LD R2, FIFTY_FIVE
                ADD R5, R5, R2
                AND R0, R0, #0
                ADD R0, R5, #0
                RET

HEX_VALUE .FILL xBEEF
INPUT_MASK .FILL xF000
BIT_SHIFT_MASK .FILL x8000
FIFTY_FIVE .FILL #55
.END
