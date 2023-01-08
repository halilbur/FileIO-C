compile:program run clean
program:lib ana
	@gcc matrixLib.o main.o -o main
lib:
	@gcc -c matrixLib.c -o matrixLib.o
ana:
	@gcc -c main.c -o main.o
run:
	@./main 
clean:
	@rm -f *.o main