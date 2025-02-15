# Lab 04
## Exercise 01
1) Find the five mistakes inside the map function in megalistmanips.s. Read all of the commented lines under the map function in megalistmanips.s (before it returns with jr ra), and make sure that the lines do what the comments say. Some hints:

   - Why do we need to save stuff on the stack before we call jal?
     > 在调用函数之前，调用者需要将其使用的寄存器（特别是caller-saved寄存器，如t0、t1等）保存到栈上，以防止被被调用者修改。 
   - What’s the difference between add t0, s0, x0 and lw t0, 0(s0)?
     > 前者把s0的值赋给了t0,后者把s0的值指向的地址的内容赋给了t0。
   - Pay attention to the types of attributes in a struct node.
   - Note: you need only focus on the map, mapLoop, and done functions but it’s worth understanding the full program.

2) For this exercise, we are requiring that you don’t use any extra save registers in your implementation. While you normally can use the save registers to store values that you want to use after returning from a function (in this case, when we’re calling f in map), we want you to use temporary registers instead and follow their caller/callee conventions. The provided map implementation only uses the s0 and s1 registers, so we’ll require that you don’t use s2-s11.
3) Make an ordered list of each of the five mistakes, and the corrections you made to fix them.
   > 1. lw a1, 0(s1) -> add a1, s1, x0 
   > 2. la a0, 8(s0) -> lw a0, 8(s0) 
   > 3. add t1, s0, x0 -> lw t1, 0(s0)  
   > 4. 添加调用mystery前后对t1的保存
   > 5. 使用t0*4作为偏移量
4) Save your corrected code in the megalistmanips.s file. Use the -cc flag to run a basic calling convention check on your code locally: 
   ```bash
   java -jar tools/venus.jar lab04/megalistmanips.s -cc 
   ```