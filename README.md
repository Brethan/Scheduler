# Scheduler Simulator

This project implements a simulator for a process scheduler using the following parameters:  
- PID: The identifier for the process.
- Arrival Time: The time at which the process should arrive in the ready queue.
- Total CPU Time: This is how long the process requires to be ran before it can be moved to 
the terminated list.
- I/O Frequency: This is how often the process requires to be moved to the waiting queue.
If the process has been running for an integer multiple of the I/O frequency, it will be moved to
the waiting queue.
- I/O Duration: When a process is moved to the waiting Queue, this is the 
amount of time it takes for the process to be moved back to the ready queue.

**Some Notes:**
- A process will be forced to stop running (preempted) if its run time exceeds the time slice
quantity specified in [the source code](./src/main.c).
- A process with Total CPU Time < I/O Frequency will never be moved to the waiting queue.
- Response time is calculated based on the average difference between Arrival time and first run time
- Wait time is calculated based on the average amount of time spent in the ready queue

I've provided both .sh files and .bat files for you to run the project
for Ubuntu / Fedora and Windows respectively. Running these scripts will 
compile the source code to an executable.

I'd just like to note that running the project on Linux with the bash scripts
will show the visualizer, while running on Windows with the batch scripts will not.

Apparently Windows isn't too keen on letting you move the console cursor
around using printf.

I've also included a testRan.sh and a testRan.bat file which will automatically
create a new list of processes to be simulated each time its run.

The ready queue will fill up very fast in the visualizer, so after about 10 or 
nodes appear, it will be appended with "...". This is to prevent any line overflows. 
The terminated queue will also clear after about 10 nodes in a very satisfying way!

The processes are ordered by arrival time by default, however to prove the simulation
will work regardless of if the arrival times are in order, you can comment out the 
variable "ordered" on line 4 of [randomizer.py](./randomizer.py). You can also mess 
with the upper and lower bounds of the different process members.

## TL;DR: Please run this project on Linux! You can mess around with the randomizer settings in [randomizer.py](./randomizer.py)
