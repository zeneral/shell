main: main.c shell.c parser.c process.c shell.h
	gcc -o main main.c shell.c parser.c process.c -lreadline 
	
debug: main.c shell.c parser.c process.c shell.h
	gcc -o main main.c shell.c parser.c process.c -lreadline -g

clean:
	rm main
