snake_game: core.o food.o snake.o src/main.c
	gcc -I./include -lallegro -lallegro_font -lallegro_primitives snake.o food.o core.o src/main.c -o snake_game

snake.o: include/base.h include/snake.h src/snake.c
	gcc -I./include -c src/snake.c -o snake.o

food.o: snake.o include/food.h src/food.c
	gcc -I./include -c ./src/food.c -o food.o

core.o: food.o include/core.h src/core.c
	gcc -I./include -c ./src/core.c -o core.o

