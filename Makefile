main: main.c shell.c parser.c process.c shell.h
	gcc -o main main.c shell.c parser.c  -lreadline 
	
debug: main.c shell.c parser.c process.c shell.h
	gcc -o main main.c shell.c parser.c debug.c -DDEBUG_SHELL -lreadline -g

test: debug
	cat test.txt | ./main > mini_shell.txt
	cat test.txt | bash > bash.txt
	diff mini_shell.txt bash.txt

clean:
	rm main
	rm mini_shell.txt
	rm bash.txt
	rm out.txt

