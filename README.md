***This project has been created as part of the 42 curriculum by  aboussab.***

### “Description”

 ***Codexion is a concurrency simulation project inspired by the classic Dining Philosophers problem. The Dining Philosopher Problem is a classic synchronization and concurrency problem that deals with resource sharing, deadlock, and starvation in systems where multiple processes require limited resources. Codexion project It models a group of coders sitting in a circular co-working space, each competing for shared USB dongles needed to compile their quantum code. Each coder alternates between three states — compiling, debugging, and refactoring. To compile, a coder must simultaneously hold two dongles, one on each side. Since dongles are shared between neighboring coders, the simulation must carefully orchestrate access to prevent deadlocks, starvation, and burnout. The simulation stops either when a coder burns out from lack of compiling or when all coders have completed the required number of compilations. The goal of this project is to master resource synchronization using POSIX threads, mutexes, and condition variables in C.***
 
### “Instructions”
**************************************************************************************************************************
To compile the project, run ***make*** at the root of the repository. This will produce an executable called codexion. To run the simulation, use the following command to run the excutable with the following arguments:

***./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler.***

All arguments are mandatory. Time values are in milliseconds. The scheduler must be either fifo or edf.

Example:

*./codexion 4 800 200 200 200 5 0 fifo*

launches a simulation with 4 coders, 800ms burnout limit, 200ms for each phase, 5 required compiles, no cooldown, and FIFO scheduling. To clean compiled files run make clean, to remove the binary run make fclean, and to recompile from scratch run make re.
***************************************************************************************************************************

###