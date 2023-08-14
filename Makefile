main: main.o pcb_queue.o io_process.o
	gcc -o main.exe main.o pcb_queue.o io_process.o
visual: visual.o cursor.o
	gcc -o mainV.exe visual.o cursor.o
main.o: src/main.c
	gcc -c src/main.c -o main.o
pcb_queue.o: src/pcb_queue.c 
	gcc -c src/pcb_queue.c -o pcb_queue.o
io_process.o: src/io_process.c
	gcc -c src/io_process.c -o io_process.o
cursor.o: src/visualizer/cursor.c
	gcc -c src/visualizer/cursor.c -o ./cursor.o
visual.o: src/visualizer/visual.c
	gcc -c src/visualizer/visual.c -o ./visual.o
.SILENT: