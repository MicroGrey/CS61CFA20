.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 88.
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 90.
# - If you receive an fread error or eof,
#   this function terminates the program with error code 91.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 92.
# ==============================================================================
read_matrix:

    # Prologue
    addi sp, sp, -24
    sw ra, 0(sp)
    sw s0, 4(sp)
	sw s1, 8(sp)      
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)

    mv s0, a1 # s0 = rows
    mv s1, a2 # s1 = cols
    mv s3, a0 # s3 = filename

    # allocates 4 bytes buffer for per integer reading. the buffer address will be in s4
    li a0, 4
    jal malloc
    beq a0, x0, malloc_error
    mv s4, a0

    # open file
    mv a1, s3
    li a2, 0
    jal fopen
    li t0, -1
    beq a0, t0, fopen_error
    mv s3, a0 # s3 is now file discriptor

    # read rows
    add a1, s3, x0
    add a2, s4, x0
    li a3, 4
    jal fread
    li t0, 4
    bne a0, t0, fread_error
    lw t0, 0(s4)
    sw t0, 0(s0)

    mv s0, t0 # s0 = rows

    # read cols
    add a1, s3, x0
    add a2, s4, x0
    li a3, 4
    jal fread
    li t0, 4
    bne a0, t0, fread_error
    lw t0, 0(s4)
    sw t0, 0(s1)

    mul s0, t0, s0 # s0 = cols * rows
    slli s0, s0, 2 # s0 = cols * rows * 4

    # malloc
    add a0, x0, s0
    jal malloc
    mv s2, a0 # s2 is return address

    beq a0, x0, malloc_error
    li t1, 0 # the counter
    # addi s0, s0, -4 # s0 = s0 - 4

read_loop:
    # read
    add a1, s3, x0
    add a2, s4, x0
    addi a3, x0, 4
    
    addi sp, sp, -4
    sw t1, 0(sp)
    jal fread
    lw t1, 0(sp)
    addi sp, sp, 4

    li t0, 4
    bne a0, t0, fread_error

    lw t0, 0(s4)

    add t2, s2, t1
    sw t0, 0(t2)

    addi t1, t1, 4
    bne t1, s0, read_loop

done: 
    # close
    mv a0, s3
    jal fclose
    li t0, -1
    beq a0, t0, fclose_error


    # Epilogue
    addi a0, s2, 0
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    addi sp, sp, 24

    ret



malloc_error:
    li a1, 88
    j exit2

fopen_error:
    li a1, 90
    j exit2

fread_error:
    li a1, 91
    j exit2

fclose_error:
    li a1, 92
    j exit2
