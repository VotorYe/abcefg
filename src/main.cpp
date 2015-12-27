#include <stdio.h>
#include <iostream>
#include <fstream>

#include "Lexer.h"
#include "Token.h"

using namespace std;

char* read_from(string file, int& length) {
    filebuf *pbuf;
    ifstream filestr;
    long size;
    filestr.open (file, ios::binary);
    // 获取filestr对应buffer对象的指针 
    pbuf=filestr.rdbuf();
    
    // 获取文件大小
    size=pbuf->pubseekoff (0,ios::end,ios::in);
    pbuf->pubseekpos (0,ios::in);
    
    char* buffer;
    buffer=new char[size];
    // 获取文件内容
    pbuf->sgetn (buffer,size);

    length = size;
    
    filestr.close();
    return buffer;
}

int test() {
    FILE *fp;
    int length;
    char* input = read_from("PerLoc.aql", length);

    Lexer lex = Lexer(input, length);
    while (!lex.end_of_input()) {
        Token tok = lex.scan();
        int tag = tok.get_tag();
        if (tag >= 151 && tag <= 164) {cout << "keyword" << " "  << tok.get_lexeme() << endl;}
        else if (tag >= 301 && tag <= 306) {cout << "brace" << " "  << tok.get_lexeme() << endl;}
        else if (tag == 201) {cout << "num" << " "  << tok.get_value() << endl;}
        else if (tag == 202) {cout << "ID" << " "  << tok.get_lexeme() << endl;}
        else if (tag >= 401 && tag <= 403) {cout << "comma" << " "  << tok.get_lexeme() << endl;}
        else if (tag == 501) {cout << "reg" << " "  << tok.get_lexeme() << endl;}
        else if (tag == 10) {cout << "end_of_input\n";}
        else {cout << "unexpected " << tag << endl;}
    }

    delete []input;
    return 0;
}
