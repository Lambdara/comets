build:
	gcc src/main.c src/graphics.c src/world.c -lGL -lGLEW -lglfw -Wall -lm -O3 -o comets
