Perfect! Here's the complete comprehensive threads course. You can copy this entire content and save it as a file on your computer:

```markdown
# Complete Threads Learning Course
## A Comprehensive Guide to Understanding Operating System Threads

---

## Table of Contents
1. [Module 1: Fundamentals](#module-1-fundamentals)
2. [Module 2: Thread Architecture](#module-2-thread-architecture)
3. [Module 3: Thread Lifecycle & States](#module-3-thread-lifecycle--states)
4. [Module 4: Scheduling & Context Switching](#module-4-scheduling--context-switching)
5. [Module 5: Synchronization Problems](#module-5-synchronization-problems)
6. [Module 6: Synchronization Solutions](#module-6-synchronization-solutions)
7. [Module 7: Thread Models](#module-7-thread-models)
8. [Module 8: Common Threading Issues](#module-8-common-threading-issues)
9. [Module 9: Best Practices](#module-9-best-practices)

---

# Module 1: Fundamentals

## 1.1 What is a Process?

A **process** is a running program with:
- **Own memory space** - isolated from other processes
- **Own code** - executable instructions
- **Own data** - variables and resources
- **Process ID (PID)** - unique identifier

**Key characteristic**: If one process crashes, it doesn't affect other processes (isolation).

### Process Creation is Expensive
- Takes significant time to allocate memory
- Takes significant time to load code
- Inter-process communication (IPC) is slow and complex
- Each process needs its own resources (file descriptors, network connections, etc.)

---

## 1.2 Why Processes Alone Are Not Enough

Imagine a web browser that needs to:
- Download a file
- Update the UI display
- Respond to user clicks
- Fetch email in the background

**ALL AT THE SAME TIME!**

If you use separate processes:
```
Process 1: Downloads file (blocked, waiting for network)
Process 2: Handles UI (blocked, waiting for user input)
Process 3: Checks email (blocked, waiting for server)
```

**Problems**:
- Creating new processes is slow and resource-heavy
- Communication between processes is complicated
- Context switching between processes is expensive
- You'd need many processes for all concurrent tasks

**Solution**: Use **threads** instead!

---

## 1.3 What is a Thread?

A **thread** is:
- **An independent sequence of execution** that lives *inside* a process
- **A lightweight unit of concurrency** - much cheaper to create than a process
- **A worker** that can run independently while other threads work on different tasks

### The Factory Analogy (Revised)

```
PROCESS = Factory
├── Shared: Building, machines, materials, blueprints
└── THREADS = Workers inside the factory
    ├── Worker 1: Operating machine A
    ├── Worker 2: Operating machine B
    ├── Worker 3: Packing products
    └── All workers share the same tools and materials
```

### Multiple Threads in One Process

```
One Process
z├── One heap memory space
├── One code section
├── One file descriptor table
└── Multiple Threads
    ├── Thread 1 (Main thread)
    ├── Thread 2
    ├── Thread 3
    └── Thread N
    
