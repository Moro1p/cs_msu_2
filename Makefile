all: task

main.o: main.c
	gcc -m32 -c -o main.o main.c
func_values.o: func_values.asm
	nasm -f elf32 -o func_values.o func_values.asm
task: main.o func_values.o
	gcc -o task main.o func_values.o -m32 -lm
clean:
	rm -rf main.o func_values.o
