all:
	gcc main.c -o gol -lSDL2 -lSDL2_ttf
	#    
	./gol -speed normal -pentomino 25