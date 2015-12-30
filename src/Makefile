run : main
	./main
main : main.o Parser.o View.o DocuSet.o Lexer.o Token.o patternItem.o regex.o tokenizer.o
	g++ -o main main.o
main.o : main.cpp Lexer.o Parser.o DocuSet.o 
	g++ -c -std=c++11 main.cpp
Parser.o : Parser.cpp Lexer.o Token.o Tag.h View.o
	g++ -c -std=c++11 Parser.cpp
View.o : View.cpp patternItem.o regex.o DocuSet.o
	g++ -c -std=c++11 View.cpp
DocuSet.o :DocuSet.cpp tokenizer.o
	g++ -c -std=c++11 DocuSet.cpp
Lexer.o : Lexer.cpp Token.o Tag.h
	g++ -c -std=c++11 Lexer.cpp
Token.o : Token.cpp
	g++ -c -std=c++11 Token.cpp
patternItem.o : patternItem.cpp
	g++ -c -std=c++11 patternItem.cpp
regex.o : regex.cpp
	g++ -c -std=c++11 regex.cpp
tokenizer.o : tokenizer.cpp
	g++ -c -std=c++11 tokenizer.cpp
clean:
	rm *.o