Each thread has its own:
- Stack (local variables)
- CPU registers
- Program counter
```

---

## 1.4 Why Threads Are Better

### 1. **Lightweight Creation**
- Creating a thread takes microseconds
- Creating a process takes milliseconds
- **1000x faster!**

### 2. **Cheap Communication**
- Threads share memory → just read/write shared variables
- No need for pipes, sockets, or message queues
- No serialization/deserialization overhead

### 3. **Responsiveness**
- Download file in thread 1
- Handle UI in thread 2
- Check email in thread 3
- All happen concurrently!

### 4. **Efficient Resource Usage**
- Threads within the same process share:
  - Code section
  - Heap memory
  - Open files
  - Network connections

### 5. **Better Performance**
- Heavy tasks split across multiple threads
- Reduced execution time (parallel execution)
- Better CPU utilization

### 6. **Shared Data (Without IPC Overhead)**
- Cheapest form of inter-task communication
- Just pass pointers/references to shared data
- No expensive copying or serialization

---

## 1.5 The Thread Trade-off: Speed vs Safety

**Fast, Efficient** ✅
- Shared memory is super efficient
- No copying, no serialization
- Threads can communicate instantly

**BUT Also Tricky** ⚠️
- When two threads modify the same data simultaneously
- **Race conditions** occur (we'll cover this in detail later)
- Harder to debug than single-threaded code
- Requires careful synchronization

---

# Module 2: Thread Architecture

## 2.1 What Threads Share Within a Process

All threads in the same process share:

### **Shared Memory (Heap)**
```
Process Memory Layout:
┌─────────────────────────┐
│  Code Section           │ ← All threads execute same code
│  (Read-only)            │
├─────────────────────────┤
│  Data Section           │ ← Global variables (shared)
│  (Global variables)     │
├─────────────────────────┤
│  Heap                   │ ← Dynamically allocated memory
│  (malloc'd memory)      │ ← ALL threads share this
├─────────────────────────┤
│  Stack 1 (Thread 1)     │ ← Local variables for Thread 1
├─────────────────────────┤
│  Stack 2 (Thread 2)     │ ← Local variables for Thread 2
├─────────────────────────┤
│  Stack N (Thread N)     │ ← Local variables for Thread N
└─────────────────────────┘
```

### **Global Variables**
- Stored in data section
- Visible to all threads
- **Can be modified by any thread** (danger zone!)

### **File Descriptors**
- If thread 1 opens a file, all threads can read from it
- Threads share the same file offset

### **Process-level Resources**
- Network connections
- Process ID
- Signal handlers
- Current working directory

---

## 2.2 What Each Thread Owns Individually

Each thread has its own:

### **Stack**
```
Thread 1 Stack:
┌──────────────────────┐
│ Local variables      │
│ - int x = 5          │
│ - char* str = "hi"   │
│ - double arr[10]     │
├──────────────────────┤
│ Function call stack  │
│ (return addresses)   │
└──────────────────────┘

Thread 2 Stack:
┌──────────────────────┐
│ Local variables      │
│ - int y = 10         │
│ - char* ptr = ...    │
├──────────────────────┤
│ Function call stack  │
└──────────────────────┘
```

**Key Point**: Local variables in one thread are invisible to other threads (each has their own stack)

### **CPU Registers**
- Each thread has its own set of CPU registers
- When scheduler switches threads, it saves/restores registers
- This is called **context switching**

### **Program Counter (PC)**
- Points to the next instruction to execute
- Different for each thread
- Thread 1 might be at instruction 1000
- Thread 2 might be at instruction 5000

### **Thread ID**
- Unique identifier within the process (pthread_t in POSIX)
- Scheduler uses this to track threads

---

## 2.3 Shared Memory Example

```c
// Global variable - SHARED by all threads
int shared_counter = 0;

void* thread_function(void* arg) {
    int thread_id = (int)arg;
    
    // Local variable - NOT shared
    int local_var = 100;
    
    // Modify shared data
    shared_counter++;  // All threads can access this!
    
    printf("Thread %d: local_var=%d, shared=%d\n", 
           thread_id, local_var, shared_counter);
    
    return NULL;
}
```

**What happens when you run this with 3 threads?**

```
Expected: shared_counter = 3
But might see: shared_counter = 1 or 2 (race condition!)

Why? Because:
Thread 1: Read shared_counter (0), increment, write (1)
Thread 2: Read shared_counter (0), increment, write (1)  ← Problem!
Thread 3: Read shared_counter (1), increment, write (2)
```

This is a **race condition** - we'll fix this in Module 6.

---

# Module 3: Thread Lifecycle & States

## 3.1 Thread States Diagram

```
         create()
            ↓
    ┌─────────────┐
    │     NEW     │ Thread created but not started yet
    └─────────────┘
            ↓
         start()
            ↓
    ┌─────────────────┐
    │   RUNNABLE      │ Ready to run, waiting for CPU
    │ (Ready Queue)   │
    └─────────────────┘
         ↑    ↓
    (scheduler       (scheduler
     grants CPU)    takes CPU)
         ↑    ↓
    ┌─────────────────┐
    │   RUNNING       │ Currently executing on CPU
    └─────────────────┘
         ↑    ↓
         │    │
         │    ├──(wait for lock)──→ ┌──────────────┐
         │    │                     │   BLOCKED    │
         │    │                     │  (Waiting)   │
         │    │    (lock acquired)  └──────────────┘
         │    ├──←──←──←──←──←──←──←──┘
         │    │
         │    ├──(I/O request)──→ ┌──────────────┐
         │    │                   │  WAITING     │
         │    │  (I/O complete)   │ (for I/O)    │
         │    └──←──←──←──←──←──←──┘ └──────────────┘
         │    │
         │    ├──(sleep expires)──→ Waiting → Runnable
         │    │
         │    └──→ ┌──────────────────┐
         └────────│  TERMINATED      │
                  │ (Finished)       │
                  └──────────────────┘
```

---

## 3.2 Detailed State Descriptions

### **NEW State**
- Thread object created
- Not yet added to scheduler
- Example:
  ```c
  pthread_t tid;
  // Thread is in NEW state after creation but before start
  ```

### **RUNNABLE State**
- Thread is ready to execute
- Waiting in the ready queue for CPU time
- Scheduler will pick it when CPU becomes available
- Thread keeps looping in this state waiting for turn

### **RUNNING State**
- Thread has acquired CPU
- Currently executing its instructions
- Only one thread per CPU core can be running at a time
- Stays here until:
  - Time slice expires (preempted)
  - Waits for something (I/O, lock, etc.)
  - Terminates

### **BLOCKED State**
- Thread wants to run but CAN'T
- Waiting for a resource it doesn't have
- **Causes**:
  - Trying to acquire a lock that another thread holds
  - Waiting for I/O to complete
  - Waiting for another thread to notify it
- When resource becomes available → back to RUNNABLE

### **WAITING State**
- Waiting for a specific event
- Thread explicitly puts itself in this state
- Examples:
  - `wait()` - waiting for notify from another thread
  - `sleep()` - waiting for time to pass
  - Waiting for a condition to become true
- When event happens → back to RUNNABLE

### **TERMINATED State**
- Thread has finished execution
- Run method returned
- Thread no longer exists
- Resources are cleaned up
- **Final state** - cannot go to any other state

---

## 3.3 State Transitions in Code

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* worker(void* arg) {
    printf("Thread: Running state\n");
    sleep(2);  // Transition to WAITING state
    printf("Thread: Back to Running\n");
    return NULL;  // Transition to TERMINATED
}

int main() {
    pthread_t tid;
    
    // Thread state: NEW
    pthread_create(&tid, NULL, worker, NULL);
    
    // Thread state: RUNNABLE (waiting for CPU)
    sleep(1);
    
    // Thread state: RUNNING (executing worker function)
    // Then: WAITING (during sleep)
    // Then: RUNNABLE (sleep finished)
    // Then: RUNNING (continues execution)
    // Then: TERMINATED (function returns)
    
    pthread_join(tid, NULL);  // Wait for thread to terminate
    printf("Main: Thread finished\n");
    
    return 0;
}
```

---

## 3.4 State Transition Reasons

| Transition | Reason | Code Example |
|-----------|--------|--------------|
| NEW → RUNNABLE | Thread starts | `pthread_create()` |
| RUNNABLE → RUNNING | Scheduler picks thread | Automatic (OS) |
| RUNNING → RUNNABLE | Time slice expires | Automatic (preemption) |
| RUNNING → BLOCKED | Acquire lock | `pthread_mutex_lock()` |
| BLOCKED → RUNNABLE | Lock released | When other thread unlocks |
| RUNNING → WAITING | I/O request | `read()`, `write()` |
| RUNNING → WAITING | Explicit wait | `sleep()`, `wait()` |
| WAITING → RUNNABLE | Event occurs | I/O complete, notify |
| RUNNING → TERMINATED | Function returns | `return` statement |
| RUNNING → TERMINATED | Exit called | `pthread_exit()` |

---

# Module 4: Scheduling & Context Switching

## 4.1 What is Scheduling?

**Scheduling** is the process of deciding:
- Which thread runs on the CPU next?
- How long does each thread get to run?
- When to switch threads?

### Scheduler Components

```
OS Kernel
├── Ready Queue
│   ├── Thread 1 (waiting)
│   ├── Thread 2 (waiting)
│   ├── Thread 3 (waiting)
│   └── Thread N (waiting)
├── Blocked Queue
│   ├── Thread A (waiting for I/O)
│   ├── Thread B (waiting for lock)
│   └── ...
└── Scheduler Algorithm
    (Picks next thread to run)
```

---

## 4.2 Context Switching

**Context Switch** = Saving one thread's state and loading another thread's state

### What Gets Saved? (Thread Context)

```
When switching FROM Thread 1 TO Thread 2:

SAVE Thread 1's Context:
├── Program Counter (PC) → 0x4050C2A0
├── Stack Pointer (SP) → 0x7FFF0000
├── All CPU Registers
│   ├── RAX = 0x12345678
│   ├── RBX = 0x87654321
│   ├── RCX = 0xDEADBEEF
│   └── ... (all registers)
├── Stack pointer
└── Status flags

LOAD Thread 2's Context:
├── Program Counter (PC) ← 0x5020D001
├── Stack Pointer (SP) ← 0x7FFF2000
├── All CPU Registers
│   ├── RAX ← 0xAAAAAAAA
│   ├── RBX ← 0xBBBBBBBB
│   └── ... (all registers)
└── Status flags
```

### Why Context Switching is Expensive

```
Cost per context switch (approximate):
├── Save registers: ~50-100 CPU cycles
├── Load registers: ~50-100 CPU cycles
├── Flush CPU cache: ~1000-5000 cycles
├── Invalidate TLB (translation lookaside buffer): ~1000 cycles
└── Total: ~2000-6000 CPU cycles

On a 3GHz CPU:
└── 3000 cycles ≈ 1 microsecond
```

**Example**: If you have 1000 context switches per second:
- 1000 microseconds = 1 millisecond per second lost to switching!
- That's 0.1% CPU wasted on just switching

---

## 4.3 Scheduling Algorithms

### **Round Robin (Time Slicing)**

Each thread gets equal CPU time:

```
Time Slice = 10ms (time quantum)

Time: 0ms    └─ Thread 1 runs (0-10ms)
Time: 10ms   └─ Context Switch
Time: 10ms   └─ Thread 2 runs (10-20ms)
Time: 20ms   └─ Context Switch
Time: 20ms   └─ Thread 3 runs (20-30ms)
Time: 30ms   └─ Context Switch
Time: 30ms   └─ Thread 1 runs again (30-40ms)
```

**Advantages**:
- Fair - all threads get equal time
- Responsive - no thread starves
- Predictable

**Disadvantages**:
- Frequent context switches (overhead)
- Not optimal for different thread priorities

---

### **Priority Scheduling**

Threads have priorities:

```
Priority Queue:
├── High Priority: Thread 1, Thread 3
├── Medium Priority: Thread 2
└── Low Priority: Thread 4, Thread 5

Scheduler picks from highest priority queue first.
Within same priority, use Round Robin.
```

**Example**:
```
Time: 0ms   └─ Thread 1 (High) runs
Time: 10ms  └─ Context Switch (High priority exhausted)
Time: 10ms  └─ Thread 3 (High) runs
Time: 20ms  └─ Context Switch (Back to High)
Time: 20ms  └─ Thread 1 (High) runs
Time: 30ms  └─ Context Switch (No more High priority ready)
Time: 30ms  └─ Thread 2 (Medium) runs
```

**Problem**: Low-priority threads might never run (starvation)

---

### **Preemptive vs Non-Preemptive Scheduling**

**Preemptive** (Modern OS - Linux, Windows):
- OS can interrupt a running thread
- Takes CPU away even if thread isn't done
- Scheduler forces context switch at time slice

```c
// Thread 1 executing:
for(int i = 0; i < 1000000; i++) {
    printf("Processing %d\n", i);  // Might get interrupted here
}
// Scheduler can force context switch HERE even though loop is running
```

**Non-Preemptive** (Older, some embedded):
- Thread keeps running until it yields
- Thread must voluntarily give up CPU
- Less overhead but less fair

```c
// Thread 1 executing:
for(int i = 0; i < 1000000; i++) {
    if(i % 100 == 0) pthread_yield();  // Manually yield
}
```

---

## 4.4 Multiprocessor Scheduling

On multicore CPUs, multiple threads actually run simultaneously:

```
CPU with 4 cores:

Core 1: │ Thread 1 │ Thread 5 │ Thread 1 │ Thread 5 │
Core 2: │ Thread 2 │ Thread 2 │ Thread 6 │ Thread 2 │
Core 3: │ Thread 3 │ Thread 3 │ Thread 3 │ Thread 3 │
Core 4: │ Thread 4 │ Thread 7 │ Thread 8 │ Thread 7 │
        └─────────┴─────────┴─────────┴─────────┘
          Time 0    Time 10    Time 20    Time 30
```

**Key Point**: 
- On 1 core: threads appear concurrent (interleaved)
- On 4 cores: 4 threads are ACTUALLY parallel
- OS scheduler tries to keep all cores busy

---

# Module 5: Synchronization Problems

## 5.1 Race Condition Detailed Example

### The Problem

```c
// Global variable shared by all threads
int counter = 0;

void* increment_thread(void* arg) {
    for(int i = 0; i < 100; i++) {
        counter++;  // THIS IS THE PROBLEM
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    // Create 2 threads
    pthread_create(&t1, NULL, increment_thread, NULL);
    pthread_create(&t2, NULL, increment_thread, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Counter = %d\n", counter);
    // Expected: 200
    // Actual: Could be 100, 150, 176, etc. (WRONG!)
    
    return 0;
}
```

### Why This Fails (Detailed Analysis)

The statement `counter++` is NOT atomic. It breaks down into:

```
assembly code for counter++:

1. LOAD counter into register  (MOV RAX, [counter_address])
2. ADD 1 to register            (ADD RAX, 1)
3. STORE register back          (MOV [counter_address], RAX)
```

### The Race Condition Timeline

```
Time | Thread 1 Action          | Thread 2 Action         | Counter Value
-----|--------------------------|-------------------------|---------------
0    | LOAD counter (0) → RAX1  |                         | 0
1    | ADD 1 → RAX1 (=1)        | LOAD counter (0) → RAX2 | 0
2    |                          | ADD 1 → RAX2 (=1)       | 0
3    | STORE RAX1 to counter    |                         | 1 (from Thread 1)
4    |                          | STORE RAX2 to counter   | 1 (from Thread 2)
     |                          |                         |
     | Expected: 2              | ACTUAL: 1  (LOST UPDATE!)
```

**What happened**: 
- Thread 1 incremented counter to 1
- Thread 2 read the OLD value (0), incremented to 1, and wrote it back
- Thread 1's increment was LOST

### Why This Happens

The problem is **interleaving of non-atomic operations**:

```
Operation A: counter = 0 → 1  (3 CPU instructions)
Operation B: counter = 0 → 1  (3 CPU instructions)

If operations are NOT protected:
- Thread 1 executes Instruction A1 (LOAD)
- Thread 2 executes Instruction B1 (LOAD) ← Problem!
- Thread 1 executes Instruction A2 (ADD)
- Thread 2 executes Instruction B2 (ADD)
- Thread 1 executes Instruction A3 (STORE)
- Thread 2 executes Instruction B3 (STORE)

Result: Lost update because B1 happened after A1 but before A3
```

---

## 5.2 Critical Section

A **Critical Section** is a part of code that:
- Accesses shared data
- Should only be executed by ONE thread at a time
- If multiple threads execute simultaneously → race condition

```c
void* worker(void* arg) {
    // Non-critical section (safe to run in parallel)
    int local_x = calculate_something();
    
    // ===== CRITICAL SECTION STARTS =====
    counter++;  // Only one thread should do this at a time
    printf("Value: %d\n", counter);  // Reading shared variable
    // ===== CRITICAL SECTION ENDS =====
    
    // Non-critical section (safe to run in parallel)
    printf("Local value: %d\n", local_x);
    
    return NULL;
}
```

### Why We Need Synchronization

```
Without Protection:
┌─────────────────────────────────────────────┐
│ Thread 1                                    │
│ ┌────────────────────────────────┐          │
│ │ ENTER Critical Section         │          │
│ │ temp = counter; (reads 0)      │          │
│ └────────────────────────────────┘          │
│                                             │
│ (gets interrupted here)                     │
└─────────────────────────────────────────────┘
        ↓ CONTEXT SWITCH
┌─────────────────────────────────────────────┐
│ Thread 2                                    │
│ ┌────────────────────────────────┐          │
│ │ ENTER Critical Section  ✗      │          │
│ │ temp = counter; (reads 0)      │
│ │ counter = temp + 1; (writes 1) │ ← BAD!
│ │ EXIT Critical Section          │          │
│ └────────────────────────────────┘          │
└─────────────────────────────────────────────┘
        ↓ CONTEXT SWITCH
┌─────────────────────────────────────────────┐
│ Thread 1 (resumes)                          │
│ ┌────────────────────────────────┐          │
│ │ counter = temp + 1; (writes 1) │ ← BAD!
│ │ EXIT Critical Section          │          │
│ └────────────────────────────────┘          │
└─────────────────────────────────────────────┘
```

---

## 5.3 Deadlock - Detailed Explanation

**Deadlock** = Two or more threads waiting for each other forever, neither can proceed.

### Classic Deadlock Scenario: Dining Philosophers

```
Think of threads as philosophers sitting at a table:

    Philosopher 1
          ↓
    ┌─ Fork 1 ─┐
    │           │
    ├ 🍽️ TABLE 🍽️ ┤
    │           │
    └─ Fork 2 ─┘
          ↓
    Philosopher 2

Each philosopher needs TWO forks to eat (one on left, one on right).
```

**Scenario**:
```
Time 1:
└─ Philosopher 1 grabs Fork 1

Time 2:
└─ Philosopher 2 grabs Fork 2

Time 3:
├─ Philosopher 1 waits for Fork 2 (held by Philosopher 2)
└─ Philosopher 2 waits for Fork 1 (held by Philosopher 1)

Result: DEADLOCK! Both wait forever.
```

### Code Example of Deadlock

```c
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void* thread1_func(void* arg) {
    pthread_mutex_lock(&lock1);  // Grab lock 1
    printf("Thread 1: Acquired lock1\n");
    sleep(1);  // Give thread 2 time to grab lock2
    
    printf("Thread 1: Waiting for lock2...\n");
    pthread_mutex_lock(&lock2);  // Try to grab lock 2 (BLOCKED)
    printf("Thread 1: Acquired lock2\n");
    
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
    return NULL;
}

void* thread2_func(void* arg) {
    pthread_mutex_lock(&lock2);  // Grab lock 2
    printf("Thread 2: Acquired lock2\n");
    sleep(1);
    
    printf("Thread 2: Waiting for lock1...\n");
    pthread_mutex_lock(&lock1);  // Try to grab lock 1 (BLOCKED)
    printf("Thread 2: Acquired lock1\n");
    
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1_func, NULL);
    pthread_create(&t2, NULL, thread2_func, NULL);
    
    pthread_join(t1, NULL);  // Will wait forever (DEADLOCK)
    pthread_join(t2, NULL);
    
    return 0;
}
```

### Timeline of the Deadlock

```
Time:  Thread 1                    Thread 2                  State
────────────────────────────────────────────────────────────────
0      lock(lock1) ✓               -                         lock1 held by T1
1      sleep(1)                    lock(lock2) ✓             lock2 held by T2
2      -                           sleep(1)                  Both sleeping
3      sleep done                  sleep done                Both done sleeping
4      lock(lock2) ← BLOCKED       lock(lock1) ← BLOCKED     DEADLOCK!
       (waiting for T2)            (waiting for T1)          Neither can proceed
```

### Conditions for Deadlock (All 4 Must Be True)

1. **Mutual Exclusion**: Resources can only be used by one thread
2. **Hold and Wait**: Thread holds a resource while waiting for another
3. **No Preemption**: Cannot forcefully take a resource from a thread
4. **Circular Wait**: Thread A waits for resource held by B, B waits for resource held by A

---

## 5.4 Starvation

**Starvation** = A thread never gets CPU time (different from deadlock).

```c
// Starvation example with priority:

void* low_priority_task(void* arg) {
    for(int i = 0; i < 1000000; i++) {
        printf("Low priority: %d\n", i);  // Might never print!
    }
    return NULL;
}

void* high_priority_task(void* arg) {
    while(1) {
        printf("High priority: Still running\n");  // Keeps running
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    // Create high priority thread first (keeps running)
    pthread_create(&t2, NULL, high_priority_task, NULL);
    
    // Create low priority thread
    pthread_create(&t1, NULL, low_priority_task, NULL);
    
    pthread_join(t1, NULL);  // Will wait forever (t1 never runs!)
    pthread_join(t2, NULL);
    
    return 0;
}
```

**Timeline**:
```
Time:  CPU Activity
────────────────────────────────
0-100  High priority thread runs continuously
100+   Low priority thread NEVER gets CPU time
       (still in ready queue, but scheduler keeps picking high priority)
```

---

## 5.5 Livelock

**Livelock** = Threads keep running but make no progress (stuck in a loop of checking/backing off).

```c
// Livelock example:
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void* thread1_func(void* arg) {
    while(1) {
        pthread_mutex_lock(&lock1);
        if(pthread_mutex_trylock(&lock2) == 0) {  // Try to get lock2
            // Successfully got both locks
            printf("Thread 1: Proceeding\n");
            pthread_mutex_unlock(&lock2);
            pthread_mutex_unlock(&lock1);
            break;
        } else {
            // Lock2 is busy, release lock1 and try again
            printf("Thread 1: Backing off, will retry\n");
            pthread_mutex_unlock(&lock1);
            // Both threads keep retrying forever!
        }
    }
    return NULL;
}

void* thread2_func(void* arg) {
    while(1) {
        pthread_mutex_lock(&lock2);
        if(pthread_mutex_trylock(&lock1) == 0) {  // Try to get lock1
            // Successfully got both locks
            printf("Thread 2: Proceeding\n");
            pthread_mutex_unlock(&lock1);
            pthread_mutex_unlock(&lock2);
            break;
        } else {
            // Lock1 is busy, release lock2 and try again
            printf("Thread 2: Backing off, will retry\n");
            pthread_mutex_unlock(&lock2);
            // Both threads keep retrying forever!
        }
    }
    return NULL;
}
```

**Timeline**:
```
Time:  Thread 1 Action              Thread 2 Action          CPU Usage
────────────────────────────────────────────────────────────────────
0      lock(lock1) ✓                -                        CPU busy
1      trylock(lock2) ✗             lock(lock2) ✓             CPU busy
2      unlock(lock1)                trylock(lock1) ✗          CPU busy
3      lock(lock1) ✓                unlock(lock2)            CPU busy
4      (repeats)                    (repeats)                CPU BUSY but no progress!
```

**Key Difference from Deadlock**:
- Deadlock: Threads blocked, CPU idle
- Livelock: Threads running, CPU busy, but no progress

---

# Module 6: Synchronization Solutions

## 6.1 Mutex (Mutual Exclusion Lock)

A **Mutex** is a lock that ensures only one thread can access a critical section at a time.

### How Mutex Works

```
Mutex State: UNLOCKED

Thread 1: lock(&mutex)  ✓ (got it, mutex → LOCKED)
Thread 2: lock(&mutex)  ✗ (blocked, waiting)
Thread 3: lock(&mutex)  ✗ (blocked, waiting)

Thread 1: [using shared resource]

Thread 1: unlock(&mutex)  (mutex → UNLOCKED)
Thread 2: lock(&mutex)   ✓ (got it, mutex → LOCKED)
Thread 3: lock(&mutex)   ✗ (blocked, waiting)
```

### Mutex Code Example

```c
#include <pthread.h>
#include <stdio.h>

// Shared resource
int counter = 0;

// Mutex to protect counter
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

void* increment_thread(void* arg) {
    for(int i = 0; i < 1000; i++) {
        // ENTER CRITICAL SECTION
        pthread_mutex_lock(&counter_lock);    // Acquire lock
        
        counter++;  // NOW SAFE - only one thread here
        
        pthread_mutex_unlock(&counter_lock);  // Release lock
        // EXIT CRITICAL SECTION
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, increment_thread, NULL);
    pthread_create(&t2, NULL, increment_thread, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Counter = %d\n", counter);  // Always 2000 (correct!)
    
    // Clean up mutex
    pthread_mutex_destroy(&counter_lock);
    
    return 0;
}
```

### Mutex Timeline (Corrected)

```
Time | Thread 1                  | Thread 2                  | Counter
-----|---------------------------|---------------------------|--------
0    | lock(&mutex) ✓            |                           | 0
1    | LOAD counter (0) → RAX1   |                           | 0
2    | ADD 1 → RAX1 (=1)         |                           | 0
3    | STORE RAX1 to counter     |                           | 1
4    | unlock(&mutex)            | lock(&mutex) ✓            | 1
5    |                           | LOAD counter (1) → RAX2   | 1
6    |                           | ADD 1 → RAX2 (=2)         | 1
7    |                           | STORE RAX2 to counter     | 2
8    |                           | unlock(&mutex)            | 2
```

**Result**: counter = 2 ✓ (CORRECT!)

### Mutex Operations

```
1. pthread_mutex_init()
   - Initialize a mutex
   - Can use PTHREAD_MUTEX_INITIALIZER for static initialization

2. pthread_mutex_lock()
   - Acquire the lock
   - If locked by another thread, BLOCK until available
   - Thread becomes BLOCKED state

3. pthread_mutex_trylock()
   - Try to acquire the lock
   - Returns immediately (doesn't block)
   - Returns 0 if successful, non-zero if already locked

4. pthread_mutex_unlock()
   - Release the lock
   - Wake up one waiting thread
   - That thread transitions from BLOCKED to RUNNABLE

5. pthread_mutex_destroy()
   - Clean up mutex resources
```

---

## 6.2 Semaphore

A **Semaphore** is a synchronization primitive with a counter. It's more flexible than mutex.

### Types of Semaphores

#### **Binary Semaphore** (like mutex)
```
Counter: 0 or 1

sem_wait(): counter--  (if 0, BLOCK)
sem_post(): counter++  (wake one waiting thread)
```

#### **Counting Semaphore**
```
Counter: 0, 1, 2, 3, ...

Example: Limit resource access to 3 threads

Initialize: sem_init(&sem, 0, 3);  // counter = 3

Thread 1: sem_wait() → counter = 2 (uses resource 1)
Thread 2: sem_wait() → counter = 1 (uses resource 2)
Thread 3: sem_wait() → counter = 0 (uses resource 3)
Thread 4: sem_wait() → BLOCKED (no resources left)
Thread 5: sem_wait() → BLOCKED

Thread 1: sem_post() → counter = 1, Thread 4 wakes up
Thread 4: sem_wait() → counter = 0 (uses resource 1)
```

### Semaphore Code Example

```c
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

// Binary semaphore (counter = 1)
// Like a mutex, but more general
sem_t binary_sem;

// Counting semaphore
// Allows up to 3 threads to access a resource
sem_t resource_sem;

void* counting_semaphore_example(void* arg) {
    int thread_id = (int)arg;
    
    printf("Thread %d: Waiting for resource\n", thread_id);
    sem_wait(&resource_sem);  // Acquire a resource
    
    printf("Thread %d: Got resource\n", thread_id);
    sleep(1);  // Use resource
    
    printf("Thread %d: Releasing resource\n", thread_id);
    sem_post(&resource_sem);  // Release resource
    
    return NULL;
}

int main() {
    // Initialize binary semaphore (like mutex)
    sem_init(&binary_sem, 0, 1);  // counter = 1
    
    // Initialize counting semaphore
    sem_init(&resource_sem, 0, 3);  // counter = 3 (max 3 threads)
    
    pthread_t threads[5];
    
    // Create 5 threads but only 3 can access resource at a time
    for(int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, counting_semaphore_example, (void*)i);
    }
    
    for(int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&binary_sem);
    sem_destroy(&resource_sem);
    
    return 0;
}
```

### Output (Expected)
```
Thread 0: Waiting for resource
Thread 1: Waiting for resource
Thread 2: Waiting for resource
Thread 3: Waiting for resource
Thread 4: Waiting for resource
Thread 0: Got resource
Thread 1: Got resource
Thread 2: Got resource
Thread 3: Waiting...
Thread 4: Waiting...
[After 1 second]
Thread 0: Releasing resource
Thread 3: Got resource
Thread 1: Releasing resource
Thread 4: Got resource
Thread 2: Releasing resource
Thread 3: Releasing resource
Thread 4: Releasing resource
```

---

## 6.3 Condition Variables

A **Condition Variable** lets threads wait for a specific condition to become true.

### Condition Variable Concept

```
Scenario: Producer-Consumer Problem

Producer: Makes data
Consumer: Waits for data, then uses it

Without condition variable:
├─ Consumer: while(no_data) { sleep(1ms); }  // Busy waiting - wastes CPU
├─ Producer: Make data
└─ Consumer: Wakes up after sleep, finds data

Better with condition variable:
├─ Consumer: Wait until data is available (efficient sleep)
├─ Producer: Make data, then SIGNAL the condition variable
└─ Consumer: Wakes up immediately
```

### Condition Variable Code Example

```c
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool data_ready = false;

void* producer(void* arg) {
    printf("Producer: Making data\n");
    sleep(2);
    
    pthread_mutex_lock(&mutex);
    printf("Producer: Data ready\n");
    data_ready = true;
    
    // Signal waiting threads
    pthread_cond_signal(&cond);  // Wake up ONE waiting thread
    
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* consumer(void* arg) {
    pthread_mutex_lock(&mutex);
    
    // Wait for condition to be true
    while(!data_ready) {
        printf("Consumer: Waiting for data\n");
        // Wait: Release mutex, sleep, re-acquire mutex when woken
        pthread_cond_wait(&cond, &mutex);
        printf("Consumer: Woken up!\n");
    }
    
    printf("Consumer: Using data\n");
    
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t prod, cons;
    
    pthread_create(&cons, NULL, consumer, NULL);
    sleep(1);  // Let consumer start first
    pthread_create(&prod, NULL, producer, NULL);
    
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    return 0;
}
```

### Output
```
Consumer: Waiting for data
Producer: Making data
Producer: Data ready
Consumer: Woken up!
Consumer: Using data
```

### Condition Variable Operations

```
1. pthread_cond_init()
   Initialize condition variable

2. pthread_cond_wait(&cond, &mutex)
   - Release mutex
   - Sleep waiting for signal
   - Upon wake: re-acquire mutex before returning
   - MUST hold mutex when calling

3. pthread_cond_signal(&cond)
   - Wake ONE waiting thread
   - That thread transitions from WAITING to RUNNABLE

4. pthread_cond_broadcast(&cond)
   - Wake ALL waiting threads
   - All transition to RUNNABLE

5. pthread_cond_destroy(&cond)
   Clean up condition variable
```

### Why Use Condition Variables?

**Without condition variable**:
```c
// Busy waiting - wastes CPU cycles!
while(!data_ready) {
    sleep(10);  // Sleep then check again
    // If data comes in 5ms, still waits 10ms
    // If no data for 1 second, wasted 10 sleep cycles
}
```

**With condition variable**:
```c
// Efficient waiting
pthread_cond_wait(&cond, &mutex);
// Wakes up immediately when data is ready
// No wasted CPU cycles checking
```

---

## 6.4 Reader-Writer Locks

A **Reader-Writer Lock** allows:
- Multiple readers to access data simultaneously
- Only one writer (exclusive access)

### Scenario

```
Bank Account Balance: $1000

Readers (read balance):
├─ Person A: reads balance
├─ Person B: reads balance (same time as A - OK)
├─ Person C: reads balance (same time as A,B - OK)
└─ Multiple readers simultaneously - NO PROBLEM

Writers (modify balance):
├─ Teller A: deposits $100 (LOCKS everyone out)
├─ If Teller A has lock:
│  ├─ Other readers: BLOCKED
│  ├─ Other writers: BLOCKED
│  └─ Teller A: Exclusive access
```

### Reader-Writer Lock Code

```c
#include <pthread.h>
#include <stdio.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int shared_data = 0;

void* reader_thread(void* arg) {
    int id = (int)arg;
    
    for(int i = 0; i < 3; i++) {
        pthread_rwlock_rdlock(&rwlock);  // Acquire read lock
        printf("Reader %d: Reading data = %d\n", id, shared_data);
        pthread_rwlock_unlock(&rwlock);  // Release lock
        
        sleep(1);
    }
    return NULL;
}

void* writer_thread(void* arg) {
    int id = (int)arg;
    
    for(int i = 0; i < 3; i++) {
        pthread_rwlock_wrlock(&rwlock);  // Acquire write lock
        shared_data++;
        printf("Writer %d: Writing data = %d\n", id, shared_data);
        pthread_rwlock_unlock(&rwlock);  // Release lock
        
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t readers[3], writers[2];
    
    for(int i = 0; i < 3; i++)
        pthread_create(&readers[i], NULL, reader_thread, (void*)i);
    
    for(int i = 0; i < 2; i++)
        pthread_create(&writers[i], NULL, writer_thread, (void*)i);
    
    for(int i = 0; i < 3; i++)
        pthread_join(readers[i], NULL);
    
    for(int i = 0; i < 2; i++)
        pthread_join(writers[i], NULL);
    
    return 0;
}
```

### Timeline

```
Time:  R1 (Reader1)       R2 (Reader2)       W1 (Writer1)
────────────────────────────────────────────────────────
0      rdlock ✓           -                  -
1      reading            rdlock ✓           -
2      reading            reading            wrlock (BLOCKED)
3      reading            reading            waiting...
4      reading            reading            waiting...
5      -                  reading            waiting...
6      -                  unlock             wrlock ✓ (lock acquired)
7      -                  -                  writing
8      -                  -                  unlock
```

---

## 6.5 Barrier

A **Barrier** synchronizes threads so they wait for each other before proceeding.

### Barrier Concept

```
Scenario: Parallel computation with phases

Phase 1: All threads do independent work
Phase 2: Wait for all threads to finish Phase 1
Phase 3: All threads do combined work
Phase 4: Wait for all threads to finish Phase 3
...

Without barrier:
├─ Thread 1: finishes phase 1, starts phase 2
├─ Thread 2: still working on phase 1
└─ WRONG DATA for thread 1 (thread 2's phase 1 not done yet)

With barrier:
├─ Thread 1: finishes phase 1, WAITS at barrier
├─ Thread 2: finishes phase 1, WAITS at barrier
├─ When all at barrier: BARRIER OPENS
└─ All proceed together to phase 2
```

### Barrier Code Example

```c
#include <pthread.h>
#include <stdio.h>

pthread_barrier_t barrier;
int thread_data[4] = {0};

void* worker_thread(void* arg) {
    int id = (int)arg;
    
    // Phase 1: Independent work
    printf("Thread %d: Phase 1 - Working\n", id);
    thread_data[id] = id * 10 + 5;
    sleep(id);  // Different threads take different times
    printf("Thread %d: Phase 1 - Done\n", id);
    
    // All threads wait here
    printf("Thread %d: Waiting at barrier\n", id);
    pthread_barrier_wait(&barrier);
    printf("Thread %d: Barrier passed, moving to phase 2\n", id);
    
    // Phase 2: Use data from all threads
    printf("Thread %d: Phase 2 - Data:", id);
    for(int i = 0; i < 4; i++) {
        printf(" [%d]=%d", i, thread_data[i]);
    }
    printf("\n");
    
    return NULL;
}

int main() {
    pthread_t threads[4];
    
    // Initialize barrier for 4 threads
    pthread_barrier_init(&barrier, NULL, 4);
    
    for(int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, worker_thread, (void*)i);
    }
    
    for(int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_barrier_destroy(&barrier);
    
    return 0;
}
```

### Output
```
Thread 0: Phase 1 - Working
Thread 1: Phase 1 - Working
Thread 2: Phase 1 - Working
Thread 3: Phase 1 - Working
Thread 0: Phase 1 - Done
Thread 0: Waiting at barrier
Thread 1: Phase 1 - Done
Thread 1: Waiting at barrier
Thread 2: Phase 1 - Done
Thread 2: Waiting at barrier
Thread 3: Phase 1 - Done
Thread 3: Waiting at barrier
Thread 0: Barrier passed, moving to phase 2
Thread 1: Barrier passed, moving to phase 2
Thread 2: Barrier passed, moving to phase 2
Thread 3: Barrier passed, moving to phase 2
[All threads now have complete data]
```

---

# Module 7: Thread Models

## 7.1 User-Level Threads vs Kernel-Level Threads

### **Kernel-Level Threads**

Threads managed by the OS kernel:

```
User Space: Your program
    ├── Thread 1
    ├── Thread 2
    └── Thread 3
           ↓ (syscall)
Kernel Space: OS manages these
    ├── Kernel Thread 1 ← Scheduled independently
    ├── Kernel Thread 2 ← Scheduled independently
    ├── Kernel Thread 3 ← Scheduled independently
    └── Scheduler picks which runs next
```

**Advantages**:
- True parallelism on multicore
- If one thread blocks, others continue
- OS scheduler optimizes thread placement

**Disadvantages**:
- Expensive syscalls to create/switch
- More memory per thread
- Context switch overhead

---

### **User-Level Threads**

Threads managed by a user-space library (not OS):

```
User Space:
    ├── Thread 1
    ├── Thread 2 ← User-space scheduler
    ├── Thread 3 ← decides which runs
    └── All map to ONE kernel thread
           ↓
Kernel Space:
    └── Kernel Thread 1 (only 1!)
         └── Scheduler doesn't know about user threads
```

**Advantages**:
- Very fast to create/switch (no syscalls)
- No kernel involvement needed
- Flexible scheduling

**Disadvantages**:
- Cannot use multicore (all on 1 kernel thread)
- If one blocks, all block (kernel thread blocks)
- No true parallelism

---

## 7.2 Thread Mapping Models

### **Model 1: Many-to-One**

Many user threads → One kernel thread

```
User Threads: 1000 threads
    ↓ (map to)
Kernel Threads: 1 thread

One Core:
┌──────────────────────────────┐
│ Kernel Thread 1 running      │
│ User thread context switch   │
│ User thread context switch   │
│ User thread context switch   │
│ (time slices among 1000!)    │
└──────────────────────────────┘
```

**Use case**: Green threads (Python, early Java)
**Problem**: One blocking call blocks all 1000 threads!

---

### **Model 2: One-to-One**

One user thread → One kernel thread

```
User Threads: 1000 threads
    ↓ (map to)
Kernel Threads: 1000 threads

Two Cores:
┌──────────────────────────────┐
│ Core 1: Kernel Thread 1      │
│ Core 2: Kernel Thread 2      │
│ Others: Waiting in queue     │
└──────────────────────────────┘
```

**Advantages**: True parallelism, non-blocking
**Disadvantages**: Cannot create many threads (resource limit)

**Use case**: Modern Linux (pthreads), Windows threads

---

### **Model 3: Many-to-Many**

Many user threads → Many kernel threads

```
User Threads: 1000 threads
    ↓ (map to)
Kernel Threads: 10 kernel threads

More flexible:
- Better than many-to-one (10 threads don't all block)
- Better than one-to-one (don't create 1000 kernel threads)
- OS scheduler manages 10 kernel threads
- User library schedules 1000 user threads
```

**Use case**: Some embedded systems, Solaris

---

## 7.3 POSIX Threads (pthreads)

Most modern systems use **POSIX threads** - One-to-One model.

```
pthread_t = kernel thread handle

linux$ getconf _NPROCESSORS_ONLN
8

Quad-core CPU with hyperthreading:
├── CPU 0 (Core 1, thread 1)
├── CPU 1 (Core 1, thread 2)
├── CPU 2 (Core 2, thread 1)
├── CPU 3 (Core 2, thread 2)
├── CPU 4 (Core 3, thread 1)
├── CPU 5 (Core 3, thread 2)
├── CPU 6 (Core 4, thread 1)
└── CPU 7 (Core 4, thread 2)
```

---

# Module 8: Common Threading Issues

## 8.1 Race Condition (Detailed Review)

**Race Condition**: Two or more threads access shared data simultaneously, at least one modifies it, result is unpredictable.

### Detection

```c
// Check for race conditions:
volatile int counter = 0;  // volatile = don't optimize away

void* thread_func(void* arg) {
    for(int i = 0; i < 1000000; i++) {
        counter++;  // RACE CONDITION if counter not protected
    }
    return NULL;
}
```

**Run multiple times**:
```
Run 1: counter = 2000000
Run 2: counter = 1999999
Run 3: counter = 2000000
Run 4: counter = 1999998
```

If results vary → RACE CONDITION exists!

### Fix

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
volatile int counter = 0;

void* thread_func(void* arg) {
    for(int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);
        counter++;  // Now protected
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
```

---

## 8.2 Deadlock (Detailed Review)

**Deadlock**: Two or more threads waiting for each other forever.

### Four Conditions (All Must Be Present)

1. **Mutual Exclusion**: Resource only used by one thread
2. **Hold and Wait**: Thread holds resource, waits for another
3. **No Preemption**: Cannot take resource from thread
4. **Circular Wait**: Cycle in wait-for graph

### Detection Strategy

**Wait-For Graph**:
```
Thread 1 → (waiting for resource held by) → Thread 2
Thread 2 → (waiting for resource held by) → Thread 1

Cycle exists → DEADLOCK!
```

### Prevention Strategies

**Strategy 1: Lock Ordering**
```c
// Always acquire locks in same order
Lock Order: lock1 < lock2

Thread 1:
    lock(lock1);
    lock(lock2);

Thread 2:
    lock(lock1);  // Same order!
    lock(lock2);
```

**Strategy 2: Timeout**
```c
// Don't wait forever
struct timespec timeout;
timeout.tv_sec = time(NULL) + 1;  // 1 second timeout

if(pthread_mutex_timedlock(&lock, &timeout) != 0) {
    printf("Lock acquisition timed out - deadlock?\n");
    // Handle timeout
}
```

**Strategy 3: Try-Lock**
```c
// Don't block, release and retry
if(pthread_mutex_trylock(&lock1) == 0) {
    if(pthread_mutex_trylock(&lock2) == 0) {
        // Got both locks
        pthread_mutex_unlock(&lock2);
        pthread_mutex_unlock(&lock1);
    } else {
        // Failed to get lock2, release lock1 and retry
        pthread_mutex_unlock(&lock1);
    }
}
```

---

## 8.3 Starvation (Detailed Review)

**Starvation**: A thread never gets CPU time (infinite wait).

### Causes

```
1. High-priority threads monopolize CPU
2. Lock scheduling is unfair
3. Busy-waiting in high-priority thread
```

### Example

```c
// Starvation due to scheduling

void* high_priority_worker(void* arg) {
    while(1) {
        // Constantly runs - uses all CPU
        calculate_something();
    }
}

void* low_priority_worker(void* arg) {
    // Never runs because scheduler always picks high priority
    return NULL;
}
```

### Fix

```c
// Solution 1: Priority boost
// Low priority threads get boosted after waiting long time

// Solution 2: Aging
// Threads waiting long time get higher priority

// Solution 3: Fair scheduling
// Use round-robin with time slices
```

---

## 8.4 Memory Visibility

**Memory Visibility**: What one thread writes, another thread sees.

### Problem

```c
int flag = 0;
int value = 0;

void* writer(void* arg) {
    value = 42;
    flag = 1;  // Signal reader
    return NULL;
}

void* reader(void* arg) {
    while(flag == 0) {
        // Busy wait for flag
    }
    printf("Value = %d\n", value);  // Might print 0!
    return NULL;
}
```

**Why?** CPU compiler optimizations and cache effects:

```
Writer CPU cache:          Reader CPU cache:
value = 42 (cache)         flag = 0 (cache)
flag = 1 (cache)           value = 0 (cache)
(might not flush to RAM)   
```

**Reader might read stale cache values!**

### Fix

```c
volatile int flag = 0;  // Volatile = don't cache
volatile int value = 0;

// OR use proper synchronization:
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* writer(void* arg) {
    pthread_mutex_lock(&lock);
    value = 42;
    flag = 1;
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* reader(void* arg) {
    pthread_mutex_lock(&lock);
    while(flag == 0) {
        pthread_mutex_unlock(&lock);
        sleep(1);
        pthread_mutex_lock(&lock);
    }
    printf("Value = %d\n", value);  // Now safe!
    pthread_mutex_unlock(&lock);
    return NULL;
}
```

---

# Module 9: Best Practices

## 9.1 Rules for Thread-Safe Code

### Rule 1: Protect Shared Data

```c
// BAD: No protection
int shared = 0;
shared++;

// GOOD: Protected with mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int shared = 0;

pthread_mutex_lock(&lock);
shared++;
pthread_mutex_unlock(&lock);
```

### Rule 2: Always Use Locks Consistently

```c
// BAD: Inconsistent locking
int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Function 1: Uses lock
void increment() {
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
}

// Function 2: Doesn't use lock!
int get_counter() {
    return counter;  // RACE CONDITION!
}

// GOOD: Always lock
int get_counter() {
    pthread_mutex_lock(&lock);
    int val = counter;
    pthread_mutex_unlock(&lock);
    return val;
}
```

### Rule 3: Minimize Critical Sections

```c
// BAD: Large critical section
pthread_mutex_lock(&lock);
int x = expensive_calculation();  // 10 seconds
counter += x;
int y = another_expensive_calc(); // 10 seconds
result = y * 2;
pthread_mutex_unlock(&lock);

// GOOD: Minimal critical section
int x = expensive_calculation();   // No lock - fine
int y = another_expensive_calc();  // No lock - fine
pthread_mutex_lock(&lock);
counter += x;
result = y * 2;
pthread_mutex_unlock(&lock);
```

### Rule 4: Avoid Nested Locks When Possible

```c
// BAD: Nested locks (risk of deadlock)
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void risky_function() {
    pthread_mutex_lock(&lock1);
    // ...
    pthread_mutex_lock(&lock2);  // Nested - risky!
    // ...
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
}

// GOOD: If needed, use recursive mutex
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
pthread_mutex_t recursive_lock;
pthread_mutex_init(&recursive_lock, &attr);

void safe_function() {
    pthread_mutex_lock(&recursive_lock);
    // Can lock again without deadlock
    pthread_mutex_lock(&recursive_lock);
    // ...
    pthread_mutex_unlock(&recursive_lock);
    pthread_mutex_unlock(&recursive_lock);
}
```

### Rule 5: Use Condition Variables for Waiting

```c
// BAD: Busy waiting
while(!condition) {
    sleep(1);  // Wastes CPU checking
}

// GOOD: Condition variable
pthread_mutex_lock(&lock);
while(!condition) {
    pthread_cond_wait(&cond, &lock);  // Efficient
}
pthread_mutex_unlock(&lock);
```

---

## 9.2 Thread Creation Best Practices

```c
#include <pthread.h>
#include <stdio.h>

// GOOD: Proper thread creation
void* thread_function(void* arg) {
    int* thread_id = (int*)arg;
    
    printf("Thread %d started\n", *thread_id);
    
    // Thread work here
    sleep(1);
    
    printf("Thread %d finishing\n", *thread_id);
    
    free(thread_id);  // Free allocated memory
    return NULL;
}

int main() {
    pthread_t threads[4];
    
    for(int i = 0; i < 4; i++) {
        // Allocate memory for thread argument
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        
        // Create thread
        if(pthread_create(&threads[i], NULL, thread_function, thread_id) != 0) {
            perror("pthread_create failed");
            free(thread_id);
            return 1;
        }
    }
    
    // Wait for all threads
    for(int i = 0; i < 4; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            return 1;
        }
    }
    
    printf("All threads completed\n");
    return 0;
}
```

---

## 9.3 Testing Thread Safety

```c
// Test 1: Stress test with many threads
void test_stress() {
    pthread_t threads[100];
    
    for(int i = 0; i < 100; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }
    
    for(int i = 0; i < 100; i++) {
        pthread_join(threads[i], NULL);
    }
    
    assert(counter == 100 * ITERATIONS);  // Check result is correct
}

// Test 2: Run multiple times
void test_multiple_runs() {
    for(int run = 0; run < 1000; run++) {
        test_stress();
        // If any run has wrong result → race condition
    }
}

// Test 3: Check thread sanitizer
// Compile with: gcc -fsanitize=thread -g program.c -o program
// Run: ./program
// ThreadSanitizer will detect data races automatically!
```

---

## 9.4 Debugging Threaded Programs

### Tool 1: ThreadSanitizer
```bash
gcc -fsanitize=thread -g program.c -o program
./program
# Detects data races automatically
```

### Tool 2: Valgrind
```bash
valgrind --tool=helgrind ./program
# Detects locks, races, deadlocks
```

### Tool 3: GDB with Threads
```bash
gdb ./program
(gdb) info threads
(gdb) thread 1
(gdb) thread 2
(gdb) break function if thread_id == 2
```

---

## Summary Table: All Synchronization Primitives

| Primitive | Use Case | Overhead | Complexity |
|-----------|----------|----------|-----------|
| Mutex | General locking | Low | Low |
| Semaphore | Resource counting | Low | Medium |
| Condition Var | Wait for events | Medium | Medium |
| Reader-Writer Lock | Many readers, few writers | Medium | High |
| Barrier | Synchronize threads | Medium | Low |
| Monitor | OOP synchronization | High | Low |

---

## Key Concepts Checklist

- [ ] Process vs Thread difference
- [ ] Why processes alone aren't enough
- [ ] What threads share vs own
- [ ] Thread states and transitions
- [ ] Context switching and scheduling
- [ ] Race conditions and detection
- [ ] Critical sections
- [ ] Deadlock conditions and prevention
- [ ] Starvation and livelock
- [ ] Mutex and semaphore usage
- [ ] Condition variables
- [ ] Reader-writer locks
- [ ] Barriers
- [ ] Thread models (kernel vs user)
- [ ] POSIX threads
- [ ] Memory visibility
- [ ] Testing thread-safe code
- [ ] Debugging techniques

---

## Practice Problems

### Problem 1: Fix the Race Condition
```c
// BUGGY CODE - Find and fix the race condition
int shared_counter = 0;

void* increment_worker(void* arg) {
    for(int i = 0; i < 1000; i++) {
        shared_counter++;
    }
    return NULL;
}
```

**Answer**: Add mutex around shared_counter access.

### Problem 2: Prevent Deadlock
```c
// BUGGY CODE - This can deadlock
void transfer_money() {
    lock(account1);
    lock(account2);
    // transfer
    unlock(account2);
    unlock(account1);
}
```

**Answer**: Always acquire locks in same order.

### Problem 3: Consumer-Producer
Implement a thread-safe queue where producers add items and consumers remove items using condition variables.

**Answer**: Use mutex + condition variables (see Module 6.3 for example).

---

## Conclusion

You've now learned:
1. **Fundamentals** - What threads are and why we need them
2. **Architecture** - How threads share and own resources
3. **Lifecycle** - Thread states and transitions
4. **Scheduling** - How OS decides which thread runs
5. **Problems** - Race conditions, deadlock, starvation
6. **Solutions** - Mutexes, semaphores, condition variables
7. **Models** - Different threading architectures
8. **Issues** - Common problems and fixes
9. **Best Practices** - How to write correct threaded code

**Next Steps**: Practice implementing threaded programs and use tools like ThreadSanitizer to verify correctness!

Good luck with your professor! 📚✨
```

---

## How to Save This File

**On Windows**:
1. Open Notepad
2. Paste the entire content above
3. Save as `Threads_Complete_Course.md` in Documents folder

**On Mac/Linux**:
1. Open Terminal
2. Run: `cat > Threads_Complete_Course.md << 'EOF'`
3. Paste the entire content
4. Press `Ctrl+D` to save

**Or use any text editor** and save with `.md` extension.

---

You now have a complete, comprehensive course covering **ALL the threading topics** your professor expects! 📚✨

This includes everything from your original notes PLUS:
- ✅ Thread states and lifecycle
- ✅ Scheduling and context switching
- ✅ Detailed race condition analysis
- ✅ Deadlock, starvation, and livelock
- ✅ All synchronization solutions (mutex, semaphore, condition variables, barriers)
- ✅ Thread models explained
- ✅ Best practices and debugging
- ✅ Code examples for everything

Good luck with your professor! Your notes will definitely impress them now! 💪