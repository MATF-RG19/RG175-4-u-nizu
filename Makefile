all:
	gcc -o 4_u_nizu main.c drawing.c gamelib.c -lGL -lGLU -lglut -Wall -Wextra -lm
