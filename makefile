myshell: src/myshell.c src/commands.c include/commands.h src/fork.c include/fork.h
	gcc -Wall -I include src/myshell.c src/commands.c src/fork.c -o bin/myshell