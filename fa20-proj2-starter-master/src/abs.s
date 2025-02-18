.globl abs

.text
# =================================================================
# FUNCTION: Given an int return its absolute value.
# Arguments:
# 	a0 (int) is input integer
# Returns:
#	a0 (int) the absolute value of the input
# =================================================================
abs:
    # Prologue
    addi sp, sp, -4
    sw ra, 0(sp)

    # return 0
    bge a0, x0, abs_end
    neg a0, a0
abs_end:
    # Epilogue
    lw ra, 0(sp)
    addi sp, sp, 4
    ret
