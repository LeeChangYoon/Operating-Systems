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

![Simple Shell Requirements](https://user-images.githubusercontent.com/41320453/209908853-318c45d9-c82a-47e3-a3e9-745fe3f93914.png)

## Simple MyShell (SiSH)
- Simple MyShell (SiSH)

![Simple Shell Execution 2](https://user-images.githubusercontent.com/41320453/209908932-2b1f1a31-eb4b-480f-b69b-ef14d7e6d93a.png)

- Example of Command Execution on SiSH

![Simple Shell Execution](https://user-images.githubusercontent.com/41320453/209908936-29d27399-382d-4c9e-8d88-eebcc2c95907.png)

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

![Multi-Threaded Word Count Program Requirements](https://user-images.githubusercontent.com/41320453/209908953-20277f27-651a-4cb6-ad74-5c0ceeefdde3.png)

## Multi-Threaded-Word-Count
- Reading FreeBSD9-orig/ObsoleteFiles.inc with Producer and Consumer Version 3 

![Multi-Threaded Word Count Program Execution](https://user-images.githubusercontent.com/41320453/209908963-6f57105d-36d4-4266-8196-20e13d24a2d3.png)

- Execution Time per Number of Threads on Program Version 2.2 of android.tar

![Multi-Threaded Word Count Program Result](https://user-images.githubusercontent.com/41320453/209908972-768f26cf-7b0a-47de-9449-e0861e7e0f84.png)

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

![Simple Scheduling Requirements](https://user-images.githubusercontent.com/41320453/209908985-4eb5cbea-cd4b-4622-bc80-095159d51f56.png)

## Simple Scheduling
- Simple Scheduling by Round-Robin (RR) Scheduling Policy

![Simple Scheduling Execution 1](https://user-images.githubusercontent.com/41320453/209908990-8936ec00-d2ce-4ea3-8bfa-9fb2bd6d76fb.png)
![Simple Scheduling Execution 2](https://user-images.githubusercontent.com/41320453/209908994-9e83b5d3-84a0-4b61-80ba-930884c3f2d0.png)

- Representation of the Normalized Scheduling Criterions for Different Scheduling Algorithms

![Simple Scheduling Result](https://user-images.githubusercontent.com/41320453/209909004-9542660b-d119-4414-bf30-28a44829dfc6.png)

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

![Virtual Memory (Paging) Requirements](https://user-images.githubusercontent.com/41320453/209909023-aac75b0f-9cde-43e4-a787-7ddb7d0bba69.png)

## Virtual Memory
- Result of the virtual memory (paging) program with memory access pattern (from left to right -> Random, FIFO, LRU, LFU, MFU, SCA, ESCA):

![Virtual Memory (Paging) Execution 1](https://user-images.githubusercontent.com/41320453/209909030-6d238ff0-ebe8-4fa4-8818-72ae0bf37f54.png)

- Result of the virtual memory (paging) program with TLB (from left to right -> 128, 256, 512):

![Virtual Memory (Paging) Result 3](https://user-images.githubusercontent.com/41320453/209909087-e3179662-12d5-4315-a273-5848d6c8bdf2.png)

- Generated Memory Access Pattern

![Virtual Memory (Paging) Result 1](https://user-images.githubusercontent.com/41320453/209909092-e21bb4a1-d2fc-4d6c-ae65-5cb03c3005b1.png)

- Representation of the Normalized Hardware Access Time on Memory Access Pattern

![Virtual Memory (Paging) Result 2](https://user-images.githubusercontent.com/41320453/209909100-987b08a8-5487-445c-9564-ceb226e37f6a.png)

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
<br><br>

# Project3-Simple-File-System

## Description
In this project, you will build a simple filesystem. You have to implement above-mentioned simple file system with the proper assumptions.

Since this program assignment could be one of your major take-outs from this course, so please work hard to complete the job/semester. If you need help, please ask for the help. (I am here for that specific purpose.) I, of course, welcome any questions on the subject. Note for the one strict rule that you should not copy the code from any others. Deep discussion on the subject is okay (and encouraged), but the same code (or semantics) will result in the sadending.

Extra implementation/analysis is highly encouraged. For example, you can implement user process that requests of reading for some specific file, instead of doing simple I/O. In addition, you can implement file with write operation that requires storage block allocationfor additional data, requiring i-node change. You may want to load directory entry into memory because every file access implies addition directory file access; and it will be redundant if you read data from storage at every directory file access, which would be one of slowest operations in a computer system. You may want to efficiently search directory entries because a directory may include many files. So, you would use a hash map to easily get the inodefrom the requested file name.

There are various topics related to the file systems, including I/O, scheduling, and virtual memory. Do as much as you can do, and learn from it. (if you need a specific guideline, I can give some.)

## Requirements
- Requirements
1. Shell 에서 disk image 를 mount, unmount 할 수 있도록 할 것.
2. Mount 시, root file system 을 읽어들이도록 할 것
3. Mount 이후, 아래의 연산을 지원할 것
A. ls – Directory 의 file 목록을 출력
B. cat – file 의 내용을 출력
C. pwd – 현재 directory 위치를 absolute directory 로 출력
4. 위의 구현을 file system 의 동작 방식에 맞게 구현할 것
5. File system 의 mount 와 unmount, 명령 실행 등에 있어 failure 가 없도록 구현할 것


## Simple File System
- Shell 실행과 mount. Mount 시의 host 는 현재 volume 의 이름으로 하고, 다른 색깔로 표현한다.

![image](https://user-images.githubusercontent.com/41320453/209910783-ff7a4af7-9584-4f3d-927a-b6fd602a6164.png)

- cat, ls, pwd 실행

![image](https://user-images.githubusercontent.com/41320453/209910795-6a8358b4-b907-45b5-83db-a27d0cfdf02a.png)

- unmount 실행 및 unmount 시의 정상 shell 동작

![image](https://user-images.githubusercontent.com/41320453/209910804-40c71b70-5cdd-4a90-a774-8d1f15443489.png)

## Environment
1. https://github.com/mobile-os-dku-cis-mse/2022-os-proj3 을 clone 받는다.
2. 32181928 branch 로 checkout 한다.
3. make all 을 입력하여 build 한 후 ./main 을 실행하여 shell 을 실행한다.
4. Disk Image 가 있는 directory 로 이동 후, mount {disk image name} 명령을 입력하여 mount 한다
5. ls, cat {file name}, pwd, unmount 등을 사용하여 테스트한다.
