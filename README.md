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
1.	./Prod_cons_v1 {$readfile} -> Execute the producer and consumer version 1 program.
2.	./Prod_cons_v2.1 {$readfile} #Producer #Consumer -> Execute the producer and consumer version 2.1 program.
3.	./Prod_cons_v2.2 {$readfile} #Producer #Consumer -> Execute the producer and consumer version 2.2 program.
4.	./Prod_cons_v2.3 {$readfile} #Producer #Consumer -> Execute the producer and consumer version 2.3 program.
5.	./Prod_cons_v3 {$readfile} #Producer #Consumer -> Execute the producer and consumer version 3 program.
6.	./Prod_cons_v4 {$readfile} #Producer #Consumer -> Execute the producer and consumer version 4 program.
<br><br>

# Project1-Simple-Scheduling

## Description
In this project, we will first explain the concepts of process, process scheduling, Inter-process communication (IPC), and CPU scheduling. By applying these concepts, we will explain how we implemented the simple scheduling program and its results for different algorithms, such as first-come-first-served (FCFS), shortest job first (SJF), round-robin (RR), and completely fair safe (CFS). Also, we will show the performance of each algorithm based on the features discussed in a later section. At the end of the paper, we will present the result of the execution of the different scheduling algorithms and compare them.

## Requirements
- Requirements

![image](![image](https://user-images.githubusercontent.com/41320453/209907454-d4d7d5eb-9f6f-4509-ae6a-881c14fbf169.png))

## Simple Scheduling
- Simple Scheduling by Round-Robin (RR) Scheduling Policy

![image](![image](https://user-images.githubusercontent.com/41320453/209907517-be24d657-2098-4bd6-a07c-2482b56b196a.png))
![image](![image](https://user-images.githubusercontent.com/41320453/209907528-6a762b50-810d-407f-bb17-91ec39a2a2fb.png))

- Representation of the Normalized Scheduling Criterions for Different Scheduling Algorithms

![image](![image](https://user-images.githubusercontent.com/41320453/209907666-1fb6d4de-9638-491c-bc15-b926b9518622.png)))

## Environment
- Build Environments:
1. Linux environment – Vi editor, GCC complier
2. Program is built by using the Makefile.

- Make Command:
1.	$make main -> build the execution program for simple scheduler.
2.	$make clean -> clean all the object files that consists of the simple scheduler programs.

- Execution Command
1.	./main 1 {$max_limit} -> Execute the simple scheduler program with the scheduler of FCFS policy with the maximum CPU and I/O burst time of {$max_limit} of the child processes.
2.	./main 2 {$max_limit} -> Execute the simple scheduler program with the scheduler of SJF policy with the maximum CPU and I/O burst time of {$max_limit} of the child processes.
3.	./main 3 {$max_limit} -> Execute the simple scheduler program with the scheduler of RR policy with the maximum CPU and I/O burst time of {$max_limit} of the child processes.
<br><br>

# Project2-Virtual-Memory

## Description
In this project, we will first explain the concepts of main memory, which contains the concepts of the difference between physical and logical memory address, paging, table look-aside buffer (TLB), protection, shared page, hierarchical paging, and swapping. Also, we will explain the concept of virtual memory, which contains the concepts of demand paging, free frame list, performance of demand paging, copy-on-write, and page replacement policies. By applying these concepts, we will explain how we implemented the virtual memory (paging) program and its results for different replacement algorithms, such as random, first-in-first-out (FIFO), least recently used (LRU), least frequently used (LFU), most frequently used (MFU), second chance (SCA), and enhanced second chance (ESCA) algorithms, and sizes of TLB. Also, we will show the performance of each replacement algorithm and the size of the TLB based on the features discussed in a later section. At the end of the paper, we will present the result of the execution of the different replacement algorithms and the size of the TLB and evaluate them.

## Requirements
- Requirements

![image](![image](https://user-images.githubusercontent.com/41320453/209907766-07152d13-1ce6-4918-8f48-8d468437af60.png))

## Virtual Memory
- Result of the virtual memory (paging) program with memory access pattern (from left to right -> Random, FIFO, LRU, LFU, MFU, SCA, ESCA):

![image](![image](https://user-images.githubusercontent.com/41320453/209907826-78679ebb-e734-40ee-a356-86e7a3f9a63e.png))

- Result of the virtual memory (paging) program with TLB (from left to right -> 128, 256, 512):

![image](![image](https://user-images.githubusercontent.com/41320453/209907915-a5fba2ee-487d-46cc-a08f-bdb093401cec.png))

- Generated Memory Access Pattern

![image](![image](https://user-images.githubusercontent.com/41320453/209907955-ff965f74-7939-4ba7-9186-2f6ead635378.png))

- Representation of the Normalized Hardware Access Time on Memory Access Pattern

![image](![image](https://user-images.githubusercontent.com/41320453/209907979-240da80a-eb8b-40a9-8f82-6ef0887f77db.png))

## Environment
- Build Environments:
1. Linux environment – Vi editor, GCC complier
2. Program is built by using the Makefile.

- Make Command:
1.	$make main -> build the execution program for virtual memory (paging).
2.	$make clean -> clean all the object files of the virtual memory (paging) program.

- Execution Command
./main {$scheduler} {$replacement policy} {TLB size} {$max limit} 
1.	scheduler: (1. FCFS, 2. SJF, 3. RR)
2.	replacement policy: (1. RANDOM, 2. FIFO, 3. LRU, 4. LFU, 5. MFU, 6. SCA, 7. ESCA)
3.	TLB size: (128, 256, 512)
4.	max limit: integer
