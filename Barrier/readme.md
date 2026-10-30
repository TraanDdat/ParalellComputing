# C++ Barrier Implementation and Benchmark

## 1. Overview

This project implements and evaluates synchronization barriers in C++ using multithreading.

The project supports custom barrier implementations and the standard C++20 `std::barrier`. It provides configurable test scenarios to evaluate synchronization correctness, shared-data behavior, and performance.

### Objectives

- Implement and test custom synchronization barriers.
- Compare custom barriers with `std::barrier`.
- Test synchronization behavior with different thread counts.
- Investigate data races involving shared variables.
- Benchmark barrier performance under different configurations.
- Use ThreadSanitizer (TSan) to detect data races.

## 2. Requirements

- C++ compiler supporting C++20, such as GCC.
- GNU Make.
- POSIX threads (`pthread`).
- Linux or WSL is recommended.

To check the installed tools:

```bash
g++ --version
make --version
```

## 3. Project Structure

```text
.
├── include/                    # Header files
├── main.cpp                    # Program entry point
├── common.cpp                  # Common utilities for custom barriers
├── common_cp.cpp               # Common utilities for std::barrier
├── thread_design.cpp           # Custom barrier test implementations
├── thread_design_cp.cpp        # std::barrier test implementations
├── Makefile                    # Build and test configuration
├── obj/                        # Compiled object files
└── exe/
    └── mycpp                   # Executable
```

The source files used for compilation depend on the `STD_BARRIER` configuration.

- `STD_BARRIER=0`: Uses `common.cpp` and `thread_design.cpp`.
- `STD_BARRIER=1`: Uses `common_cp.cpp` and `thread_design_cp.cpp`.

## 4. Build and Run

**Note:** Run `make clean` before building with a different configuration.

Following format of build:
> ```bash
> make clean
> make [option]
> make run       # Normal execution
> make run_tsan  # ThreadSanitizer
> ```

### 4.1 Build with the default configuration

```bash
make clean
make
make run
```

Default configuration:

| Parameter | Default value |
|---|---:|
| `VAR` | 1 |                   
| `TEST_PROB` | 1 |             
| `NUM_OF_THREAD` | 3 |         
| `BACKOFF` | 0 |                
| `NUM_OF_BARRIER` | 0 |        
| `THREAD_SLEEP` | 0 |          
| `STD_BARRIER` | 0 |           

### 4.2 Configure the test

Pass parameters to `make` to select the barrier variant, test scenario, thread count, and other options.

```bash
make clean
make VAR=1 TEST_PROB=1 NUM_OF_THREAD=3
make run
```

For example, test a custom barrier with 8 threads:

```bash
make clean
make VAR=1 TEST_PROB=1 NUM_OF_THREAD=8
make run
```

**Important:** Run `make clean` before rebuilding with different configurations to avoid reusing object files compiled with previous settings.

### 4.3 Compare with `std::barrier`

To build and run the standard C++20 barrier implementation:

```bash
make clean
make STD_BARRIER=true NUM_OF_THREAD=3
make run
```

The `STD_BARRIER` option selects the source files used for compilation and is also passed to the program as a compile-time definition.

## 5. Configuration Parameters

| Parameter | Description |
|---|---|
| `VAR` | Selects the custom barrier implementation variant. | 1 to 6
| `TEST_PROB` | Selects the test problem. | 1 to 4
| `NUM_OF_THREAD` | Number of worker threads. |
| `BACKOFF` | Configures the backoff cycle, if supported by the selected implementation. |
| `NUM_OF_BARRIER` | Configures the number of barriers for throughout checking | >0 to enable throughout checking
| `THREAD_SLEEP` | Configures thread sleeping behavior for throughout checking |
| `STD_BARRIER` | Selects the standard `std::barrier` implementation when enabled. |
| `SANITIZE` | Enables an optional sanitizer configuration. |

The supported values and exact behavior of `VAR`, `TEST_PROB`, `BACKOFF`, `NUM_OF_BARRIER`, `NUM_OF_THREAD` and `THREAD_SLEEP` depend on the implementation in the source files.

## 6. Testing & evaluation

The project can be evaluated using three categories of tests.

### 6.1 Barrier Correctness (test_correctness)

Verify that participating threads synchronize correctly at the barrier and do not proceed past a synchronization point before the required participants have arrived.
This test scenarios only execute one cycle.

Command: 
```bash
make clean
make VAR=1 TEST_PROB=1 NUM_OF_THREAD=3 TOTAL_IT=1000 STD_BARRIER=true
make run
```

### 6.2 Scalability checking (test_scalability)

Evaluate how barrier performance and synchronization correctness change as the number of threads increases. Repeat the tests under different configurations, including different backoff settings, and measure the average execution time. Check shared-data consistency and use ThreadSanitizer to investigate potential data races.

```bash
make clean
make VAR=1 TEST_PROB=1 NUM_OF_THREAD=3 TOTAL_IT=1000 STD_BARRIER=true BACKOFF=1
make run
```

### 6.3 Throughout (test_barrier_throughput)

Measure the throughput of the barrier implementation by evaluating the number of barrier synchronization operations completed per unit of time under different configurations, such as varying thread counts and barrier variants. 
This helps assess scalability and compare the performance of custom barriers with std::barrier.

```bash
make clean
make VAR=1 NUM_OF_BARRIER=10000 NUM_OF_THREAD=3 TOTAL_IT=1000 STD_BARRIER=true BACKOFF=0
make run
```

### 6.4 Evalulation

Keep the test conditions consistent when comparing performance. Avoid excessive logging during benchmark runs because I/O can affect thread scheduling and measured execution time.

Measure barrier performance under different configurations, including:
- Different thread counts.
- Different custom barrier variants.
- Custom barriers versus `std::barrier`.
- Different backoff and thread-sleep configurations, where supported.

Test shared variables such as `val` and `val_atm`.
- Check whether the final values match the expected results.
- Investigate concurrent reads and writes to shared data.
- Distinguish incorrect results from data races.
- Remember that a correct barrier does not automatically make every shared-data access race-free.

## 7. Detect Data Races with ThreadSanitizer (ubuntu)

ThreadSanitizer helps identify data races in multithreaded programs.

### 7.1 Build with TSan

```bash
make clean
make SANITIZE=thread VAR=1 TEST_PROB=1 NUM_OF_THREAD=3 TOTAL_IT=1000
make run_tsan
```

### 7.2 Interpret the Results

- **No race reported:** TSan did not detect a data race during that execution. This does not prove the program is race-free.
- **Data race reported:** Inspect the conflicting memory accesses and determine whether they are properly synchronized.
- **Incorrect final result without a TSan report:** Investigate logical errors, synchronization issues, and test assumptions. Not every correctness bug is a data race.

## 8. Clean Build Artifacts

Remove compiled object files and the executable:

```bash
make clean
```

Then rebuild with the desired configuration.

## 9. Notes

- The Makefile uses `-std=c++20`, which is required for the standard `std::barrier` implementation.
- Custom barrier variants must be implemented correctly for their supported thread counts and usage patterns.
- Logging can change thread scheduling and hide timing-sensitive bugs.
- Benchmark results depend on the processor, operating system, compiler, optimization flags, and thread scheduling.
- Use the same compiler flags and test conditions when comparing implementations.