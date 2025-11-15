# Linux Process Scheduler Simulation
A simulation of a Linux Completely Fair Scheduler (CFS) implemented using C++ multithreading, condition variables, and a simplified run queue structure.
It models how an OS schedules processes across multiple CPU cores, handles interrupts, tracks runtime statistics, and manages fair execution.

Process Scheduler is a C++ based simulation of a CPU scheduler supporting:
- Priority Based scheduling
- Interrupt handling
- Virtual runtime tracking
- Multi-threaded process management
- Real-time logging and metrics generation

##  Design and thought Process
- Using multi threads to simulate multiple core of a cpu. ie one process runs on 1 thread simulating it running on 1 core
- Using signal handling to generate interrupts anytime to test how it is handled
- Implemented CFS Algo of choosing process with the least vruntime
- Considered two different type of priority process CPU Bound and I/O Bound

##  Build & Run

### 1. Clone the repository
git clone https://github.com/johnitkujur/Process_Scheduler.git

### 2. Compile
Use make or compilation command - g++ main.cpp process.cpp interrupt.cpp time_utility.cpp -o scheduler -pthread

### 3. Considerations before running
- To Add/Remove process edit this structure in main.cpp
```cpp
vector<proc>p_list = 
        {       //pid   arrival_time    completion_time         priority        cpu_time        vruntime        total_time_left
                {1,     0,              0,                      0,              0,              0,              7},
                {2,     400,            0,                      1,              0,              0,              3},
                {3,     3000,           0,                      1,              0,              0,              10},
                {4,     500,            0,                      1,              0,              0,              15},
                {5,     100,            0,                      0,              0,              0,              1},
                {6,     0,              0,                      0,              0,              0,              5},
                {7,     200,            0,                      1,              0,              0,              6},
                {8,     0,              0,                      0,              0,              0,              3}
        };
```
- For Windows interrupts are simulated via timer edit this function to add/remove
```cpp
add_interrupt_test
```
- For Linux System you can change below macro to zero(in main.cpp) this will enable you to simulate interrupts by pressing (ctrl + Z)
- Innterrupts are simulated for linux via signal handling
```cpp
//For Windows set this to 1
#define WINDOWS_SYSTEM 1
```
### 4. Run
./scheduler

## Snapshots
<img width="400" height="400" alt="image" src="https://github.com/user-attachments/assets/6790ddbe-5691-4123-aa3b-2b5b1901aac0" /> <img width="400" height="400" alt="image" src="https://github.com/user-attachments/assets/6a486f50-3950-41bd-8186-69d3a2dcf539" /> <img width="400" height="400" alt="image" src="https://github.com/user-attachments/assets/9ca8cd47-ba68-4700-b1a0-83947b48a617" />

## Reference

- https://singhdevhub.bearblog.dev/dissecting-linux-schedulers-implementing-our-toy-cfs_scheduler-simulation/
