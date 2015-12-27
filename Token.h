#pragma once
#include <string>
#include "Tag.h"

using namespace std;

class Token {
public:
    int tag;
    string lexeme ="";
    int value;

    Token(int t) {tag = t;}
    int get_tag() {return tag;}
    string get_lexeme() {return lexeme;}
    int get_value() {return value;}
};

class Num:public Token {
public:
    Num(int v):Token(Tag.NUM) {value = v;}
};

class Word:public Token{
public:
    Word(string str, int tag):Token(tag) {
        lexeme = str;
    }
};

