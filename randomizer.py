from random import randint

def ranIntTuple(x: tuple) -> int:
	return randint(x[0], x[1])
# Comment the line below to order the random processes by arrival time
# ordered = "cabbage"

# Variables to change the simulation
numProcesses = 100

arrTimeBounds = (0, 3000)
cpuTimeBounds = (10, 350)
ioFreq_bounds = (10, 200)
ioTime_bounds = (50, 500)

time = []
cpu = []
freq = []
dur = []

result = "PID,Arrival Time,CPU Time,IO Frequency,IO Duration\n"

for i in range(numProcesses):
	if i % 4 == 0:
		time.append(ranIntTuple((0, 300)))
	else:
		time.append(ranIntTuple(arrTimeBounds))
	
	cpu.append(ranIntTuple(cpuTimeBounds))
	freq.append(ranIntTuple(ioFreq_bounds))
	dur.append(ranIntTuple(ioTime_bounds))
	
	
if "ordered" in globals():
	time.sort()
	
for i in range(len(time)):
	pid = 1001 + i
	result += f"{pid},{time[i]},{cpu[i]},{freq[i]},{dur[i]}\n"

f = open("./data/test_case_ran.csv", "w")
f.write(result.strip());
f.close();
