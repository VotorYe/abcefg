#ifndef __DOCUSET_H__
#define __DOCUSET_H__
#include <map>
#include <stdio.h>
#include <fstream>
#include <dirent.h>
#include <vector>
#include "tokenizer.cpp"

using namespace std;
class DocuSet;

class DocuSet {
public:
    static char* content;
    static int length;
    static vector<token> tokens;

    // return true for file, false for dir
    bool input_file(string path) {
        return path.find(".input") != path.npos;
    }

    // read a file in to a char array
    char* read_from(string file, int& length) {
        filebuf *pbuf;
        ifstream filestr;
        long size;
        filestr.open (file.c_str(), ios::binary);
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

    // load the content, tokenize the content save as a vector
    void load_doc(string path) {
        if (content != NULL) {
            delete []content;
        }
        content = DocuSet::read_from(path, length);
        tokens.clear();
        tokens = tokenizer(path.c_str());
    }

    // return content (point)
    char* get_content(string filename) {
        return content;
    } 

    // return tokens 
    vector<token> get_tokens() {
        return tokens;
    }
};
char* DocuSet::content = NULL;
int DocuSet::length;
vector<token> DocuSet::tokens;
#endif
