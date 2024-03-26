Group Members:
>> Aayan Soni: 2022CSB1061
>> Aditya Garg: 2022CSB1062
>> Ananya Sethi: 2022CSB1066

Link of Github repository:
https://github.com/Adityagarg-github/RISCV-Assembler

Follow the below instructions to run the code:
>> Give trace as input in "input.txt" file.
>> To compile and run the code, enter the below to commands on terminal:
	1. g++ branch_predictor.cpp
	2. ./a.out
>> The program maintains a Branch Table Bufffer and history table for the following branch predictors:
	1. Static "Always Taken" predictor
	2. Static "Always Not Taken" predictor
	3. Dynamic "One bit" predictor
	4. Dynamic "Two bit" predictor
>> You would be asked whether you want to print the Branch Table Buffer on the terminal or not (y/n). 
>> Enter the input accordingly (y/n).
>> Branch table buffer (if entered 'y') and the accuracy of all the four predictors will be printed on terminal.
>> A new file, named "output.txt" would be created that will contain the Branch Table Buffer and the accuracy of all the four predictors (no matter whether you entered 'y' or 'n').

NOTE:
>> Initial State for 1-bit branch predictor is taken as 'N' (Not Taken).
>> Initial State for 2-bit branch predictor is taken as 'SNT' (Strongly Not Taken).
>> These can be changed in the code.

XXXX END XXXX