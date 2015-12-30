#ifndef __LEXER_H__
#define __LEXER_H__
#include <map>
#include <string>
#include <iostream>

#include "Tag.h"
#include "Token.cpp"
using namespace std;
class Lexer;

bool isDigit(const char& peek) {
    if ('0' <= peek && peek <= '9') {
        return true;
    } else {
        return false;
    }
}

bool isLetter(const char& peek) {
    if ('a' <= peek && peek <= 'z' ||
                 'A' <= peek && peek <= 'Z' ||
                 peek == '_') {
        return true;
    } else {
        return false;
    }
}

bool isLetterOrDigit(const char& peek) {
    return isDigit(peek) | isLetter(peek);
}

class Lexer {
private:
    char peek = ' ';
    string code;
    int codeLength;
    int index;
    int line = 0;
    int col = 0;
    bool endFlag;
    map<string, Word> words;

    Word
        leftparenthesis = Word("(", Tag.LEFTPARENTHESIS), rightparenthesis = Word(")", Tag.RIGHTPARENTHESIS), // ()
        leftbrace = Word("{", Tag.LEFTBRACE), rightbrace = Word("}", Tag.RIGHTBRACE),  // {}
        less = Word("<", Tag.LESS), greater = Word(">", Tag.GREATER), //<>

        dot = Word(".", Tag.DOT),
        comma = Word(",", Tag.COMMA),
        semicolon = Word(";", Tag.SEMICOLON); // . , ;

public:
    Lexer(string cod, int length) {
        codeLength = length;
        code = cod;
        index = 0;
        endFlag = false;
        line = 0;
        col = 0;
        peek = ' ';

        reserve(Word("create",  Tag.CREATE));
        reserve(Word("view", Tag.VIEW));
        reserve(Word("as", Tag.AS));
        reserve(Word("output", Tag.OUTPUT));
        reserve(Word("select", Tag.SELECT));
        reserve(Word("from", Tag.FROM));
        reserve(Word("extract", Tag.EXTRACT));
        reserve(Word("regex", Tag.REGEX));
        reserve(Word("on", Tag.ON));
        reserve(Word("return", Tag.RETURN));
        reserve(Word("group", Tag.GROUP));
        reserve(Word("and", Tag.AND));
        reserve(Word("Token", Tag.TOKEN));
        reserve(Word("pattern", Tag.PATTERN));
    }

    void back_end() {
        index = 0;
        endFlag = false;
        line = 0;
        col = 0;
        peek = ' ';
    }

    void readch() {
        peek = code[index];
        index++;
        col++;
        
        if (codeLength - index <= 0) endFlag = true;
    }

    void reserve(Word w) {
        words.insert(pair<string, Word>(w.get_lexeme(), w));
    }

    bool end_of_input() {return endFlag;}

    void index_backward() {index--;}

    Token scan() {
        while(!end_of_input()) {
            readch();
            if (peek == ' ' || peek == '\t' || peek == '\r') {
                continue;
            } else if (peek == '\n') {
                line++;
                col = -1;
            } else break;
        }
        switch(peek) {
            case '(':
                return leftparenthesis;
            case ')':
                return rightparenthesis;
            case '{':
                return leftbrace;
            case '}':
                return rightbrace;
            case '<':
                return less;
            case '>':
                return greater;
            case '.':
                return dot;
            case ',':
                return comma;
            case ';':
                return semicolon;
            case '/':
                string reg = "";
                readch();
                while (peek != '/') {
                    reg += peek;
                    readch();
                }
                return Word(reg, Tag.REG);
        }
        if (isDigit(peek)) {
            int v = 0;
            do {
                v = 10*v + peek - 48;
                readch();
            } while(!end_of_input() && isDigit(peek));
            if (!(peek == ' ' || peek == '\t' || peek == '\r' || peek == '\n')) {index_backward();}
            return Num(v);
        }
        if (isLetter(peek)) {
            string re = "";
            do {
                re += peek;
                readch();
            } while (!end_of_input() && isLetterOrDigit(peek));
            if (!(peek == ' ' || peek == '\t' || peek == '\r' || peek == '\n')) {index_backward();}

            map<string, Word>::iterator it = words.find(re);
            if (it != words.end()) {
                return it->second;
            }
            Word w  = Word(re, Tag.ID);
            words.insert(pair<string, Word>(re, w));
            return w;
        }
        Token tok = Token(peek);
        peek = ' ';
        return tok;
    }
};
#endif