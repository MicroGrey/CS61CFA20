.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 77.
# =================================================================
argmax:
    # Prologue
    addi sp, sp, -16
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)

    add s0, a0, x0
    add s1, x0, x0
    add t2, a0, x0

loop_start:
    lw t0, 0(s0)
    lw t1, 0(t2)

    ble t0, t1, loop_continue
    add s2, s1, x0
    add t2, s0, x0


loop_continue:
    addi s0, s0, 4
    addi s1, s1, 1

    bne s1, a1, loop_start

loop_end:
    
    mv a0, s2 
    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    addi sp, sp, 16
    ret
