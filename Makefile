all: Token.o LexAnalyzer.o Compiler.o main.o 
	g++ main.o Token.o LexAnalyzer.o Compiler.o -o main   

clean:
	if [ -f Token.o ]; then rm Token.o; fi
	if [ -f Compiler.o ]; then rm Compiler.o; fi
	if [ -f LexAnalyzer.o ]; then rm LexAnalyzer.o; fi
	if [ -f main.o ]; then rm main.o; fi
	if [ -f main ]; then rm main; fi

Token.o: Token.h Token.cpp
	g++ -c Token.cpp

Compiler.o: Compiler.h Compiler.cpp
	g++ -c Compiler.cpp

LexAnalyzer.o: LexAnalyzer.h Compiler.cpp
	g++ -c LexAnalyzer.cpp
	
main.o: main.cpp Compiler.h
	g++ -c main.cpp
