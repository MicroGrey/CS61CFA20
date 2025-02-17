# Lab 06
## Exercise 01
- All this circuit does is take in two inputs, multiply them together, and then add the result to the current state value. For this circuit, let the propagation delay of an adder block be 45ns and the propagation delay of a multiplication block be 60ns. The register has a CLK-to-Q delay of 10ns, setup time of 10ns, and hold time of 5ns. Calculate the maximum clock rate at which this circuit can operate. Assume that both inputs come from clocked registers that receive their data from an outside source.
     > $\text{clock rate} =\frac{1}{(45 \; + \; 60 \; + \; 10 \; + \; 10) \;  \text{ns} } =$ 8MHz  
## Exercise 02
- 给每个过程后加寄存器，加和减成为两个独立的pipeline.
     > After pipelining:  
     > **max_delay** = $\max$ (CLK_to_Q_Delay + Adder_Delay +Setup_Time , CLK_to_Q_Delay + multiplication_Delay + Setup_Time)  
     > $= \max (10 \text{ns} + 45 \text{ns} + 10 \text{ns} , 10 \text{ns} + 60 \text{ns} + 10 \text{ns} ) = 80 \text{ns}$  
     > **clock_rate** $=\frac{1}{80 \;  \text{ns} } =$ 12.5MHz 
- We discussed that if a computation depends on the output of a previous computation, it’s difficult to pipeline them and we often need to insert a pipeline “bubble” (or several) to ensure that the output of the first computation is ready to be an input to the second. As a reminder a bubble is the process of purposely delaying an instruction in the pipeline. It is important to understand why such “bubbles” are unnecessary for this particular circuit.
     > - 在这个电路中，乘法和加法有不同的延迟：乘法的延迟较高（60ns），而加法的延迟较低（45ns）。通过在乘法和加法之间插入寄存器，可以确保加法器能够立即使用乘法的结果，而不需要等待乘法器完成所有计算。因此，乘法的结果在寄存器中存储并可供加法操作使用，不需要气泡来等待前一阶段的结果。  
     > - 乘法和加法是独立的。通过流水线化，每个计算阶段（乘法和加法）都有自己的寄存器，可以保证每个阶段在自己的时钟周期内执行。