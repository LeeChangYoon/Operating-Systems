# Operating-System
Repository for Operating System class

# Index
1. [Homework1 - Simple MyShell](#Homework1-Simple-MyShell)
2. [Homework2 - Multi Threaded Word Count](#Homework2-Multi-Threaded-Word-Count)
3. [Project1 - Simple Scheduling](#Project1-Simple-Scheduling)
4. [Project2 - Virtual Memory](#Project2-Virtual-Memory)
5. [Project3 - Simple File System](#Project3-Simple-File-System)

<br><br>
# Homework1-Simple-MyShell

## Description
Most operating systems, including LINUX, UNIX, and Windows, contain the command
interpreter as a program that runs when a process is initiated or when a user first logs in (on
interactive systems). In a system with multiple command interpreters to choose from,
interpreters are known as shells (Silberschatz et al., 2014). The main function of the shell,
which is also known as the command interpreter, is to get and execute the user-specified
commands. Many of the commands given at this level manipulate files: create, delete, list,
print, copy and execute, etc. In this paper, we will discuss the concepts and methods that are
used in the shell, which is the command interpreter. Also, we will present our
implementation and the results of the execution of the shell, which is the simple shell (SiSH).

## Requirements
- Requirements

![image](![image](https://user-images.githubusercontent.com/41320453/209906783-e8b373e3-a7b5-49c0-b0d1-fb864d9e0a38.png))

## Simple MyShell (SiSH)
- Simple MyShell (SiSH)

![image](![image](https://user-images.githubusercontent.com/41320453/209906896-96f6254a-790b-4bc6-be1b-c0f52068b53a.png))

- Example of Command Execution on SiSH

![image](![image](https://user-images.githubusercontent.com/41320453/209906962-6b05e498-fbb2-4c75-ac0e-367cbfa6ef31.png))

## Environment
- Build Environments:
1. Linux environment – Vi editor, GCC complier
2. Program is built by using the Makefile.

- Make command:
1.	$make SiSH -> build the execution program of Simple Shell
2.	$make clean -> clean all of the object files that consists of the main function
<br><br>

# Homework2-Multi-Threaded-Word-Count

## Description
In this project, we will first explain the concepts of thread, multi-thread, problems along the multi-threaded programming, and their solutions. By applying these concepts, we will explain how we implemented the multi-threaded word count program and its results for versions 1 through 3. Also, we will show the optimization with some methods to present the enhanced performance. At the end of the paper, we will present the execution time among the differences between the number of threads and implementation methods.
 
## Requirements
- Requirements

![image](![image](https://user-images.githubusercontent.com/41320453/209907059-ba5c9caf-34a5-452f-8df4-4a2c889f4048.png))

## Multi-Threaded-Word-Count
- Reading FreeBSD9-orig/ObsoleteFiles.inc with Producer and Consumer Version 3 

![image](![image](https://user-images.githubusercontent.com/41320453/209907146-ed813970-2ea2-4f9f-a3e0-5da0ecfecdca.png))

- Execution Time per Number of Threads on Program Version 2.2 of android.tar

![image](![image](https://user-images.githubusercontent.com/41320453/209907196-2772975f-b277-4414-9f2e-55ceb77a373a.png))

## Environment
- Build Environments:
1. Linux environment – Vi editor, GCC complier
2. Program is built by using the Makefile.

- Make command:
1.	$make prod_cons -> build the execution program for prod_cons from version 1 to 4.
2.	$make clean -> clean all the object files that consists of the prod_cons programs.

-	Execution Command:
1.	./Prod_cons_v1 {$readfile} 
-> Execute the producer and consumer version 1 program.
2.	./Prod_cons_v2.1 {$readfile} #Producer #Consumer
-> Execute the producer and consumer version 2.1 program.
3.	./Prod_cons_v2.2 {$readfile} #Producer #Consumer
-> Execute the producer and consumer version 2.2 program.
4.	./Prod_cons_v2.3 {$readfile} #Producer #Consumer
-> Execute the producer and consumer version 2.3 program.
5.	./Prod_cons_v3 {$readfile} #Producer #Consumer
-> Execute the producer and consumer version 3 program.
6.	./Prod_cons_v4 {$readfile} #Producer #Consumer
-> Execute the producer and consumer version 4 program.
<br><br>

# Project3-Pipelined-MIPS-Simulator

## Description
 The work presented in this project is the third part of a large project designed to implement and optimize the 
microarchitecture that uses the MISP ISA. We will apply the advanced concepts to the former single-cycle microarchitecture to enhance the performance of the microarchitecture. 
 The first step in this project is to specify the requirements for the pipelined microarchitecture. Second, we move on to 
concepts critically related to microarchitecture: multi-cycle microarchitecture, pipelined microarchitecture, performance 
analysis, data dependency handling, control dependency handling, and branch prediction. Third, we will state the data path, 
which includes the latches, forwarding unit, data hazard detection unit, branch prediction unit, and program definitions. Fourth, we will describe how we implemented the data paths and pipelined MIPS simulator according to the hardware components and program definitions. Then, there will be some results by executing the binary programs using an implemented simulator. In the end, we will evaluate the pipelined MIPS simulator with performance comparison with the single-cycle MISP simulator, and the flow of the data paths based on some assumptions.

## Requirements
- Requirements

![image](https://user-images.githubusercontent.com/41320453/174941467-6313ac64-9178-4dea-b314-b02767cb214a.png)

## Pipeline
- Pipelined Microarchitecture

![image](https://user-images.githubusercontent.com/41320453/174941521-258992a5-8749-4d88-8455-fe88843ce27c.png)

## Environment
- Build Environments:
1. Linux environment – Vi editor, GCC complier
2. Program is built by using the Makefile.

- Make Command:
1. $make cpu -> build the execution program
2. $make clean -> clean all the object files that builds main

- Execution Command
1. ./cpu test_prog/{$program} 1 -> Execute the program with Always Not Taken branch predictor.
2. ./cpu test_prog/{$program} 2 -> Execute the program with Always Taken branch predictor.
3. ./cpu test_prog/{$program} 3 -> Execute the program with 1-bit Last Time branch predictor.
4. ./cpu test_prog/{$program} 4 -> Execute the program with 2-bit Counter-Based branch predictor.
5. ./cpu test_prog/{$program} 5 -> Execute the program with 2-Level Global branch predictor.
6. ./cpu test_prog/{$program} 6 -> Execute the program with 2-Level Local branch predictor.
<br><br>

# Project4-Pipelined-MIPS-Simulator-with_Cache

## Description
In this project, we will mainly discuss the cache operation, implementation, and evaluation in the pipelined microarchitecture. 
The works presented in this project are the last part of a large project designed to implement and optimize the 
microarchitecture that uses MIPS ISA. We will add the concept of the cache into the pipelined microarchitecture to enhance the 
performance of the microarchitecture.
The first step in this project is to specify the requirements for the cache structure. Second, we will move on to the concepts 
for the cache: memory hierarchy, memory locality, performance, types of the cache, cache replacement policies, and writeback
policies. Third, we will state the program definition for our cache implementation. Fourth, we will describe how we 
implemented the cache with various cache replacements and write-back policies. Then, there will be some results for each 
execution of the binary program by the cache MIPS simulator. At the end of this project, we will evaluate the cache MIPS 
simulator with the performance comparison, and the memory latency based on some assumptions.

## Requirements
- Requirements

![image](https://user-images.githubusercontent.com/41320453/174941653-31c4663e-7e4a-46fa-9ed9-ca7fa22b6e86.png)

## Cache
- Cache Structure in CPU

![image](https://user-images.githubusercontent.com/41320453/174941715-5539b70b-f7e5-4067-b205-238a6611d3de.png)

## Environment
- Build Environments:
1. Linux environment – Vi editor, GCC complier
2. Program is built by using the Makefile.

- Make Command:
1. $make cpu -> build the execution program
2. $make clean -> clean all the object files that builds main

- Execution Command
5. ./cpu test_prog/{$program} 1-x-x x -> Execute the program with Fully Associative Cache.
6. ./cpu test_prog/{$program} 2-x-x x -> Execute the program with Direct Mapped Cache.
7. ./cpu test_prog/{$program} 3-x-x x -> Execute the program with Set Associative Cache.
8. ./cpu test_prog/{$program} x-0-x x -> Execute the program with Write Through Policy.
9. ./cpu test_prog/{$program} x-1-x x -> Execute the program with Write Back Policy.
10. ./cpu test_prog/{$program} x-x-0 x -> Execute the program with FIFO Algorithm.
11. ./cpu test_prog/{$program} x-x-1 x -> Execute the program with Random Algorithm.
12. ./cpu test_prog/{$program} x-x-2 x -> Execute the program with LRU Algorithm.
13. ./cpu test_prog/{$program} x-x-3 x -> Execute the program with LFU Algorithm.
14. ./cpu test_prog/{$program} x-x-4 x -> Execute the program with Second Chance Algorithm.
15. ./cpu test_prog/{$program} x-x-5 x -> Execute the program with Enhanced Second Chance Algorithm.

- CSV File Generation
1. First, if we execute the cache MIPS simulator, we can get the result.txt file. 
2. Second, open the Data_Purification.ipnyb and upload the result.txt on it, and then execute all the programs. 
3. As a result, we can get the following CSV files that contain the results of our cache MIPS simulator.
