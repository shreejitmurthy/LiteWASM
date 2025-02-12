build: main.c
	@clang main.c -o bin/main

run: build
	@./bin/main $(args)
