.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 75.
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 76.
# =======================================================
dot:
    # Prologue
    addi sp, sp, -12
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)

    mv s0, a0
    mv s1, a1

    # Check if the length of the vector is less than 1
    ble a2, x0, length_end
    # Check if the stride of either vector is less than 1
    ble a3, x0, stride_end
    ble a4, x0, stride_end

    slli t0, a3, 2
    slli t1, a4, 2 

    add t5, x0, x0
    add t4, x0, x0

loop_start:
    lw t2, 0(s0)
    lw t3, 0(s1)
    mul t2, t2, t3
    add t5, t5, t2

    addi t4, t4, 1
    add s0, s0, t0
    add s1, s1, t1
    blt t4, a2, loop_start
loop_end:
    # Epilogue
    mv a0, t5

    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    addi sp, sp, 12
    
    ret

length_end:
    li a7, 75
    ecall   

stride_end:
    li a7, 76
    ecall