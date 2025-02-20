.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 89.
    # - If malloc fails, this function terminats the program with exit code 88.
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>

    # prologue
    addi sp, sp, -40
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)
    sw s5, 24(sp)
    sw s6, 28(sp)
    sw s7, 32(sp)
    sw s8, 36(sp)

    # load a0 - a2
    mv s0, a0 # s0 = argc
    mv s1, a1 # s1 = argv
    mv s2, a2 # s2 = print_classification

    # Check if there are an incorrect number of command line args
    li t0, 5
    bne t0, s0, exit_89

	# =====================================
    # LOAD MATRICES
    # =====================================

    # malloc space for column and row sizes
    li a0, 12
    jal malloc
    beq a0, x0, exit_88
    mv s3, a0 # s3 = rows

    li a0, 12
    jal malloc
    beq a0, x0, exit_88
    mv s4, a0 # s4 = cols

    # Load pretrained m0
    lw a0, 4(s1)
    mv a1, s3
    mv a2, s4
    jal read_matrix
    mv s5, a0 # s5 = m0

    # Load pretrained m1
    lw a0, 8(s1)
    addi a1, s3, 4
    addi a2, s4, 4
    jal read_matrix
    mv s6, a0 # s6 = m1

    # Load input matrix
    lw a0, 12(s1)
    addi a1, s3, 8
    addi a2, s4, 8
    jal read_matrix
    mv s7, a0 # s7 = input_matrix

    lw s8, 16(s1) # s8 = output_path

    # =====================================
    # RUN LAYERS
    # =====================================

    # 1. LINEAR LAYER:    m0 * input

    lw t0, 0(s3) # rows of m0
    lw t1, 0(s4) # cols of m0
    lw t2, 8(s3) # rows of input
    lw t3, 8(s4) # cols of input

    mul s1, t0, t3 # s1 = rows of m0 * cols of input
    slli s1, s1, 2 # s1 = rows of m0 * cols of input * 4

    add a0, s1, x0
    jal malloc
    beq a0, x0, exit_88
    mv s0, a0 # s0 = pointer of d

    add a0, s5, x0 # m0
    lw a1, 0(s3) # rows of m0
    lw a2, 0(s4) # cols of m0
    add a3, s7, x0 # input_matrix
    lw a4, 8(s3) # rows of input_matrix
    lw a5, 8(s4) # cols of input_matrix
    add a6, s0, x0 # d
    jal matmul # s0 = m0 * input_matrix

    # 2. NONLINEAR LAYER: ReLU(m0 * input)

    add a0, s0, x0 # d
    srli a1, s1, 2 # size of d
    jal relu # ReLU(d)

    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)

    lw t0, 4(s3) # rows of m1
    lw t1, 4(s4) # cols of m1
    lw t2, 0(s3) # rows of ReLU(m0 * input)
    lw t3, 8(s4) # cols of ReLU(m0 * input)

    mul s1, t0, t3 # rows of m1 * cols of ReLU(m0 * input)
    slli s1, s1, 2 # rows of m1 * cols of ReLU(m0 * input) * 4
    add a0, s1, x0
    jal malloc
    beq a0, x0, exit_88
    mv s1, a0 # pointer of d
    
    add a0, s6, x0 # m1
    lw a1, 4(s3) # rows of m1
    lw a2, 4(s4) # cols of m1
    add a3, s0, x0 # ReLU(m0 * input)
    lw a4, 0(s3) # rows of ReLU(m0 * input)
    lw a5, 8(s4) # cols of ReLU(m0 * input)
    add a6, s1, x0 # d
    jal matmul # d = m1 * ReLU(m0 * input)

    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix
    mv a0, s8 # output_path
    mv a1, s1 # d
    lw a2, 4(s3) # rows of d
    lw a3, 8(s4) # cols of d

    jal write_matrix

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax
    add a0, s1, x0 # d
    lw t0, 4(s3) # rows of d
    lw t1, 8(s4) # cols of d
    mul a1, t0, t1 # rows of d * cols of d
    jal argmax
    mv s0, a0 # classification

    # Print classification
    bne s2, x0, done
    add a1, s0, x0
    jal print_int

    # Print newline afterwards for clarity
    li a1 '\n'
    jal print_char

done:
    mv a0, s0
    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp)
    lw s7, 32(sp)
    lw s8, 36(sp)
    addi sp, sp, 40

    ret


exit_88:
    li a1, 88
    j exit2

exit_89:
    li a1, 89
    j exit2