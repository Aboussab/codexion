***This project has been created as part of the 42 curriculum by  aboussab.***

# “Description”

 ***Codexion is a concurrency simulation project inspired by the classic Dining Philosophers problem. The Dining Philosopher Problem is a classic synchronization and concurrency problem that deals with resource sharing, deadlock, and starvation in systems where multiple processes require limited resources. Codexion project It models a group of coders sitting in a circular co-working space, each competing for shared USB dongles needed to compile their quantum code. Each coder alternates between three states — compiling, debugging, and refactoring. To compile, a coder must simultaneously hold two dongles, one on each side. Since dongles are shared between neighboring coders, the simulation must carefully orchestrate access to prevent deadlocks, starvation, and burnout. The simulation stops either when a coder burns out from lack of compiling or when all coders have completed the required number of compilations. The goal of this project is to master resource synchronization using POSIX threads, mutexes, and condition variables in C.***
 
# “Instructions”
**************************************************************************************************************************
To compile the project, run ***make*** at the root of the repository. This will produce an executable called codexion. To run the simulation, use the following command to run the excutable with the following arguments:

***./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler.***

All arguments are mandatory. Time values are in milliseconds. The scheduler must be either fifo or edf.
launches a simulation with 4 coders, 800ms burnout limit, 200ms for each phase, 5 required compiles, no cooldown, and FIFO scheduling. To clean compiled files run make clean, to remove the binary run make fclean, and to recompile from scratch run make re.

Example:

*./codexion 4 800 200 200 200 5 0 fifo*

***************************************************************************************************************************

# “Resources”

**The following resources were useful for understanding the concepts involved in this project:**

**-The classic Dining Philosophers problem description by Dijkstra provides the theoretical foundation**

**-The POSIX threads programming guide by Blaise Barney at Lawrence Livermore National Laboratory is an excellent practical reference for pthread functions.**

- https://hpc-tutorials.llnl.gov/posix/#note-this-tutorial-is-no-longer-supported-it-remains-for-archival-purposes 

*-The Linux man pages for ***pthread_mutex_lock***, ***pthread_cond_wait***, ***pthread_cond_timedwait***, and ***gettimeofday*** were consulted throughout development.*

**-there was also a list of tutorials used to understand The POSIX library like :**

-https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2

-https://www.youtube.com/watch?v=M9HHWFp84f0

-https://www.youtube.com/watch?v=f-IlYeyTwzY&t=119s

**-AI was used during this project in several ways — to understand and clarify concurrency concepts such as condition variables, spurious wakeups, and deadlock prevention strategies, to discuss the overall architecture and data structure design before writing any code, and to review logic errors in specific functions during development. it also helped find a helpful trusted resources to clearly read about all the conceptes.**

# “Blocking cases handled”

**prevention is achieved through the odd/even alternation strategy — even-numbered coders always pick up their right dongle first then their left, while odd-numbered coders do the opposite. This breaks the circular wait condition which is one of Coffman's four necessary conditions for deadlock. Starvation prevention is handled through the scheduling policies — FIFO ensures every coder is served in arrival order so no coder waits indefinitely, while EDF prioritizes the coder closest to burnout ensuring the most urgent coder gets access first. Dongle cooldown is enforced using pthread_cond_timedwait with an absolute timeout calculated from the release time, ensuring no dongle is reused before its cooldown expires. Burnout detection is handled by a dedicated monitor thread that checks every coder's last compile time every millisecond, guaranteeing the burnout message is printed within 10ms of the actual burnout event. Log serialization is achieved through a dedicated log mutex that every thread must acquire before printing, ensuring messages never interleave on the same line.**

# “Thread synchronization mechanisms”

***This project uses several POSIX threading primitives to coordinate access to shared resources. Each dongle is protected by a pthread_mutex_t that ensures only one thread at a time can read or modify the dongle's state — its availability, release time, and waiting queue. A pthread_cond_t is associated with each dongle so that coders can sleep efficiently while waiting for it to become available, rather than busy-waiting and wasting CPU. The simulation's stop flag is protected by a dedicated flag_mutex so that both the monitor and coder threads can safely read and write it without race conditions. A separate log_mutex serializes all print operations across every thread. Each coder also has its own coder_mutex protecting its last_compile_time and counter_compiling fields since both the coder thread and the monitor thread access these simultaneously. Race conditions are prevented by always acquiring the relevant mutex before reading or writing any shared field and releasing it immediately after. When the simulation stops, the monitor broadcasts on all dongle condition variables to wake up any coder threads sleeping inside the dongle acquisition function, allowing them to notice the stop flag and exit cleanly.***