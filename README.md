# Parallel Computing
## 1.Mutex Locks
### Description
- This repository is implementation of three lock mechanism (TAS, TTAS, and MCS) for multi-threaded program using C++.
### Features and Source Path
- TAS Lock : .\src\TASLock.cpp
- TTAS Lock Implementation: .\src\TTASLock.cpp
- MCS Lock Implementation: .\src\MCSLock.cpp
- Benchmark: .\src\benchmark.cpp
- Test Strategy: .\test\test.cpp
- Draw The Plot: .\test\DrawPlot.ipynb
### Instructions
1) Compile and Run the Test Strategy
- Go to main repository
>cd ParalellComputing
- Open cmd on the main repository
- Run the command to compile the Test Strategy
>make buildtest
- Run the command to run the test
>.\build\test_out.exe
2) Compile and Run the Benchmark
- Go to main repository
>cd ParalellComputing
- Open cmd on the main repository
- Run the command to compile the Test Strategy
>make
- Run the command to run the test
>.\build\benchmark.exe
3) To Generate the Plot of Benchmark
- Go to main repository
>cd ParalellComputing
- Open cmd on the main repository
- Run the command to compile the Test Strategy
>make
- Run the command to run the test
>.\benchmark.exe > logs\log.txt
- Open the ".\test\DrawPlot.ipynb"
- Select the Python environment and run the python notebook
### Note
- The running will be took long time approximately 5hours -> 6hours.