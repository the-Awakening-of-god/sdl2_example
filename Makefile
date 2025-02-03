build: 
	gcc -Wall ./src/*.c  -lSDL2   -I ./include/ -o game -ldl

run: 
		./game 

clean: 
	rm game
